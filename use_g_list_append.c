#include <gmodule.h>

int
main(int argc, char *argv[]) {
    gchar *key1 = "key1";
    gchar *key2 = "key2";
    gchar *key3 = "key3";

    typedef GList GList_CStr;
    
    GList_CStr *list = NULL;
    list = g_list_append(list, key1);
    g_list_append(list, key2);
    g_list_append(list, key3);
    
    // output list :
    // "key2"
    // "key3"
    // "key1"
    
    
    return 0;
}
