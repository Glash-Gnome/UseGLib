#include <gmodule.h>

int
main(int argc, char *argv[]) {
    gchar *key1 = "key1";
    gchar *key2 = "key2";
    gchar *key3 = "key3";

    typedef GList GList_CStr;
    GList_CStr *list;
    
    list = g_list_append(NULL, key1);
    g_list_append(list, key2);
    g_list_append(list, key3);
    
    // output list :
    // "key1"
    // "key2"
    // "key3"
    
    list = g_list_prepend(NULL, key1);//            output list : {0: key1}
    g_list_prepend(list, key2);//          output list : {0: key2, 1: key1}
    g_list_prepend(list, key3);// output list : {0: key2, 1: key3, 2: key1}
    // output list :
    // "key2"
    // "key3"
    // "key1"
    
    
    return 0;
}
