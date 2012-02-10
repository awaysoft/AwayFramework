#include "run.h"
#include "global.h"
#include <webkit/webkit.h>
#include <string.h>

static gchar *get_url(gchar *url)
{
    static gchar *rurl = NULL;

    if (rurl) g_free(rurl);

    do{
        if (strstr(url, "http://") == url){
            rurl = g_strdup(url);
            break;
        }

        if (strstr(url, "away:") == url){
            rurl = g_strdup(url);
            break;
        }

        if (g_path_is_absolute(url)){
            rurl = g_build_filename("file:", url, NULL);
            break;
        }

        rurl = g_build_filename("file:", g_get_current_dir(), url, NULL);
    }while(0);

    g_log("AFR", G_LOG_LEVEL_INFO, "Loading URL: %s", rurl);
    return rurl;
}

static void load_url(void)
{
    GtkWidget * view;
    gchar * url;

    view = global_get("view");
    url = get_url(global_get("url"));
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(view), url);
}

static void load_app(void)
{
    load_url();
}

void run(void)
{
    GtkWidget * window;
    /* Load Application */
    load_app();

    window = global_get("window");

    /* Show Main Window */
    gtk_widget_show_all(window);

    gtk_main ();
}


void af_exit(GtkObject *object, gpointer user_data)
{
    gtk_main_quit();
}
