/**
 * all: use_g_type_register_dynamic.c
 *	gcc use_g_type_register_dynamic.c -o main `pkg-config --cflags --libs gobject-2.0`
 */
#include <gmodule.h>

int
main (int argc, char *argv[])
{
    char *c_str = "My-Object";

    return 0;
}
