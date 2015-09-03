/*	- added support for setting root password on installation system by [Dimitris Tzemos <djemos@slackel.gr>]
	- adeed support for creating a user name and user password on installation system by [Dimitris Tzemos <djemos@slackel.gr>]
	- added partition manager connectivity in gui by [Dimitris Tzemos <djemos@slackel.gr>]
	*/

#include <gtk/gtk.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <libintl.h>
#include <locale.h>
#define _(String) gettext (String)
#define N_(String) String
#include "config.h"

#include "sli.h"

void do_action (gboolean copy) {
	gchar *commandline, **command, *location, *home;
	gint stdoutfd;
	GtkTextView *view;
	GtkTextBuffer *buffer;
	GtkComboBox *listwidget;
	GtkTreeIter iter;
	GtkListStore *list;
	char *rootpasswd,*usernam,*userpasswd, *installation_mode, *rootdirectory, *bootloader;
	
	GtkWidget *rootpassword;
	GtkWidget *username;
	GtkWidget *userpassword;
	GtkWidget *core, *basic, *full;
	core = (GtkWidget *) gtk_builder_get_object(widgetstree, "core");
	basic = (GtkWidget *) gtk_builder_get_object(widgetstree, "basic");
	full = (GtkWidget *) gtk_builder_get_object(widgetstree, "full");
	
	
	 rootpassword = (GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword");
	 username = (GtkWidget *) gtk_builder_get_object(widgetstree, "username");
	 userpassword = (GtkWidget *) gtk_builder_get_object(widgetstree, "userpassword");
	
	rootpasswd = g_strdup (gtk_entry_get_text(GTK_ENTRY(rootpassword)));
	usernam =  g_strdup (gtk_entry_get_text(GTK_ENTRY(username)));
	userpasswd =  g_strdup (gtk_entry_get_text(GTK_ENTRY(userpassword)));
	
	const gchar *DW[] = { "installdevices", "copydevices" };
	//
	listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, "homedevices");
	gtk_combo_box_get_active_iter(listwidget, &iter);
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	gtk_tree_model_get((GtkTreeModel *) list, &iter, 0, &home, -1);
	if (strlen(home) == 0) {
		g_free(home);
		home = g_strdup(location);
	}
	//
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (core))) {
		installation_mode = g_strdup ("core") ;
		rootdirectory = g_strdup ("modules");
		} 
	
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (basic))) {
     installation_mode = g_strdup ("basic") ;
     rootdirectory = g_strdup ("modules");
		}
	
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (full))) {
		installation_mode = g_strdup ("full") ;
		rootdirectory = g_strdup ("modules");
		//rootdirectory = g_strdup ("system");
		}
	
	listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, DW[(guint) copy]);
	gtk_combo_box_get_active_iter(listwidget, &iter);
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	gtk_tree_model_get((GtkTreeModel *) list, &iter, 0, &location, -1);
	if (location == NULL) {
		return;
	}
	if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "lilo"))) {
		bootloader = g_strdup ("lilo");
	   } 
	else if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "grub"))){
		bootloader = g_strdup ("grub");
	 }
		
	if (copy) {
		commandline = g_strdup_printf("build-slackware-live.sh --usb /live/media %s\n", location);
	} else {
			if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "lilo"))) {
					commandline = g_strdup_printf("build-slackware-live.sh --install /live/%s %s -auto %s %s %s %s %s %s\n", rootdirectory, location, rootpasswd, usernam, userpasswd, installation_mode, bootloader, home);			
			} 
			else if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "grub"))) {
					commandline = g_strdup_printf("build-slackware-live.sh --install /live/%s %s -auto %s %s %s %s %s %s\n", rootdirectory, location, rootpasswd, usernam, userpasswd, installation_mode, bootloader, home);			
			} else { 
					commandline = g_strdup_printf("build-slackware-live.sh --install /live/%s %s -expert %s %s %s %s %s\n", rootdirectory, location, rootpasswd, usernam, userpasswd, installation_mode, home);
			 }
	       }

	g_free(location);
	view = (GtkTextView *) gtk_builder_get_object(widgetstree, "log");
	buffer = gtk_text_view_get_buffer(view);
	gtk_text_buffer_set_text(buffer, commandline, -1);
	g_shell_parse_argv(commandline, NULL, &command, NULL);
	g_free(commandline);
	g_spawn_async_with_pipes(NULL, command, NULL, G_SPAWN_SEARCH_PATH|G_SPAWN_DO_NOT_REAP_CHILD, NULL, NULL, &pid, NULL, &stdoutfd, NULL, NULL);
	stdoutioc = g_io_channel_unix_new(stdoutfd);
	outputsource = g_io_add_watch(stdoutioc, G_IO_IN, progress_handler, NULL);
	g_child_watch_add(pid, on_process_end, NULL);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "copy_btn"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "install_btn"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "cancel_btn"), TRUE);
	
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword1"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "username"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "userpassword"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "userpassword1"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "checkbutton1"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "checkbutton2"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "button2"), FALSE);
	
	
	g_strfreev(command);
}

void on_button1_clicked (GtkWidget *widget, gpointer user_data) {
	GtkWidget *dialogusers;
	dialogusers = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialogusers");
	gtk_widget_hide(dialogusers);
}

void on_button3_clicked (GtkWidget *widget, gpointer user_data) {
	GtkWidget *dialogrootpass;
	dialogrootpass = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialogrootpass");
	gtk_widget_hide(dialogrootpass);
}

void on_button4_clicked (GtkWidget *widget, gpointer user_data) {
	GtkWidget *dialoguserpass;
	dialoguserpass = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialoguserpass");
	gtk_widget_hide(dialoguserpass);
}

void on_button5_clicked (GtkWidget *widget, gpointer user_data) {
	GtkWidget *dialogbootloader;
	dialogbootloader = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialogbootloader");
	gtk_widget_hide(dialogbootloader);
}


void on_checkbutton1_toggled (GtkWidget *widget, gpointer user_data) {
 	if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "checkbutton1"))) {
		gtk_entry_set_visibility (GTK_ENTRY ((GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword")), TRUE);
		gtk_entry_set_visibility (GTK_ENTRY ((GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword1")), TRUE);
	} else {
		gtk_entry_set_visibility (GTK_ENTRY ((GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword")), FALSE);
		gtk_entry_set_visibility (GTK_ENTRY ((GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword1")), FALSE);
		}
}

void on_checkbutton2_toggled (GtkWidget *widget, gpointer user_data) {
 	if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "checkbutton2"))) {
		gtk_entry_set_visibility (GTK_ENTRY ((GtkWidget *) gtk_builder_get_object(widgetstree, "userpassword")), TRUE);
		gtk_entry_set_visibility (GTK_ENTRY ((GtkWidget *) gtk_builder_get_object(widgetstree, "userpassword1")), TRUE);
	} else {
		gtk_entry_set_visibility (GTK_ENTRY ((GtkWidget *) gtk_builder_get_object(widgetstree, "userpassword")), FALSE);
		gtk_entry_set_visibility (GTK_ENTRY ((GtkWidget *) gtk_builder_get_object(widgetstree, "userpassword1")), FALSE);
		}	
}

void clearlocations() {
	GtkComboBox *listwidget;
	GtkListStore *list;
	// clear copydevices
	listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, "copydevices");
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	gtk_list_store_clear (list) ;
    // Clear installdevices
	gtk_list_store_clear (list) ;
	listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, "installdevices");
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	gtk_list_store_clear (list) ;
	// Clear homedevices
	gtk_list_store_clear (list) ;
	listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, "homedevices");
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	gtk_list_store_clear (list) ;
	
}

void on_button2_clicked (GtkWidget *widget, gpointer user_data) {
	if (g_file_test("/usr/sbin/gparted", G_FILE_TEST_EXISTS)) {
	system("/usr/sbin/gparted");
	clearlocations();
	initlocations();
	}
}

void on_copy_btn_clicked (GtkWidget *widget, gpointer user_data) {
	do_action(TRUE);
}


void on_install_btn_clicked (GtkWidget *widget, gpointer user_data) {
	GtkWidget *dialogusers;
	GtkWidget *dialogrootpass;
	GtkWidget *dialoguserpass;
	GtkWidget *dialogbootloader;
	GtkWidget *rootpassword;
	GtkWidget *username;
	GtkWidget *userpassword;
	GtkWidget *rootpassword1;
	GtkWidget *userpassword1;
	GtkWidget *button1;
	GtkWidget *button3;
	GtkWidget *button4;
	GtkWidget *button5;
		
	button1 = (GtkWidget *) gtk_builder_get_object(widgetstree, "button1");
	button3 = (GtkWidget *) gtk_builder_get_object(widgetstree, "button3");
	button4 = (GtkWidget *) gtk_builder_get_object(widgetstree, "button4");
	button5 = (GtkWidget *) gtk_builder_get_object(widgetstree, "button54");
		
	dialogusers = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialogusers");
	g_signal_connect  (button1, "clicked", G_CALLBACK (on_button1_clicked), (gpointer) dialogusers);	
	
	dialogrootpass = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialogrootpass");
	g_signal_connect  (button3, "clicked", G_CALLBACK (on_button3_clicked), (gpointer) dialogrootpass);	
	
	dialoguserpass = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialoguserpass");
	g_signal_connect  (button4, "clicked", G_CALLBACK (on_button4_clicked), (gpointer) dialoguserpass);	
	
	dialogbootloader = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialogbootloader");
	g_signal_connect  (button5, "clicked", G_CALLBACK (on_button5_clicked), (gpointer) dialogbootloader);	
	
	
	rootpassword = (GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword");
	username = (GtkWidget *) gtk_builder_get_object(widgetstree, "username");
	userpassword = (GtkWidget *) gtk_builder_get_object(widgetstree, "userpassword");
    
    rootpassword1 = (GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword1");
	userpassword1 = (GtkWidget *) gtk_builder_get_object(widgetstree, "userpassword1");
	
	if (gtk_entry_get_text_length (GTK_ENTRY(rootpassword)) == 0  ||  gtk_entry_get_text_length (GTK_ENTRY(username)) == 0
			|| (gtk_entry_get_text_length (GTK_ENTRY(userpassword)) == 0)) {
				gtk_widget_show(dialogusers);
	       }
	 else if (gtk_entry_get_text_length (GTK_ENTRY(rootpassword)) < 5  ||  gtk_entry_get_text_length (GTK_ENTRY(userpassword)) < 5) {
				gtk_widget_show(dialogusers);
	       }  
	else if  ( strcmp(gtk_entry_get_text(GTK_ENTRY(rootpassword)),gtk_entry_get_text (GTK_ENTRY(rootpassword1)))!=0 ) {
				gtk_widget_show(dialogrootpass);				
		   }
    else if  (strcmp(gtk_entry_get_text (GTK_ENTRY(userpassword)),gtk_entry_get_text (GTK_ENTRY(userpassword1)))!=0 ) {
				gtk_widget_show(dialoguserpass);				
		   }
    else if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "lilo")) 
		&& gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "grub"))){ 
		gtk_widget_show(dialogbootloader);}
	else {
	do_action(FALSE);
	}
}

void on_cancel_btn_clicked (GtkWidget *widget, gpointer user_data) {
	kill (pid, SIGTERM);
}


void on_exit (GtkWidget *widget, gpointer user_data) {
	if (pid != 0) {
		kill (pid, SIGTERM);
	}
	gtk_main_quit();
}

void initlocations() {
	GtkComboBox *listwidget;
	GtkTreeIter iter;
	GtkListStore *list;
	gchar **lines, *output, *device;
	gint i;
	gint status;
	
	listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, "copydevices");
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	copydevicescount = 0;
	g_spawn_command_line_sync("sli-location-detection.sh copy", &output, NULL, &status, NULL);
	if (status == 0) {
		lines = g_strsplit(output, "\n", 0);
		for (i=0; lines[i] != NULL && strlen(lines[i])>0; i++) {
			gtk_list_store_append(list, &iter);
			gtk_list_store_set(list, &iter, 0, lines[i], -1);
		}
		copydevicescount = i;
		g_strfreev(lines);
	}
	g_free(output);
	if (copydevicescount != 0){
		gtk_combo_box_set_active_iter(listwidget, &iter);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "copy_btn"), TRUE);
	}
	
	
	listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, "installdevices");
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	installdevicescount = 0;
	g_spawn_command_line_sync("sli-location-detection.sh install", &output, NULL, &status, NULL);
	if (status == 0) {
		lines = g_strsplit(output, "\n", 0);
		for (i=0; lines[i] != NULL && strlen(lines[i])>0; i++) {
			gtk_list_store_append(list, &iter);
			gtk_list_store_set(list, &iter, 0, lines[i], -1);
		}
		installdevicescount = i;
		g_strfreev(lines);
	}
	g_free(output);
	if (installdevicescount != 0){
		gtk_combo_box_set_active_iter(listwidget, &iter);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "install_btn"), TRUE);
	}

listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, "homedevices");
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	homedevicescount = 0;
	g_spawn_command_line_sync("sli-location-detection.sh home", &output, NULL, &status, NULL);
	if (status == 0) {
		lines = g_strsplit(output, "\n", 0);
		for (i=0; lines[i] != NULL && strlen(lines[i])>0; i++) {
			gtk_list_store_append(list, &iter);
			gtk_list_store_set(list, &iter, 0, lines[i], -1);
		}
		homedevicescount = i;
		g_strfreev(lines);
		gtk_list_store_prepend(list, &iter);
		gtk_list_store_set(list, &iter, 0, "", -1);
	}
	g_free(output);
	if (homedevicescount != 0){
		gtk_combo_box_set_active_iter(listwidget, &iter);
	}
}


void on_process_end (GPid thepid, gint status, gpointer data) {
	GtkWidget *dialog;

	g_source_remove(outputsource);
	g_io_channel_shutdown(stdoutioc, FALSE, NULL);
	g_io_channel_unref(stdoutioc);
	pid = 0;

	if (copydevicescount != 0){
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "copy_btn"), TRUE);
	}
	if (installdevicescount != 0){
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "install_btn"), TRUE);
		
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword1"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "username"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "userpassword"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "userpassword1"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "checkbutton1"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "checkbutton2"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "button2"), TRUE);
	}
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "cancel_btn"), FALSE);
	
	if (0 == status) {
		dialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialogfinished");
	} else {
		dialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialogerror");
	}
	
	gtk_widget_show(dialog);
}


gboolean progress_handler (GIOChannel *source, GIOCondition condition, gpointer data) {
	GtkTextView *view;
	GtkTextBuffer *buffer;
	GtkTextIter iter;
	gchar *line;
	
	g_io_channel_read_line(stdoutioc, &line, NULL, NULL, NULL);
	if (line != NULL) {
		view = (GtkTextView *) gtk_builder_get_object(widgetstree, "log");
		buffer = gtk_text_view_get_buffer(view);
		gtk_text_buffer_get_end_iter(buffer,&iter);
		gtk_text_buffer_insert(buffer, &iter, line, -1);
		g_free(line);
	}
	return TRUE;
}

void on_about_activate (GtkWidget *widget, gpointer user_data) {
	GtkWidget *aboutdialog;
	
	aboutdialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "aboutdialog");
	gtk_widget_show(aboutdialog);
}

void on_quit_activate (GtkWidget *widget, gpointer user_data) {
	if (pid != 0) {
		kill (pid, SIGTERM);
	}
	gtk_main_quit();
}


int main (int argc, char *argv[]) {
	GtkWindow *mainwindow;
	GtkWidget *notlivedialog;
	GtkWidget *menubar1;
	GtkWidget *about;
	GtkAboutDialog *aboutdialog;
	gchar *path;
		
	setlocale(LC_ALL, "");
	bindtextdomain(PROJECT_NAME, LOCALE_DIR);
	textdomain(PROJECT_NAME);
	
	path = g_strdup_printf("/sbin:/usr/sbin:/usr/local/sbin:%s", g_getenv("PATH"));
	g_setenv("PATH", path, TRUE);
	g_free(path);
	
	gtk_init(&argc, &argv);
	widgetstree = gtk_builder_new();
	gtk_builder_add_from_file(widgetstree, UI_FILE, NULL);
	gtk_builder_connect_signals(widgetstree, NULL);
	
	pid = 0;
	
	mainwindow = (GtkWindow *) gtk_builder_get_object(widgetstree, "mainwindow");
	gtk_window_set_icon_from_file(mainwindow, APP_ICON, NULL);
	aboutdialog = (GtkAboutDialog *) gtk_builder_get_object(widgetstree, "aboutdialog");
	
	gtk_about_dialog_set_version(aboutdialog, PROJECT_VERSION);
	gtk_widget_show((GtkWidget *) mainwindow);
	
	if (g_file_test("/live/media/boot/liveboot", G_FILE_TEST_EXISTS)) {
		initlocations();
	} else {
		notlivedialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "notlivedialog");
		gtk_widget_show(notlivedialog);
	}	
	gtk_main();
	return 0;
}
