#ifndef __COMMON_H__
#define __COMMON_H__

#include <gtk/gtk.h>

void show_message(gchar *message);
void show_errormessage(gchar *message);
gchar *af_file_get_name(gchar *filename);
gchar *af_file_get_ext(gchar *filename);

typedef enum{
    AWAY_SHOW_LIST = 0,
    AWAY_SHOW_APP = 1
}runmode;


#endif
