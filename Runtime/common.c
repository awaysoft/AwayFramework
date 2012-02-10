#include "common.h"
#include "global.h"
#include <string.h>

void show_message(gchar *message)
{
    gpointer window = global_get("window");
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new (GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, message);
    gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

void show_errormessage(gchar *message)
{
    gpointer window = global_get("window");
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new (GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, message);
    gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

gchar *af_file_get_name(gchar *filename)
{
    gchar *name = g_path_get_basename(filename);

    gchar *result = g_strrstr(name, ".");
    if (!result) return name;

    *result = '\0';
    result = g_strdup(name);
    g_free(name);

    return result;
}

gchar *af_file_get_ext(gchar *filename)
{
    gchar *name = g_path_get_basename(filename);

    gchar *result = g_strrstr(name, ".");
    if (!result) return NULL;

    result = g_strdup(result+1);
    g_free(name);

    return result;
}
