#include "server.h"
#include "global.h"
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <libsoup/soup.h>

static void
do_get (SoupServer *server, SoupMessage *msg, const char *path)
{
	char *slash;
	struct stat st;
	int fd;
	static char *path2 = NULL;

	if (!path2){g_free(path2); path2=NULL;}
	path2 = g_build_filename((const gchar *)global_get("htmlhome"), path, NULL);

	if (stat (path2, &st) == -1) {
		if (errno == EPERM)
			soup_message_set_status (msg, SOUP_STATUS_FORBIDDEN);
		else if (errno == ENOENT)
			soup_message_set_status (msg, SOUP_STATUS_NOT_FOUND);
		else
			soup_message_set_status (msg, SOUP_STATUS_INTERNAL_SERVER_ERROR);
		return;
	}

	if (S_ISDIR (st.st_mode)) {
		char *index_path;

		slash = strrchr (path, '/');
		if (!slash || slash[1]) {
			char *uri, *redir_uri;

			uri = soup_uri_to_string (soup_message_get_uri (msg), FALSE);
			redir_uri = g_strdup_printf ("%s/", uri);
			soup_message_headers_append (msg->response_headers,
						     "Location", redir_uri);
			soup_message_set_status (msg, SOUP_STATUS_MOVED_PERMANENTLY);
			g_free (redir_uri);
			g_free (uri);
			return;
		}

		index_path = g_strdup_printf ("%s/index.html", path);
		if (stat (index_path, &st) != -1) {
			do_get (server, msg, index_path);
			g_free (index_path);
			return;
		}

		char *buf = "<title>Forbidden</title><h1>Forbidden</h1>", len[10];
		sprintf(len, "%d", strlen(buf));

		soup_message_body_append (msg->response_body, SOUP_MEMORY_TAKE,
					  buf, strlen(buf));
        soup_message_headers_append (msg->response_headers,
                    "Content-Length", len);
		soup_message_set_status(msg, SOUP_STATUS_FORBIDDEN);
		return;
	}

	fd = open (path2, O_RDONLY);
	if (fd == -1) {
		soup_message_set_status (msg, SOUP_STATUS_INTERNAL_SERVER_ERROR);
		return;
	}

	if (msg->method == SOUP_METHOD_GET) {
		char *buf;

		buf = g_malloc (st.st_size);
		read (fd, buf, st.st_size);
		close (fd);
		soup_message_body_append (msg->response_body, SOUP_MEMORY_TAKE,
					  buf, st.st_size);
	} else /* msg->method == SOUP_METHOD_HEAD */ {
		char *length;

		/* We could just use the same code for both GET and
		 * HEAD. But we'll optimize and avoid the extra
		 * malloc.
		 */
		length = g_strdup_printf ("%lu", (gulong)st.st_size);
		soup_message_headers_append (msg->response_headers,
					     "Content-Length", length);
		g_free (length);
	}

	soup_message_set_status (msg, SOUP_STATUS_OK);
}

static void
do_put (SoupServer *server, SoupMessage *msg, const char *path)
{
	struct stat st;
	FILE *f;
	gboolean created = TRUE;
	static char *path2 = NULL;

	if (!path2){g_free(path2); path2=NULL;}

	path2 = g_build_filename((const gchar *)global_get("htmlhome"), path, NULL);

	if (stat (path2, &st) != -1) {
		const char *match = soup_message_headers_get_one (msg->request_headers, "If-None-Match");
		if (match && !strcmp (match, "*")) {
			soup_message_set_status (msg, SOUP_STATUS_CONFLICT);
			return;
		}

		if (!S_ISREG (st.st_mode)) {
			soup_message_set_status (msg, SOUP_STATUS_FORBIDDEN);
			return;
		}

		created = FALSE;
	}

	f = fopen (path2, "w");
	if (!f) {
		soup_message_set_status (msg, SOUP_STATUS_INTERNAL_SERVER_ERROR);
		return;
	}

	fwrite (msg->request_body->data, 1, msg->request_body->length, f);
	fclose (f);

	soup_message_set_status (msg, created ? SOUP_STATUS_CREATED : SOUP_STATUS_OK);
}

static void
server_callback (SoupServer *server, SoupMessage *msg,
		 const char *path, GHashTable *query,
		 SoupClientContext *context, gpointer data)
{
	SoupMessageHeadersIter iter;

	soup_message_headers_iter_init (&iter, msg->request_headers);


	if (msg->method == SOUP_METHOD_GET || msg->method == SOUP_METHOD_HEAD)
		do_get (server, msg, path);
	else if (msg->method == SOUP_METHOD_PUT)
		do_put (server, msg, path);
	else
		soup_message_set_status (msg, SOUP_STATUS_NOT_IMPLEMENTED);
}

int init_server()
{
    SoupServer *server;
    int port = SOUP_ADDRESS_ANY_PORT;

    server = soup_server_new (SOUP_SERVER_PORT, port,
                              SOUP_SERVER_SERVER_HEADER, "AwayFramework ",
                              NULL);
    if (!server) {
        g_log("AFR", G_LOG_LEVEL_ERROR, "Unable to bind to server port %d\n", port);
        return -1;
    }

    soup_server_add_handler (server, NULL,
                             server_callback, NULL, NULL);
    g_log("AFR", G_LOG_LEVEL_INFO, "Starting Server on port %d",
          soup_server_get_port (server));
    soup_server_run_async (server);

    return soup_server_get_port (server);
}
