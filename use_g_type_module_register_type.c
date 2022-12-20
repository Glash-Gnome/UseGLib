/**
 *	gcc use_g_type_module_register_type.c -o main `pkg-config --cflags --libs gobject-2.0`
 */
#include <gmodule.h>
#include <glib-object.h>


typedef struct _MyTypeModule {
    GTypeModule parent;
} MyTypeModule;

typedef struct _MyTypeModuleClass {
    GTypeModuleClass parent_class;
} MyTypeModuleClass;

#define MY_TYPE_TYPE_MODULE              (my_type_module_get_type ())
#define MY_TYPE_MODULE(module)           (G_TYPE_CHECK_INSTANCE_CAST ((module), MY_TYPE_TYPE_MODULE, MyTypeModule))
#define MY_TYPE_MODULE_CLASS(class)      (G_TYPE_CHECK_CLASS_CAST ((class), MY_TYPE_TYPE_MODULE, MyTypeModuleClass))
#define MY_IS_TYPE_MODULE(module)        (G_TYPE_CHECK_INSTANCE_TYPE ((module), MY_TYPE_TYPE_MODULE))
#define MY_IS_TYPE_MODULE_CLASS(class)   (G_TYPE_CHECK_CLASS_TYPE ((class), MY_TYPE_TYPE_MODULE))
#define MY_TYPE_MODULE_GET_CLASS(module) (G_TYPE_INSTANCE_GET_CLASS ((module), MY_TYPE_TYPE_MODULE, MyTypeModuleClass))

G_DEFINE_TYPE (MyTypeModule, my_type_module, G_TYPE_TYPE_MODULE)

static MyTypeModule *global_type_module;

MyTypeModule*
my_type_module_get ()
{
    if (global_type_module == NULL) {
        global_type_module = g_object_new (MY_TYPE_TYPE_MODULE, NULL);
    }

    return global_type_module;
}

static gboolean
my_type_module_load (GTypeModule *self)
{
    return TRUE;
}

static void
my_type_module_unload (GTypeModule *self)
{
    //g_assert_not_reached ();
}

static void
my_type_module_class_init (MyTypeModuleClass *klass)
{
    GTypeModuleClass *type_module_class;

    type_module_class = G_TYPE_MODULE_CLASS (klass);
    type_module_class->load = my_type_module_load;
    type_module_class->unload = my_type_module_unload;
}

static void
my_type_module_init (MyTypeModule *self)
{
    /* Prevent the use count from ever dropping to zero */
    g_type_module_use (G_TYPE_MODULE (self));
}


static void
my_object_class_init(GObjectClass *class,
                      gpointer      user_data)
{
	g_print("Class init: %s -> install properties %s\n", g_type_name_from_class((GTypeClass*)class), (char*)user_data);
}

static const char *global_class_data = "hello";

int
main (int argc, char *argv[])
{


    GType parent_type = G_TYPE_OBJECT;
    const gchar *type_name = "My-Object";
    GTypeInfo type_info = {
        0, /* class_size */

	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,

	(GClassInitFunc) my_object_class_init,
	(GClassFinalizeFunc) NULL,
	global_class_data,//NULL, /* class_data */

	0,    /* instance_size */
	0,    /* n_preallocs */
	(GInstanceInitFunc) NULL,
    };
    type_info.class_size = sizeof(GObjectClass);
    type_info.instance_size = sizeof(GObject);

    MyTypeModule *module = my_type_module_get();

    GTypeModule *plugin = G_TYPE_MODULE(module);
    GTypeFlags flags = 0;//G_TYPE_FLAG_NONE;

    GType type = g_type_module_register_type (plugin,
                             parent_type,
                             type_name,
                             &type_info,
                             flags);



    const gchar *name = "My-Object";
    GType t = g_type_from_name(name);
    GObject *obj = g_object_new(type, NULL);
    g_print("name = %s\n", g_type_name_from_instance((GTypeInstance*)obj));
    //g_print("name = %s\n", g_type_name(type));
    g_object_unref(obj);

    return 0;
}
