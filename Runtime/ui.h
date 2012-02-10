#ifndef __UI_H__
#define __UI_H__

int init_window();


int af_window_get_width();
int af_window_get_height();
gboolean af_window_get_fullscreen();
gchar *af_window_get_title();
gboolean af_window_get_visible();
gboolean af_window_get_resizable();
gboolean af_window_get_active();
int af_window_get_left();
int af_window_get_top();
gboolean af_window_get_border();
void af_window_set_width(int width);
void af_window_set_height(int height);
void af_window_set_fullscreen(gboolean fullscreen);
void af_window_set_title(char *title);
void af_window_set_resizable(gboolean resizable);
void af_window_set_visible(gboolean visible);
void af_window_set_left(int left);
void af_window_set_top(int top);
void af_window_set_border(gboolean border);
void af_window_set_position(int x, int y);
gboolean af_window_maximize();
gboolean af_window_minimize();
gboolean af_window_restore();
int af_window_get_state();
void af_window_set_above(gboolean above);
void af_window_set_below(gboolean below);

#endif
