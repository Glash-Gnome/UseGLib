#include <gmodule.h>

int
main (int argc, char *argv[])
{
    char *c_str = "element";
    GString *str, *tmp;
    GArray *array;

    str = g_string_new( c_str );

    array = g_array_new(FALSE, FALSE, sizeof(GString*));
    g_array_append_vals( array, &str, 1);// g_array_append_val( array, str);

    tmp = g_array_index(array, GString*, 0);
    g_assert(tmp==str);


    g_free(array->data);
    g_array_free(array, FALSE);

    g_free(str->str);
    g_string_free(str, FALSE);

    return 0;
}
