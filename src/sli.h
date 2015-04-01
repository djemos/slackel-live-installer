GtkBuilder *widgetstree;

guint outputsource;
GIOChannel *stdoutioc;
GPid pid;

gint copydevicescount;
gint installdevicescount;
gint homedevicescount;

void on_copy_btn_clicked (GtkWidget *widget, gpointer user_data);

void on_install_btn_clicked (GtkWidget *widget, gpointer user_data);

void on_cancel_btn_clicked (GtkWidget *widget, gpointer user_data);

gboolean progress_handler (GIOChannel *source, GIOCondition condition, gpointer data);

void on_process_end (GPid thepid, gint status, gpointer data);

void do_action(gboolean save);

void on_exit (GtkWidget *widget, gpointer user_data);

void initlocations();
