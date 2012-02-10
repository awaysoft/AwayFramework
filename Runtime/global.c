#include "global.h"


static GHashTable * global   = NULL;
static GHashTable * setting  = NULL;

void init_global ()
{
    if (global == NULL)
        global = g_hash_table_new(g_str_hash, g_str_equal);

    if (setting == NULL)
        setting   = g_hash_table_new(g_str_hash, g_str_equal);
}

gpointer global_get (const gchar *key)
{
    return g_hash_table_lookup(global, key);
}

gboolean global_remove (const gchar *key)
{
    return g_hash_table_remove(global, key);
}

GList * global_get_keys()
{
    return g_hash_table_get_keys(global);
}

void global_set (const gchar *key, const gpointer value)
{
    g_hash_table_insert(global, (gpointer)key, value);
}

gpointer setting_get (const gchar *key)
{
    return g_hash_table_lookup(setting, key);
}

void setting_set (const gchar *key, const gpointer value)
{
    g_hash_table_insert(setting, (gpointer)key, value);
}

gboolean setting_remove (const gchar *key)
{
    return g_hash_table_remove(setting, key);
}

GList * setting_get_keys()
{
    return g_hash_table_get_keys(setting);
}
