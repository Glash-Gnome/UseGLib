

static void
array_free_element(gpointer data) {
    GString **ptr = data;
    GString *str = *ptr;

    g_free(str->str);// g_strdup
    g_string_free(str, FALSE);
}

int
main (int argc, char *argv[])
{
    char *c_str = g_strdup("element");

    GString *str, *tmp;
    GArray *array;

    str = g_string_new( c_str );

    array = g_array_new(FALSE, FALSE, sizeof(GString*));
    g_array_set_clear_func(array, array_free_element);
    g_array_append_vals( array, &str, 1);

    tmp = g_array_index(array, GString*, 0);

  
    g_assert(tmp==str);


    g_free(c_str);
    g_array_free(array, TRUE);

    return 0;
}
