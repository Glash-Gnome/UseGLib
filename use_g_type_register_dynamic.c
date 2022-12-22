/**
 *	gcc use_g_type_register_dynamic.c -o main `pkg-config --cflags --libs gobject-2.0`
 */
#include <gmodule.h>
#include <glib-object.h>

#define TRACE_OFF()
#define TRACE_ON() g_print("\e[32;1m%s\e[0;m\n", __FUNCTION__)
#define TRACE() TRACE_ON()

#define USE_DYNAMIC 0

// #######################################################################

G_DECLARE_DERIVABLE_TYPE(MyObject, my_object, MY, TYPE_OBJECT, GObject)
/*struct _MyObject {
    GObject parent;
};*/

struct _MyObjectClass {
    GObjectClass parent_class;
};
G_DEFINE_TYPE (MyObject, my_object, G_TYPE_OBJECT)


static void
my_object_class_init (MyObjectClass *klass)
{
    TRACE();
}

static void
my_object_init (MyObject *self)
{
    TRACE();
}


// #######################################################################

typedef struct _MyTypeModule {
    GTypeModule parent;
} MyTypeModule;

typedef struct _MyTypeModuleClass {
    GTypeModuleClass parent_class;
} MyTypeModuleClass;

G_DEFINE_TYPE (MyTypeModule, my_type_module, G_TYPE_TYPE_MODULE)

static MyTypeModule *global_type_module;

MyTypeModule*
my_type_module_get ()
{
    TRACE();
    if (global_type_module == NULL) {
        global_type_module = g_object_new (my_type_module_get_type(), NULL);
    }

    return global_type_module;
}

static gboolean
my_type_module_load (GTypeModule *self)
{
    TRACE();
    return TRUE;
}

static void
my_type_module_unload (GTypeModule *self)
{
    TRACE();
    //g_assert_not_reached ();
}

static void
my_type_module_class_init (MyTypeModuleClass *klass)
{
    TRACE();
    GTypeModuleClass *type_module_class;

    type_module_class = G_TYPE_MODULE_CLASS (klass);
    type_module_class->load = my_type_module_load;
    type_module_class->unload = my_type_module_unload;
}

static void
my_type_module_init (MyTypeModule *self)
{
    TRACE();

    /* Prevent the use count from ever dropping to zero */
    g_type_module_use (G_TYPE_MODULE (self));
}



int
main (int argc, char *argv[])
{
#if 0
    GType type = my_object_get_type();
    GObject *obj = g_object_new(type, NULL);
    g_print("name = %s\n", g_type_name_from_instance((GTypeInstance*)obj) );
    g_clear_object(&obj);
#endif


/*
    const gchar *type_init_name = "G-Object";
    const gchar *type_base_name = "My-Object";
    const gchar *type_name = "My-Person";
*/


    GType parent_type = g_object_get_type();
    const gchar *type_name = "Php-Object";
    MyTypeModule *module = my_type_module_get();
    GTypeFlags flags = 0;//G_TYPE_FLAG_NONE;
    GType type = g_type_register_dynamic (parent_type, type_name,
                         G_TYPE_PLUGIN(module),
                         flags);
    GTypeModule *m = G_TYPE_MODULE(module);
      /*module_type_info = g_new (ModuleTypeInfo, 1);
      
      module_type_info->parent_type = parent_type;
      module_type_info->type = g_type_register_dynamic (parent_type, type_name, G_TYPE_PLUGIN (module), flags);
      
      module->type_infos = g_slist_prepend (module->type_infos, module_type_info);*/

    //g_print("m = %p == NULL\n", m->type_infos/*->data*/);



    g_print("name = %s\n", g_type_name(type));
    //GTypeInstance *instance = g_type_create_instance (type);


    /*GObject *obj = g_object_new(type, NULL);
    const gchar *name = "My-Object";
    GType t = g_type_from_name(name);
    g_print("name = %s\n", g_type_name(t));
    g_print("name = %s\n", g_type_name(type));*/


    return 0;
}
