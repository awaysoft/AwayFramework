#ifndef __PLUGINS_H__
#define __PLUGINS_H__

#include <webkit/webkit.h>

typedef struct _plugin plugin;

int load_plugins();

void js_callback(WebKitWebView  *web_view,
                 WebKitWebFrame *frame,
                 gpointer        context,
                 gpointer        window_object,
                 gpointer        user_data);
#endif
