#include "global.h"
#include "ui.h"
#include "run.h"
#include "plugins.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <webkit/webkit.h>

static GdkWindowState window_state = 0;

static void screen_get_size(gint *width, gint *height)
{
    GdkScreen* screen;
    screen  = gdk_screen_get_default();
    *width  = gdk_screen_get_width(screen);
    *height = gdk_screen_get_height(screen);
}

static gboolean webview_onclose (WebKitWebView *web_view, gpointer user_data);
static gboolean window_onstatechange (GtkWidget *widget, GdkEvent *event, gpointer user_data);

static int init_setting(GtkWidget * window, GtkWidget * view)
{
    gint root_x, root_y;
    gint screen_width, screen_height;
    /* Setting Window */
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), global_get("title"));
    gtk_widget_set_size_request(GTK_WIDGET(window), 10, 10);
    gtk_window_set_resizable(GTK_WINDOW(window), (gboolean)global_get("resizeable"));
    gtk_window_set_decorated(GTK_WINDOW(window), (gboolean)global_get("border"));

    if (gtk_window_get_resizable(GTK_WINDOW(window))) {
        gtk_window_resize(GTK_WINDOW(window), (gint)global_get("width"), (gint)global_get("height"));
    } else {
        gtk_widget_set_size_request(GTK_WIDGET(window), (gint)global_get("width"), (gint)global_get("height"));
    }

    /* Move Window */
    screen_get_size(&screen_width, &screen_height);

    root_x = (screen_width - (gint)global_get("width")) / 2;
    root_y = (screen_height - (gint)global_get("height")) / 2;

    if ((gint)global_get("x") != 10000) root_x = (gint)global_get("x");
    if ((gint)global_get("y") != 10000) root_y = (gint)global_get("y");

    if (root_x < 0) root_x = screen_width + root_x;
    if (root_y < 0) root_y = screen_height + root_y;

    gtk_window_move(GTK_WINDOW(window), root_x, root_y);

    /* Setting Icon */
    if (g_file_test(global_get("icon"), G_FILE_TEST_EXISTS)) {
        gtk_window_set_icon_from_file(GTK_WINDOW(window), global_get("icon"), NULL);
    }
    /* Fullscreen */
    if ((gboolean)global_get("fullscreen")) af_window_set_fullscreen(TRUE);


    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(af_exit), NULL);
    g_signal_connect(G_OBJECT(window), "window-state-event", G_CALLBACK(window_onstatechange), NULL);

    /* Setting Webkit */
    WebKitWebSettings *setting = webkit_web_view_get_settings (WEBKIT_WEB_VIEW(view));

    g_object_set(G_OBJECT(setting), "default-encoding", global_get("encode"), NULL);
    g_object_set(G_OBJECT(setting), "enable-webgl", TRUE, NULL);
    //g_object_set(G_OBJECT(view), "self-scrolling", TRUE, NULL);
    g_object_set(G_OBJECT(setting), "default-font-family", "SimSun", NULL);
    g_object_set(G_OBJECT(setting), "monospace-font-family", "SimSun", NULL);
    g_object_set(G_OBJECT(setting), "sans-serif-font-family", "SimSun", NULL);
    g_object_set(G_OBJECT(setting), "serif-font-family", "SimSun", NULL);
    g_object_set(G_OBJECT(setting), "cursive-font-family", "SimSun", NULL);

    webkit_web_view_set_settings(WEBKIT_WEB_VIEW(view), setting);

    g_signal_connect(WEBKIT_WEB_VIEW(view), "window-object-cleared", G_CALLBACK(js_callback), NULL);
    g_signal_connect(WEBKIT_WEB_VIEW(view), "close-web-view", G_CALLBACK(webview_onclose), NULL);

    return 0;
}

int init_window()
{
    GtkWidget * window;
    GtkWidget * scroll_window;
    GtkWidget * view;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    view = webkit_web_view_new();
    scroll_window = gtk_scrolled_window_new(NULL, NULL);

    global_set("window", window);
    global_set("view", view);
    global_set("scrollwindow", scroll_window);

    init_setting(window, view);


    // gtk_container_add(GTK_CONTAINER(window), view);
    gtk_container_add(GTK_CONTAINER(window), scroll_window);
    gtk_container_add(GTK_CONTAINER(scroll_window), view);


    return 0;
}

static gboolean webview_onclose (WebKitWebView *web_view,
                                 gpointer       user_data)
{
    af_exit(global_get("window"), NULL);
    return TRUE;
}

static gboolean window_onstatechange (GtkWidget *widget,
                                      GdkEvent *event,
                                      gpointer user_data)
{
    window_state = event->window_state.new_window_state;
    return FALSE;
}

int af_window_get_width()
{
    int width, height;
    GtkWidget *window = global_get("window");

    if (gtk_window_get_resizable(GTK_WINDOW(window))) {
        gtk_window_get_size(GTK_WINDOW(window), &width, &height);
    } else {
        gtk_widget_get_size_request(window, &width, &height);
    }

    return width;
}

int af_window_get_height()
{
    int width, height;
    GtkWidget *window = global_get("window");

    if (gtk_window_get_resizable(GTK_WINDOW(window))) {
        gtk_window_get_size(GTK_WINDOW(window), &width, &height);
    } else {
        gtk_widget_get_size_request(window, &width, &height);
    }

    return height;
}

gboolean af_window_get_fullscreen()
{
    return (gboolean)global_get("fullscreen");
}

gchar *af_window_get_title()
{
    GtkWidget *window = global_get("window");
    return (gchar *)gtk_window_get_title(GTK_WINDOW(window));
}

gboolean af_window_get_visible()
{
    GtkWidget *window = global_get("window");
    return gtk_widget_get_visible(window);
}

gboolean af_window_get_resizable()
{
    GtkWidget *window = global_get("window");
    return gtk_window_get_resizable(GTK_WINDOW(window));
}

gboolean af_window_get_active()
{
    GtkWidget *window = global_get("window");
    return gtk_window_is_active(GTK_WINDOW(window));
}

int af_window_get_left()
{
    GtkWidget *window = global_get("window");
    int x, y;
    gtk_window_get_position(GTK_WINDOW(window), &x, &y);
    return x;
}

int af_window_get_top()
{
    GtkWidget *window = global_get("window");
    int x, y;
    gtk_window_get_position(GTK_WINDOW(window), &x, &y);
    return y;
}

gboolean af_window_get_border()
{
    GtkWidget *window = global_get("window");
    return gtk_window_get_decorated(GTK_WINDOW(window));
}

void af_window_set_width(int width)
{
    if (width < 0) return ;
    global_set("width", (gpointer)width);

    GtkWidget *window = global_get("window");

    if (gtk_window_get_resizable(GTK_WINDOW(window))) {
        gtk_window_resize(GTK_WINDOW(window), (gint)global_get("width"), (gint)global_get("height"));
    } else {
        gtk_widget_set_size_request(GTK_WIDGET(window), (gint)global_get("width"), (gint)global_get("height"));
    }
}

void af_window_set_height(int height)
{
    if (height < 0) return ;
    global_set("height", (gpointer)height);

    GtkWidget *window = global_get("window");

    if (gtk_window_get_resizable(GTK_WINDOW(window))) {
        gtk_window_resize(GTK_WINDOW(window), (gint)global_get("width"), (gint)global_get("height"));
    } else {
        gtk_widget_set_size_request(GTK_WIDGET(window), (gint)global_get("width"), (gint)global_get("height"));
    }
}

void af_window_set_fullscreen(gboolean fullscreen)
{
    GtkWidget *window = global_get("window");

    global_set("fullscreen", (gpointer)fullscreen);

    if (fullscreen)
        gtk_window_fullscreen(GTK_WINDOW(window));
    else
        gtk_window_unfullscreen(GTK_WINDOW(window));
}

void af_window_set_title(char *title)
{
    GtkWidget *window = global_get("window");

    global_set("title", title);
    gtk_window_set_title(GTK_WINDOW(window), title);
}

void af_window_set_resizable(gboolean resizable)
{
    GtkWidget *window = global_get("window");
    global_set("resizable", (gpointer)resizable);
    gtk_window_set_resizable(GTK_WINDOW(window), resizable);
}

void af_window_set_visible(gboolean visible)
{
    GtkWidget *window = global_get("window");
    gtk_widget_set_visible(window, visible);
}

void af_window_set_left(int left)
{
    GtkWidget *window = global_get("window");
    int x, y;
    gtk_window_get_position(GTK_WINDOW(window), &x, &y);
    x = left;
    global_set("x", (gpointer)x);
    gtk_window_move(GTK_WINDOW(window), x, y);
}

void af_window_set_top(int top)
{
    GtkWidget *window = global_get("window");
    int x, y;
    gtk_window_get_position(GTK_WINDOW(window), &x, &y);
    y = top;
    global_set("y", (gpointer)y);
    gtk_window_move(GTK_WINDOW(window), x, y);
}

void af_window_set_border(gboolean border)
{
    GtkWidget *window = global_get("window");
    global_set("border", (gpointer)border);
    gtk_window_set_decorated(GTK_WINDOW(window), border);
}

void af_window_set_position(int x, int y)
{
    GtkWidget *window = global_get("window");
    global_set("x", (gpointer)x);
    global_set("y", (gpointer)y);
    gtk_window_move(GTK_WINDOW(window), x, y);
}

gboolean af_window_maximize()
{
    if (!af_window_get_visible()) return FALSE;

    GtkWidget *window = global_get("window");
    gtk_window_maximize(GTK_WINDOW(window));
    return TRUE;
}

gboolean af_window_minimize()
{
    if (!af_window_get_visible()) return FALSE;

    GtkWidget *window = global_get("window");
    gtk_window_iconify(GTK_WINDOW(window));
    return TRUE;
}

gboolean af_window_restore()
{
    if (!af_window_get_visible()) return FALSE;

    GtkWidget *window = global_get("window");
    switch(af_window_get_state()){
        case GDK_WINDOW_STATE_WITHDRAWN: af_window_set_visible(TRUE);
            break;
        case GDK_WINDOW_STATE_ICONIFIED: gtk_window_deiconify(GTK_WINDOW(window));
            break;
        case GDK_WINDOW_STATE_MAXIMIZED: gtk_window_unmaximize(GTK_WINDOW(window));
            break;
        case GDK_WINDOW_STATE_STICKY: gtk_window_unstick(GTK_WINDOW(window));
            break;
        case GDK_WINDOW_STATE_FULLSCREEN: af_window_set_fullscreen(FALSE);
            break;
        case GDK_WINDOW_STATE_ABOVE: af_window_set_above(FALSE);
            break;
        case GDK_WINDOW_STATE_BELOW: af_window_set_below(FALSE);
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

int af_window_get_state()
{
    return window_state;
}

void af_window_set_above(gboolean above)
{
    GtkWidget *window = global_get("window");
    gtk_window_set_keep_above(GTK_WINDOW(window), above);
}

void af_window_set_below(gboolean below)
{
    GtkWidget *window = global_get("window");
    gtk_window_set_keep_below(GTK_WINDOW(window), below);
}

