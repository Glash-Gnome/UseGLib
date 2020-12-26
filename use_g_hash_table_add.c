#include <gmodule.h>

void no_transfer() {
    gchar *key1 = g_strdup("key1");
    gchar *key2 = g_strdup("key2");
    gchar *key3 = g_strdup("key3");
    
    {
        typedef GHashTable GHashSet_String;
        GHashSet_String *table = g_hash_table_new(g_str_hash, g_str_equal);
    
        g_hash_table_add(table, key1);
        g_hash_table_add(table, key2);
        g_hash_table_add(table, key3);
    
        g_hash_table_destroy(table);
    }
    
    g_free(key1);
    g_free(key2);
    g_free(key3);
}

int
main(int argc, char *argv[]) {
    gchar *key1 = "key1";
    gchar *key2 = "key2";
    gchar *key3 = "key3";

    typedef GHashTable GHashSet_String;
    GHashSet_String *table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    
    g_hash_table_add(table, g_strdup(key1));
    g_hash_table_add(table, g_strdup(key2));
    g_hash_table_add(table, g_strdup(key3));
    
    g_hash_table_contain(table, "key3");
    
    g_hash_table_destroy(table);
    
    return 0;
}
