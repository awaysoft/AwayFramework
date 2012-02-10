#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <glib.h>
#include <glib-object.h>

#define APP_NAME "AwayFramework"
#define VERSION 1
#define VERSION_SHOW "0.1"
#define PLATFORM 1
#ifdef WIN32
#define OS 1
#endif
#ifdef __linux__
#define OS 2
#endif
#ifdef MACOS
#define OS 4
#endif


void init_global ();
gpointer global_get (const gchar *key);
void global_set (const gchar *key, const gpointer value);
gboolean global_remove (const gchar *key);
GList * global_get_keys();
gpointer setting_get (const gchar *key);
void setting_set (const gchar *key, const gpointer value);
gboolean setting_remove (const gchar *key);
GList * setting_get_keys();

#endif
