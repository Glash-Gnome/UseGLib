/**
 * button callback:
 * If you want to run the worker thread in the main gtk loop,
 * you can directly make the computation in the button callback,
 * updating the GUI and treating events from it with gtk_event_pending() and gtk_main_iteration(),
 * as in the following sample code:
 */

#include <gtk/gtk.h>


typedef struct _AppWorker {
    //mutable std::mutex m_Mutex;

    // Data used by both GUI thread and worker thread.
    gboolean shall_stop;
    gboolean is_running;
    gdouble fraction_done;
    GString *message;

} AppWorker;

static AppWorker app_worker = {FALSE, FALSE, 0.0, NULL};

typedef struct _AppData {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *button_box;
    GtkWidget *button_start;
    GtkWidget *button_stop;
    GtkWidget *button_quit;
    GtkWidget *progress_bar;
    GtkWidget *scrolled_window;
    GtkWidget *text_view;

    AppWorker *app_worker;

} AppData;
static AppData app_data = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL, &app_worker};

static void
update_start_stop_buttons()
{
  gtk_widget_set_sensitive(app_data.button_start, !app_data.app_worker->is_running);
  gtk_widget_set_sensitive(app_data.button_stop, app_data.app_worker->is_running);
}

static void app_work(AppData *data) {
    // huge computation in a loop:
    int step = 5;
    int i=0;
    while(1) {
        if (i>step) break;
        if(data->app_worker->shall_stop) {
            break;
        }

        // do some computation...
        usleep(1000*1000);// Notice this bloque the GUI 1s...
        i++;

        // stop if stop button
        gdouble frac = data->app_worker->fraction_done = (gdouble)i/step;
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(data->progress_bar), frac);

      // update the GUI and treat events from it:
      while(gtk_events_pending()) gtk_main_iteration();
    }
}

static void
on_start_button_clicked(GObject *self, gpointer user_data)
{
    AppData *data = (AppData*) user_data;

    // modify the GUI:
    gdouble frac = data->app_worker->fraction_done = 0.0;
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(data->progress_bar), frac);
    data->app_worker->is_running = TRUE;
    data->app_worker->shall_stop = FALSE;
    update_start_stop_buttons();

    // run the main iteration to update the GUI,
    // you need to call these functions even if the GUI wasn't modified,
    // in order to get it responsive and treat events from it:
    while(gtk_events_pending()) gtk_main_iteration();

    // do some other computation...
    app_work(data);

    data->app_worker->is_running = FALSE;
    update_start_stop_buttons();

}

static void
on_stop_button_clicked(GObject *self, gpointer user_data)
{
    AppData *data = (AppData*)user_data;
    if(data->app_worker->is_running) {
        data->app_worker->shall_stop = TRUE;
    }
}

static void
on_quit_button_clicked()
{
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{


  app_data.window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (app_data.window), "Multi-threaded example");
  gtk_window_set_default_size (GTK_WINDOW (app_data.window), 200, 200);

  app_data.vbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  app_data.button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
  app_data.button_start = gtk_button_new_with_label("Start work");
  app_data.button_stop = gtk_button_new_with_label("Stop work");
  app_data.button_quit = gtk_button_new_with_mnemonic("_Quit");
  app_data.progress_bar = gtk_progress_bar_new();
  app_data.scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  app_data.text_view = gtk_text_view_new();


  gtk_container_add (GTK_CONTAINER (app_data.window), app_data.vbox);

  // Add the ProgressBar.
  gtk_box_pack_start(GTK_BOX(app_data.vbox), app_data.progress_bar, TRUE, TRUE, 0);

  gtk_progress_bar_set_text(GTK_PROGRESS_BAR(app_data.progress_bar), "Fraction done");
  gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(app_data.progress_bar), TRUE);

  // Add the TextView, inside a ScrolledWindow.
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(app_data.scrolled_window), app_data.text_view);

  // Only show the scrollbars when they are necessary.
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(app_data.scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gtk_box_pack_start(GTK_BOX(app_data.vbox), app_data.scrolled_window, TRUE, TRUE, 0);

  gtk_text_view_set_editable(GTK_TEXT_VIEW(app_data.text_view), FALSE);

  // Add the buttons to the ButtonBox.
  gtk_box_pack_start(GTK_BOX(app_data.vbox), app_data.button_box, TRUE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(app_data.button_box), app_data.button_start, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(app_data.button_box), app_data.button_stop, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(app_data.button_box), app_data.button_quit, TRUE, TRUE, 0);

  gtk_container_set_border_width(GTK_CONTAINER(app_data.button_box), 5);
  //m_ButtonBox.set_spacing(5);
  gtk_button_box_set_layout(GTK_BUTTON_BOX(app_data.button_box), GTK_BUTTONBOX_END);


  // Connect the signal handlers to the buttons.
  g_signal_connect (app_data.button_start, "clicked", G_CALLBACK (on_start_button_clicked), &app_data);
  g_signal_connect (app_data.button_stop, "clicked", G_CALLBACK (on_stop_button_clicked), &app_data);
  g_signal_connect (app_data.button_quit, "clicked", G_CALLBACK (on_quit_button_clicked), &app_data);

  // Connect the handler to the dispatcher.
  //m_Dispatcher.connect(sigc::mem_fun(*this, &ExampleWindow::on_notification_from_worker_thread));

  // Create a text buffer mark for use in update_widgets().
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_data.text_view));
  GtkTextIter end;
  gtk_text_buffer_get_end_iter(buffer, &end);
  gtk_text_buffer_create_mark(buffer, "last_line", &end, TRUE);

  update_start_stop_buttons();


  gtk_widget_show_all (app_data.window);
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gnome.App", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
