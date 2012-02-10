#include <stdlib.h>
#include <gtk/gtk.h>

#include "init.h"
#include "ui.h"
#include "plugins.h"
#include "run.h"

#define CALL(x) do{int return_var = x;\
if (return_var > 0) return return_var;\
else if (return_var < 0) return 0;\
}while(0)

int main (int argc, char *argv[])
{
    /* Initialize GTK+ */
    g_log_set_handler ("AFR", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
    gtk_init (&argc, &argv);
    g_log_set_handler ("AFR", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);


    /* Initialize System */
    g_log("AFR", G_LOG_LEVEL_INFO, "Initing System...");
    CALL(init(argc, argv));


    /* Create Main Window */
    g_log("AFR", G_LOG_LEVEL_INFO, "Initing User Interface...");
    CALL(init_window());

    g_log("AFR", G_LOG_LEVEL_INFO, "Initing Plugins...");
    /* Loading Plugins */
    CALL(load_plugins());

    g_log("AFR", G_LOG_LEVEL_INFO, "Running...");
    /* Run */
    run();

    return 0;
}
