/*
sli.c (Slackel-Live installer) from Slackel <http://www.slackel.gr>
Copyright (C) 2014-2025 Dimitris Tzemos <dijemos@gmail.com>
This program is free software: you can redistribute it and/or modify it under the terms 
of the GNU General Public License as published by the Free Software Foundation, 
either version 2 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty 
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License at <http://www.gnu.org/licenses/> for more details.
*/

#include <gtk/gtk.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <libintl.h>
#include <locale.h>
#include <sys/wait.h> 
#define _(String) gettext (String)
#define N_(String) String
#include "config.h"

#include "sli.h"

void ntp_sync_now(){
	GtkWidget *calendar,*spinbutton_hrs,*spinbutton_min,*spinbutton_sec;
	guint day, month, year, hour, min, sec, ntp;
	system("sed -i 's/#server 0.pool.ntp.org/server 0.pool.ntp.org/' /etc/ntp.conf");
	system("sed -i 's/#server 1.pool.ntp.org/server 1.pool.ntp.org/' /etc/ntp.conf");
	system("sed -i 's/#server 2.pool.ntp.org/server 2.pool.ntp.org/' /etc/ntp.conf");
	system("sed -i 's/#server 3.pool.ntp.org/server 3.pool.ntp.org/' /etc/ntp.conf");
	system("service stop ntpd");
	system("ntpd -gq");
	time_t current_time = time(NULL);
	struct  tm tm = *localtime(&current_time);
	calendar = (GtkWidget *) gtk_builder_get_object(widgetstree, "calendar");
	spinbutton_hrs = (GtkWidget *) gtk_builder_get_object(widgetstree, "spinbutton_hrs");
	spinbutton_min = (GtkWidget *) gtk_builder_get_object(widgetstree, "spinbutton_min");
	spinbutton_sec = (GtkWidget *) gtk_builder_get_object(widgetstree, "spinbutton_sec");
	gtk_calendar_select_day (GTK_CALENDAR (calendar),tm.tm_mday);
	gtk_calendar_select_month (GTK_CALENDAR (calendar),tm.tm_mon,tm.tm_year+1900);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON(spinbutton_hrs),tm.tm_hour);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON(spinbutton_min),tm.tm_min);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON(spinbutton_sec),tm.tm_sec);
	
}

int ntppresent(){
	if (g_file_test("/etc/rc.d/rc.ntpd", G_FILE_TEST_EXISTS)) {
		return 1;
	}
	else 
	{ 
		return 0;
	}
}

int ntpstate(){
	if (g_file_test("/etc/rc.d/rc.ntpd", G_FILE_TEST_IS_EXECUTABLE)) {
		return 1;
	}
	else 
	{ 
		return 0;
	}
}

void setntp(int ntp){
	if (ntp) {
			system("/usr/sbin/service restart ntpd");
		}
	else
		{ 
			system("/usr/sbin/service stop ntpd");
		}
}

int utcstate(){
	int UTC = 0;
	static const char filename[] = "/etc/hardwareclock";
	FILE *file = fopen ( filename, "r" );
	if ( file != NULL )
	{
		char line [128]; 
		while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
		{  size_t ln = strlen(line) - 1; if (*line && line[ln] == '\n') {line[ln] = '\0';}
			if (strcmp(line,"localtime")==0){
				UTC = 0;
				}
			else if (strcmp(line,"UTC")==0){
				UTC = 1;
				}
		}
		fclose ( file );
		return UTC;
   }
   else
   {
      perror ( filename );
   }
}

void get_current_zone(gchar dest[2][30])
{ 
	gchar *output,*kzone,**lines,**dummy;
	gint status, i;
	g_spawn_command_line_sync("sh get_current_zone.sh kzone", &output, NULL, &status, NULL);
	if (status == 0) {
	    dummy = g_strsplit(output, "\n", 0);
	    lines = g_strsplit(dummy[0], "/", 0);
	    for (i=0; lines[i] != NULL && strlen(lines[i])>0; i++) {
		strcpy(dest[0],lines[0]);
		strcpy(dest[1],lines[1]);
	    }
	    g_strfreev(lines);
	}
	g_free(output); 
	if (g_file_test("zone", G_FILE_TEST_EXISTS)) {
		system("rm zone");}
}

void settimezone(gchar *name, gchar *continent, gchar *location){
	gchar current_zone[2][30];
	get_current_zone(current_zone);
	if (symlink(name, "/etc/localtime")!=0){
	    unlink("/etc/localtime");
	    symlink(name, "/etc/localtime");
	}
}

void setutc(int utc) {
	char time[10];
	if (utc) strcpy(time,"UTC"); else strcpy(time,"localtime");
	FILE *fp;
	fp = fopen("/etc/hardwareclock", "w+");
	fprintf(fp,"# /etc/hardwareclock\n");
	fprintf(fp,"#\n");
	fprintf(fp,"# Tells how the hardware clock time is stored.\n");
	fprintf(fp,"# You should run (gtk)clocksetup or timeconfig to edit this file.\n\n");
	fprintf(fp,time);
	fprintf(fp,"\n");
	fclose(fp);
}	

void setkeymap(gchar *keymap) {
	FILE *fp;
	fp = fopen("set_keymap.sh", "w+");
	fprintf(fp,"#/bin/bash\n");
	fprintf(fp,"#\n");
	fprintf(fp,"/usr/sbin/keyboardsetup -k ");
	fprintf(fp,keymap);
	fprintf(fp,"\n");
	fclose(fp);
}	

void set_locale(gchar *locale) {
	FILE *fp;
	fp = fopen("set_locale.sh", "w+");
	fprintf(fp,"#/bin/bash\n");
	fprintf(fp,"#\n");
	fprintf(fp,"localesetup ");
	fprintf(fp,locale);
	fprintf(fp,"\n");
	fclose(fp);
}	

void create_date_time_zone(guint day,guint month,guint year,guint hour,guint min, guint sec)
{ 
	char s_day[4], s_month[4], s_year[8], s_hour[4], s_min[4], s_sec[4];

	sprintf(s_day, "%02d", day);
	sprintf(s_month, "%02d", month);
	sprintf(s_year, "%04d", year);
	sprintf(s_hour, "%02d", hour);
	sprintf(s_min, "%02d", min);
	sprintf(s_sec, "%02d", sec);
  
	FILE *f = fopen("set_date_time_zone.sh", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	int i;
	char *lines[14] = {"#!/bin/sh\n#\n", "date +%Y%m%d -s ",s_year,s_month,s_day," \n","date +T -s ",
					s_hour,":",s_min,":",s_sec," \n", "hwclock --systohc\n"
					};
	for (i=0 ; i<14; i++) {
			fprintf(f, "%s", lines[i]);
		}

	fclose(f);
}

void set_date_time_zone(){
	gchar *output, *do_nothing;
	gint status;
	g_spawn_command_line_sync("sh set_date_time_zone.sh do_nothing", &output, NULL, &status, NULL);
 
	g_free(output); 
	system("rm set_date_time_zone.sh");
}

void do_action (gboolean copy) {
	gchar *commandline, **command, *output, *home, *fstype, *usbfstype, *locale, *keyboard;
	GtkComboBox *listwidget;
	GtkTreeIter iter;
	GtkListStore *list;
	GtkWidget *label ;
	gboolean utc;
	gchar time[80];
	
	GtkWidget *calendar,*spinbutton_hrs,*spinbutton_min,*spinbutton_sec;
	guint day, month, year,hour,min,sec;
		
	char *rootpasswd,*usernam,*userpasswd, *installation_mode, *rootdirectory, *bootloader, *format_home;
	
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
	listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, "homedevices");
	gtk_combo_box_get_active_iter(listwidget, &iter);
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	gtk_tree_model_get((GtkTreeModel *) list, &iter, 0, &home, -1);
	if (strlen(home) == 0) {
	    g_free(home);
	    home = g_strdup(location);         
	}
	listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, "filesystem");
	gtk_combo_box_get_active_iter(listwidget, &iter);
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	gtk_tree_model_get((GtkTreeModel *) list, &iter, 0, &fstype, -1);
	if (strlen(fstype) == 0) {
	    g_free(fstype);
	    fstype = g_strdup("ext4");
	}
	listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, "usbfilesystem");
	gtk_combo_box_get_active_iter(listwidget, &iter);
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	gtk_tree_model_get((GtkTreeModel *) list, &iter, 0, &usbfstype, -1);
	if (strlen(usbfstype) == 0) {
	    g_free(usbfstype);
	    usbfstype = g_strdup("vfat");
	}

	label = (GtkWidget *) gtk_builder_get_object(widgetstree, "label20"); 
	locale = g_strdup(gtk_label_get_text(GTK_LABEL(label)));
	if (strlen(locale) == 0) {
		locale = g_strdup("");
	}
	
	label = (GtkWidget *) gtk_builder_get_object(widgetstree, "label21"); 
	keyboard = g_strdup(gtk_label_get_text(GTK_LABEL(label)));
	if (strlen(keyboard) == 0) {
		keyboard = g_strdup("");
	}
	
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
		}
	
	fullpercent = FALSE;
	pulsebar = TRUE;
	progressbar_handler_id = g_timeout_add(100, progressbar_handler, NULL);
	if (location != NULL) {
		g_free(location);
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
	
	if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "format_home"))) {
		format_home = g_strdup ("yes");
		}
	else {
	    format_home = g_strdup ("no");
	   } 		
	
	if (copy) {
		g_spawn_command_line_sync("du -s -m /live/media", &output, NULL, NULL, NULL);
		totalsize = g_ascii_strtoull(output, NULL, 10);
		commandline = g_strdup_printf("build-slackware-live.sh --usb /live/media %s\n", location, usbfstype);
	} else {
			if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "lilo"))) {
					g_spawn_command_line_sync("du -s -m /live/modules", &output, NULL, NULL, NULL);
					totalsize = g_ascii_strtoull(output, NULL, 10);
					commandline = g_strdup_printf("build-slackware-live.sh --install /live/%s %s -auto %s %s %s %s %s %s %s %s %s %s\n", rootdirectory, location, rootpasswd, usernam, userpasswd, installation_mode, bootloader, format_home, fstype, home, locale, keyboard);			
			} 
			else if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "grub"))) {
					g_spawn_command_line_sync("du -s -m /live/modules", &output, NULL, NULL, NULL);
					totalsize = g_ascii_strtoull(output, NULL, 10);
					commandline = g_strdup_printf("build-slackware-live.sh --install /live/%s %s -auto %s %s %s %s %s %s %s %s %s %s\n", rootdirectory, location, rootpasswd, usernam, userpasswd, installation_mode, bootloader, format_home, fstype, home, locale, keyboard);			
			} else { 
					g_spawn_command_line_sync("du -s -m /live/modules", &output, NULL, NULL, NULL);
					totalsize = g_ascii_strtoull(output, NULL, 10);
					commandline = g_strdup_printf("build-slackware-live.sh --install /live/%s %s -expert %s %s %s %s %s %s %s %s %s\n", rootdirectory, location, rootpasswd, usernam, userpasswd, installation_mode, "none", format_home, fstype, home, locale, keyboard);
			 }
	       }

	g_shell_parse_argv(commandline, NULL, &command, NULL);
	g_free(commandline);
	g_spawn_async(NULL, command, NULL, G_SPAWN_SEARCH_PATH|G_SPAWN_DO_NOT_REAP_CHILD, NULL, NULL, &pid, NULL);
	g_child_watch_add(pid, on_process_end, NULL);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "copy_btn"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "install_btn"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "cancel_btn"), TRUE);
	
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword1"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "username"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "userpassword"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "userpassword1"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "checkbutton_root"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "checkbutton_user"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "gparted"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "grub"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "lilo"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "format_home"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "filesystem"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "usbfilesystem"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "core"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "basic"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "full"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "copydevices"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "installdevices"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "homedevices"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "Language"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "keyboard"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "location"), FALSE);
	
	g_strfreev(command);
}

void on_checkbutton_root_toggled (GtkWidget *widget, gpointer user_data) {
    if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "checkbutton_root"))) {
	gtk_entry_set_visibility (GTK_ENTRY ((GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword")), TRUE);
	gtk_entry_set_visibility (GTK_ENTRY ((GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword1")), TRUE);
	} 
    else {
	gtk_entry_set_visibility (GTK_ENTRY ((GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword")), FALSE);
	gtk_entry_set_visibility (GTK_ENTRY ((GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword1")), FALSE);
	}
}

void on_checkbutton_user_toggled (GtkWidget *widget, gpointer user_data) {
    if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "checkbutton_user"))) {
	gtk_entry_set_visibility (GTK_ENTRY ((GtkWidget *) gtk_builder_get_object(widgetstree, "userpassword")), TRUE);
	gtk_entry_set_visibility (GTK_ENTRY ((GtkWidget *) gtk_builder_get_object(widgetstree, "userpassword1")), TRUE);
	} 
    else 
	{
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

void on_gparted_clicked (GtkWidget *widget, gpointer user_data) {
    if (g_file_test("/usr/bin/gparted", G_FILE_TEST_EXISTS)) {
	system("/usr/bin/gparted");
	clearlocations();
	initlocations();
	}
}

void on_Startup_Guide_activate (GtkWidget *widget, gpointer user_data) {
	if (g_file_test("/usr/doc/sli-1.2.6/slackel_startup_guide.pdf", G_FILE_TEST_EXISTS)) {
		gtk_show_uri_on_window(NULL,"file:///usr/doc/sli-1.2.6/slackel_startup_guide.pdf",gtk_get_current_event_time (), NULL);
	}
}

void on_button_sync_now_clicked(GtkWidget *widget, gpointer user_data) {
    ntp_sync_now();
}

void on_keyboard_button_ok_clicked(GtkWidget *widget, gpointer user_data) {
	 
	gchar *keymap;
	GtkWidget *label;
	GtkTreeIter iter;
	GtkTreeView *treeview;
	GtkListStore *list;
	GtkTreeModel *model;
	
	treeview = (GtkTreeView *) gtk_builder_get_object(widgetstree, "keymaplist");
	model = gtk_tree_view_get_model(treeview);
	list = GTK_LIST_STORE(model);
	GtkTreeSelection *selection;
	selection = gtk_tree_view_get_selection(treeview);

   if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
	  gtk_tree_model_get (model, &iter, 0, &keymap, -1);
	  label = (GtkWidget *) gtk_builder_get_object(widgetstree, "label21"); 
	  gtk_label_set_text(GTK_LABEL(label),keymap);
	  setkeymap(keymap);
	  system("sh set_keymap.sh");
	  system("rm set_keymap.sh");
	  }
}

char* concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2)+1;
    char *result = malloc(len1 + len2 );
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 );
    return result;
}

char * create_locationlist(char *name, char *locationlist)
{
FILE *f = fopen(name, "w");
if (f == NULL)
{
    printf("Error opening file!\n");
    exit(1);
}

int i;
char *lines[5] = {"#!/bin/sh\n#\n", "ls  /usr/share/zoneinfo/",
					locationlist,"/ > locationlist\n",
					"cat locationlist\n"
					};
  for (i=0 ; i<5; i++) {
	fprintf(f, "%s", lines[i]);
		}

fclose(f);
}

char * create_keymaplist(char *name, char *keymaplist)
{
FILE *f = fopen(name, "w");
if (f == NULL)
{
    printf("Error opening file!\n");
    exit(1);
}

 int i;
char *lines[5] = {"#!/bin/sh\n#\n", "ls  /usr/share/kbd/keymaps/i386/",
					keymaplist,"/ | sed 's/.map.gz//' > keymaplist\n",
					"cat keymaplist\n"
					};
  for (i=0 ; i<5; i++) {
			fprintf(f, "%s", lines[i]);
		}

fclose(f);
}

void system_locale(gchar *name)
{
FILE *f = fopen(name, "w");
if (f == NULL)
{
    printf("Error opening file!\n");
    exit(1);
}

int i;
gchar *lines[6] = {"#!/bin/sh\n#\n",	"echo `", "locale | grep LANG= | sed 's/LANG=//' "," | sed 's/UTF-8/utf8/' " ,"` > system_locale\n",
					"cat system_locale\n"
					};
  for (i=0 ; i<6; i++) {
			fprintf(f, "%s", lines[i]);
		}

fclose(f);
}

void get_system_locale(gchar *dest)
{ 
	gchar *output,*locale,**lines;
	gint status, i,count;
	count=0;
	system_locale("get_system_locale.sh");
	g_spawn_command_line_sync("sh get_system_locale.sh locale", &output, NULL, &status, NULL);
	if (status == 0) {
		lines = g_strsplit(output, "\n", 0);
		for (i=0; lines[i] != NULL && strlen(lines[i])>0; i++) {
			strcpy(dest,lines[0]);
	}
	count=i;
	g_strfreev(lines);
 }
	g_free(output); 
}

void get_keymap(gchar *dest)
{ 
	gchar *output,*kmap,**lines;
	gint status, i,count;
	count=0;
	memset(dest, '\0', sizeof(dest));
	g_spawn_command_line_sync("sh get_current_keymap.sh kmap", &output, NULL, &status, NULL);
	if (status == 0) {
		lines = g_strsplit(output, "\n", 0);
		for (i=0; lines[i] != NULL && strlen(lines[i])>0; i++) {
			strcpy(dest,lines[0]);
	}
	count=i;
	g_strfreev(lines);
 }
	g_free(output); 
}

void get_keybtype(gchar *dest)
{ 
	gchar *output,*keybtype,**lines;
	gint status, i;
	memset(dest, '\0', sizeof(dest));
	g_spawn_command_line_sync("sh get_current_keybtype.sh keybtype", &output, NULL, &status, NULL);
	if (status == 0) {
		lines = g_strsplit(output, "\n", 0);
		for (i=0; lines[i] != NULL && strlen(lines[i])>0; i++) {
			strcpy(dest,lines[0]);
	}
	g_strfreev(lines);
 }
	g_free(output); 
}

void on_keyboard_clicked (GtkWidget *widget, gpointer user_data) {
	GtkWidget *keyboardwindow;
	GtkTreeView *keybtypelistwidget, *keymaplistwidget;
	GtkListStore *keybtypeliststore, *keymapliststore;
	gchar *keytypelist, keymaplist,currentkeymap[30] ;
	GtkWidget *scrolledwindow1,*scrolledwindow2;
    
	GtkTreeIter iter;
	gchar currentkeybtype[30];
	gboolean valid;
	gint row_count = 0;
	//
	GtkTreePath *path;
	GtkTreeModel *model;
	GtkTreeViewColumn *column;
	// 
       
	keyboardwindow = (GtkWidget *) gtk_builder_get_object(widgetstree, "keyboardwindow");
	scrolledwindow1 = (GtkWidget *) gtk_builder_get_object(widgetstree, "scrolledwindow1");    
	keybtypelistwidget = (GtkTreeView *) gtk_builder_get_object(widgetstree, "keybtypelist");
	keybtypeliststore = (GtkListStore *) gtk_tree_view_get_model(keybtypelistwidget);
	
	scrolledwindow2 = (GtkWidget *) gtk_builder_get_object(widgetstree, "scrolledwindow2");    
	keymaplistwidget = (GtkTreeView *) gtk_builder_get_object(widgetstree, "keymaplist");
	keymapliststore = (GtkListStore *) gtk_tree_view_get_model(keymaplistwidget);
 	gtk_widget_show(keyboardwindow);
 	
 	get_keybtype(currentkeybtype);
 	model = gtk_tree_view_get_model(keybtypelistwidget);
 	valid = gtk_tree_model_get_iter_first (model, &iter);
	while (valid)
         {
           gchar *keybtype;
           gtk_tree_model_get (model, &iter,
                               0, &keybtype,
                               -1);
				path = gtk_tree_model_get_path (model, &iter);
				column=gtk_tree_view_get_column(keybtypelistwidget , 0);
           	if  (strncmp(currentkeybtype,keybtype,strlen(currentkeybtype))==0) { 
				path = gtk_tree_model_get_path (model, &iter);
				column=gtk_tree_view_get_column(keybtypelistwidget , 0);
				gtk_tree_view_set_cursor (keybtypelistwidget , path, column, FALSE);
				if (row_count>5){
				gtk_tree_view_scroll_to_cell(keybtypelistwidget , path,  column, TRUE,0.5,0.5);
				}
			}
			
           row_count ++;
           valid = gtk_tree_model_iter_next (model, &iter);
         }	
         
	model = gtk_tree_view_get_model(keymaplistwidget);
	get_keymap(currentkeymap);
	valid = gtk_tree_model_get_iter_first (model, &iter);
	while (valid)
         {
           gchar *keymap;
           gtk_tree_model_get (model, &iter,
                               0, &keymap,
                               -1);
				path = gtk_tree_model_get_path (model, &iter);
				column=gtk_tree_view_get_column(keymaplistwidget , 0);
           	if  (strncmp(currentkeymap,keymap,strlen(currentkeymap))==0) { 
				path = gtk_tree_model_get_path (model, &iter);
				column=gtk_tree_view_get_column(keymaplistwidget , 0);
				gtk_tree_view_set_cursor (keymaplistwidget , path, column, FALSE);
				if (row_count>5){
				gtk_tree_view_scroll_to_cell(keymaplistwidget , path,  column, TRUE,0.5,0.5);
				}
			}
			
           row_count ++;
           valid = gtk_tree_model_iter_next (model, &iter);
         }
	system("rm currentkeymap");          
}

void readfile(gchar registered_keymaplist[131][30]){
	static const char filename[] = "/usr/share/salixtools/keymaps";
	FILE *file = fopen ( filename, "r" );
	char dest1[133][30];
	int i,ii=0,i2=0;
	if ( file != NULL )
	{ 
		char line [ 128 ]; 
		while ( fgets ( line, sizeof line, file ) != NULL ) 
		{  size_t ln = strlen(line) - 1; for (i=0;i<ln;i++) if (line[i] == '|' || line[i] == '#' ||  line[i] == '\n') {line[i] = '\0';}
		   strcpy(dest1[ii],line);
			ii++;	
		}
		for (i=3;i<ii;i++) {	
				strcpy(registered_keymaplist[i-3],dest1[i]);
							}
		fclose ( file );
	}
	else
	{
	    perror ( filename ); 
	}
}


int is_registered_keymap(char *keymap) {
	char registered_keymaplist[131][30];
	int i,found=0;
	readfile(registered_keymaplist);
	for (i=0;i<131;i++) {
	    if (strcmp(keymap,registered_keymaplist[i])==0) 
		{
		found=1;return found;
		    }
	    }
	return found;
}
	
void on_keybtypelist_cursor_changed(GtkTreeView       *treeview, 
                                GtkTreePath       *path, 
                                GtkTreeViewColumn *column,
                                gpointer userdata) {										

	GtkTreeView *listwidget;
	GtkTreeView *keybtypelistwidget, *keymaplistwidget;
	GtkListStore *keybtypeliststore, *keymapliststore;
	gchar *keymaplist,currentkeymap[30];
	gchar **lines, *output,*locale;
	gint i;
	gint status;
	GtkWidget *scrolledwindow2;
    
	gboolean valid;
	gint row_count = 0;
    
	gchar *keymap;
	GtkTreeIter iter;
	GtkTreeSortable *sortable;
	GtkSortType	order;
	GtkTreeModel *model;
	model = gtk_tree_view_get_model(treeview);
	keymapliststore = GTK_LIST_STORE(model);
	GtkTreeSelection *selection;
	selection = gtk_tree_view_get_selection(treeview);
	   
   if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
		gtk_tree_model_get (model, &iter, 0, &keymap, -1);
		create_keymaplist("sli-keymap_list-detection.sh", keymap);
	   
		scrolledwindow2 = (GtkWidget *) gtk_builder_get_object(widgetstree, "scrolledwindow2");    
		keymaplistwidget = (GtkTreeView *) gtk_builder_get_object(widgetstree, "keymaplist");
		keymapliststore = (GtkListStore *) gtk_tree_view_get_model(keymaplistwidget);
	
		/*sortable = GTK_TREE_SORTABLE(keymapliststore);
		gtk_tree_sortable_set_sort_column_id(sortable, 0, GTK_SORT_ASCENDING);*/
	
		gtk_list_store_clear(keymapliststore) ;
		
		localecount = 0;
		g_spawn_command_line_sync("sh sli-keymap_list-detection.sh locale", &output, NULL, &status, NULL);
	 if (status == 0) {
		lines = g_strsplit(output, "\n", 0);
		for (i=0; lines[i] != NULL && strlen(lines[i])>0; i++) {
			if (is_registered_keymap(lines[i])){
				gtk_list_store_append(keymapliststore, &iter);
				gtk_list_store_set(keymapliststore, &iter, 0, lines[i], -1);
			}
		}
		localecount = i;
		g_strfreev(lines);
	    }
		g_free(output);
		system("rm sli-keymap_list-detection.sh");
		system("rm keymaplist");  
	    }
	 
	    //keymap focus
	
	    model = gtk_tree_view_get_model(keymaplistwidget); 
      
	    get_keymap(currentkeymap);
     
	    valid = gtk_tree_model_get_iter_first (model, &iter);
	    path = gtk_tree_model_get_path (model, &iter);
	    column=gtk_tree_view_get_column(keymaplistwidget , 0);
	    gtk_tree_view_set_cursor (keymaplistwidget , path, column, FALSE);
	    
	    while (valid)
	    {
		gchar *keymap;
		gtk_tree_model_get (model, &iter,
                               0, &keymap,
                               -1);
				path = gtk_tree_model_get_path (model, &iter);
				column=gtk_tree_view_get_column(keymaplistwidget , 0);
           	if  (strncmp(currentkeymap,keymap,strlen(currentkeymap))==0) { 
				path = gtk_tree_model_get_path (model, &iter);
				column=gtk_tree_view_get_column(keymaplistwidget , 0);
				gtk_tree_view_set_cursor (keymaplistwidget , path, column, FALSE);
				if (row_count>5){
				gtk_tree_view_scroll_to_cell(keymaplistwidget , path,  column, TRUE,0.5,0.5);
				}
			}
			
	    row_count ++;
	    valid = gtk_tree_model_iter_next (model, &iter);
	    }
          
	    system("rm currentkeymap");     
}

void on_continentlist_cursor_changed(GtkTreeView       *treeview, 
                                GtkTreePath       *path, 
                                GtkTreeViewColumn *column,
                                gpointer userdata) {										

	GtkTreeView *listwidget;
	GtkTreeView *continentlistwidget, *locationlistwidget;
	GtkListStore *continentliststore, *locationliststore;
	gchar *continentlist,current_zone[2][30],**dummy;
	gchar **lines, *output,*locale,*location;
	gint i;
	gint status;
	GtkWidget *scrolledwindow4;
    
	gboolean valid;
	gint row_count = 0;
    
	GtkTreeIter iter;
	GtkTreeSortable *sortable;
	GtkTreeModel *model;
	model = gtk_tree_view_get_model(treeview);
	locationliststore = GTK_LIST_STORE(model);
	GtkTreeSelection *selection;
	selection = gtk_tree_view_get_selection(treeview);
  	GtkWidget *label;
  	
   if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
		gtk_tree_model_get (model, &iter, 0, &location, -1);
		create_locationlist("sli_location_list_detection.sh", location);
		scrolledwindow4 = (GtkWidget *) gtk_builder_get_object(widgetstree, "scrolledwindow4");    
		locationlistwidget = (GtkTreeView *) gtk_builder_get_object(widgetstree, "locationlist");
		locationliststore = (GtkListStore *) gtk_tree_view_get_model(locationlistwidget);
	    	model = gtk_tree_view_get_model(locationlistwidget);
		/*sortable = GTK_TREE_SORTABLE(keymapliststore);
		gtk_tree_sortable_set_sort_column_id(sortable, 0, GTK_SORT_ASCENDING);*/
	
		gtk_list_store_clear(locationliststore) ;
		localecount = 0;
		g_spawn_command_line_sync("sh sli_location_list_detection.sh location", &output, NULL, &status, NULL);
	 if (status == 0) {
		lines = g_strsplit(output, "\n", 0);
		for (i=0; lines[i] != NULL && strlen(lines[i])>0; i++) {
			gtk_list_store_append(locationliststore, &iter);
			gtk_list_store_set(locationliststore, &iter, 0, lines[i], -1);
			}
			localecount = i;
			g_strfreev(lines);
		}
		g_free(output);
		system("rm sli_location_list_detection.sh");
		system("rm locationlist");  
	}
	 
		//location focus
        	get_current_zone(current_zone);
		/* label = (GtkWidget *) gtk_builder_get_object(widgetstree, "label25");
		
		if (strlen(gtk_label_get_text(GTK_LABEL(label)))==0){
			get_current_zone(current_zone);
			}
		else { 
			dummy=g_strsplit(gtk_label_get_text(GTK_LABEL(label)), "/", 0);
			strcpy(current_zone[0],dummy[0]);
			strcpy(current_zone[1],dummy[1]);
			} */
		
		valid = gtk_tree_model_get_iter_first (model, &iter);
		
	    if (valid){
		path = gtk_tree_model_get_path (model, &iter);
		column=gtk_tree_view_get_column(locationlistwidget , 0);
		gtk_tree_view_set_cursor (locationlistwidget , path, column, FALSE);
		
		
		while (valid)
		{
			gchar *klocation;
			gtk_tree_model_get (model, &iter,
                               0, &klocation,
                               -1);
				path = gtk_tree_model_get_path (model, &iter);
				column=gtk_tree_view_get_column(locationlistwidget , 0);
		    if  (strncmp(current_zone[1],klocation,strlen(current_zone[1]))==0) { 
				path = gtk_tree_model_get_path (model, &iter);
				column=gtk_tree_view_get_column(locationlistwidget , 0);
				gtk_tree_view_set_cursor (locationlistwidget , path, column, FALSE);
				if (row_count>5){
				gtk_tree_view_scroll_to_cell(locationlistwidget , path,  column, TRUE,0.5,0.5);
				}
			}
			
		row_count ++;
		valid = gtk_tree_model_iter_next (model, &iter);
		}
	}
}

void on_button4_clicked(GtkWidget *widget, gpointer user_data) {
		GtkWidget *calendar,*spinbutton_hrs,*spinbutton_min,*spinbutton_sec;
		guint day, month, year, hour, min, sec;
		gchar current_zone[2][30];
		gchar continent[80],location[80];
		GtkWidget *label,*label1,*label2, *utccheckbutton, *ntpcheckbutton;
		gchar zonelabel[80],zonelabel_text[80];
		gboolean utc;
		gchar **dummy;
	
		calendar = (GtkWidget *) gtk_builder_get_object(widgetstree, "calendar");
		spinbutton_hrs = (GtkWidget *) gtk_builder_get_object(widgetstree, "spinbutton_hrs");
		spinbutton_min = (GtkWidget *) gtk_builder_get_object(widgetstree, "spinbutton_min");
		spinbutton_sec = (GtkWidget *) gtk_builder_get_object(widgetstree, "spinbutton_sec");
	
		label = (GtkWidget *) gtk_builder_get_object(widgetstree, "zone");
		label1 = (GtkWidget *) gtk_builder_get_object(widgetstree, "label25");
		label2 = (GtkWidget *) gtk_builder_get_object(widgetstree, "timezonelabel");
		strcpy(zonelabel_text,gtk_label_get_text(GTK_LABEL(label2)));
		strcat(zonelabel_text," ");
		strcat(zonelabel_text,gtk_label_get_text(GTK_LABEL(label1)));	 
		gtk_label_set_text(GTK_LABEL(label),zonelabel_text);
		strcpy(zonelabel,"/usr/share/zoneinfo/");
		strcat(zonelabel,g_strdup(gtk_label_get_text(GTK_LABEL(label1))));
		dummy=g_strsplit(gtk_label_get_text(GTK_LABEL(label1)), "/", 0);
		strcpy(continent,dummy[0]);
		strcpy(location,dummy[1]);
		settimezone(zonelabel,continent,location);
		utccheckbutton = (GtkWidget *) gtk_builder_get_object(widgetstree, "utccheckbutton");
		!gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (utccheckbutton)) == utcstate();
		setutc(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (utccheckbutton)));
		ntpcheckbutton = (GtkWidget *) gtk_builder_get_object(widgetstree, "ntpcheckbutton");
		!gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (ntpcheckbutton)) == ntpstate();
		setntp(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (ntpcheckbutton)));
			
		if  (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (ntpcheckbutton)) == FALSE) {	
			gtk_calendar_get_date (GTK_CALENDAR (calendar),&year, &month, &day);
			hour = gtk_spin_button_get_value (GTK_SPIN_BUTTON(spinbutton_hrs));
			min = gtk_spin_button_get_value (GTK_SPIN_BUTTON(spinbutton_min));
			sec = gtk_spin_button_get_value (GTK_SPIN_BUTTON(spinbutton_sec));
			create_date_time_zone(day,month+1,year,hour,min,sec);
			set_date_time_zone();
			}
			if (g_file_test("zone", G_FILE_TEST_EXISTS)) {
		        system("rm zone");}
}

void on_location_button_ok_clicked(GtkWidget *widget, gpointer user_data) {
	gchar *continent,*location;
	GtkWidget *label;
	GtkTreeIter iter;
	GtkTreeView *treeview;
	GtkListStore *list;
	GtkTreeModel *model;
	gchar timezonelabel_text[80],timezone[80];
	
	treeview = (GtkTreeView *) gtk_builder_get_object(widgetstree, "continentlist");
	model = gtk_tree_view_get_model(treeview);
	list = GTK_LIST_STORE(model);
	GtkTreeSelection *selection;
	selection = gtk_tree_view_get_selection(treeview);

   if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
	gtk_tree_model_get (model, &iter, 0, &continent, -1);
	label = (GtkWidget *) gtk_builder_get_object(widgetstree, "label25"); 
	strcpy(timezonelabel_text,continent);
	strcat(timezonelabel_text,"/");
	}
	
	treeview = (GtkTreeView *) gtk_builder_get_object(widgetstree, "locationlist");
	model = gtk_tree_view_get_model(treeview);
	list = GTK_LIST_STORE(model);
	selection = gtk_tree_view_get_selection(treeview);

   if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
	  gtk_tree_model_get (model, &iter, 0, &location, -1);
	  label = (GtkWidget *) gtk_builder_get_object(widgetstree, "label25"); 	  
	  strcat(timezonelabel_text,location);
	  }
	   gtk_label_set_text(GTK_LABEL(label),timezonelabel_text);
	   
	    strcpy(timezone,"/usr/share/zoneinfo/");
		strcat(timezone,continent);
		strcat(timezone,"/");
		strcat(timezone,location);
		settimezone(timezone,continent,location);
		
 }

void on_button_ok_clicked(GtkWidget *widget, gpointer user_data) {
	 
	gchar *locale;
	GtkWidget *label;
	GtkTreeIter iter;
	GtkTreeView *treeview;
	GtkListStore *list;
	GtkTreeModel *model;
	
	treeview = (GtkTreeView *) gtk_builder_get_object(widgetstree, "clist");
	model = gtk_tree_view_get_model(treeview);
	list = GTK_LIST_STORE(model);
	GtkTreeSelection *selection;
	selection = gtk_tree_view_get_selection(treeview);

   if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
	  gtk_tree_model_get (model, &iter, 1, &locale, -1);
	  label = (GtkWidget *) gtk_builder_get_object(widgetstree, "label20"); 
	  gtk_label_set_text(GTK_LABEL(label),locale);}
	  set_locale(locale);
	  system("sh set_locale.sh");
	  system("rm set_locale.sh");
}

void on_timezonebutton_clicked(GtkWidget *widget, gpointer user_data) {
	gchar *continents[16] = {"Africa", "America", "Antarctica", "Asia", 
			"Atlantic", "Australia", "Europe", "Indian", 
			"Pacific", "US", "Mexico", "Chile",
			"Canada", "Brazil", "Arctic", "Etc"};
	GtkWidget *timezonewindow;
	GtkTreeView *listwidget,*locationlistwidget;
	GtkTreeIter iter;
	GtkListStore *list,*locationliststore;
	gchar *continentlist,*locationlist, current_zone[2][30];
	GtkWidget *scrolledwindow3,*scrolledwindow4;
	//GtkWidget *label;
	GtkTreeSortable *sortable;
	gboolean valid;
	gint i, row_count = 0;
	//
	GtkTreePath *path;
	GtkTreeModel *model;
	GtkTreeViewColumn *column;
	//  
	timezonewindow = (GtkWidget *) gtk_builder_get_object(widgetstree, "timezonewindow");
	scrolledwindow3 = (GtkWidget *) gtk_builder_get_object(widgetstree, "scrolledwindow3");    
	listwidget = (GtkTreeView *) gtk_builder_get_object(widgetstree, "continentlist");
	list = (GtkListStore *) gtk_tree_view_get_model(listwidget);
	model = gtk_tree_view_get_model(listwidget);
	
	scrolledwindow4 = (GtkWidget *) gtk_builder_get_object(widgetstree, "scrolledwindow4");    
	locationlistwidget = (GtkTreeView *) gtk_builder_get_object(widgetstree, "locationlist");
	locationliststore = (GtkListStore *) gtk_tree_view_get_model(locationlistwidget);

	gtk_list_store_clear(list);
	for (i=0; i<16; i++) {
			gtk_list_store_append(list, &iter);
			gtk_list_store_set(list, &iter, 0, continents[i], -1);
		}
	
	get_current_zone(current_zone);
	sortable = GTK_TREE_SORTABLE(list);
	gtk_tree_sortable_set_sort_column_id(sortable, 0, GTK_SORT_ASCENDING);
	valid = gtk_tree_model_get_iter_first (model, &iter);
   
       while (valid)
         {
           gchar *str_data0;
           gtk_tree_model_get (model, &iter,
                               0, &str_data0,
                               -1);
                             
				path = gtk_tree_model_get_path (model, &iter);
				column=gtk_tree_view_get_column(listwidget , 0);
           	if  (strncmp(current_zone[0],str_data0,strlen(current_zone[0]))==0) { 
				path = gtk_tree_model_get_path (model, &iter);
				column=gtk_tree_view_get_column(listwidget , 0);
				gtk_tree_view_set_cursor (listwidget , path, column, FALSE);
				if (row_count>5){
				gtk_tree_view_scroll_to_cell(listwidget , path,  column, TRUE,0.5,0.5);
				}
			}		
           row_count ++;
           valid = gtk_tree_model_iter_next (model, &iter);
         } 
         
	//location time
	model = gtk_tree_view_get_model(locationlistwidget);
	valid = gtk_tree_model_get_iter_first (model, &iter);
		 
	while (valid)
         {
			gchar *klocation;
			gtk_tree_model_get (model, &iter,
                               0, &klocation,
                               -1);
				path = gtk_tree_model_get_path (model, &iter);
				column=gtk_tree_view_get_column(locationlistwidget , 0);
           	if  (strncmp(current_zone[1],klocation,strlen(current_zone[1]))==0) { 
				path = gtk_tree_model_get_path (model, &iter);
				column=gtk_tree_view_get_column(locationlistwidget , 0);
				gtk_tree_view_set_cursor (locationlistwidget , path, column, FALSE);
				if (row_count>5){
				gtk_tree_view_scroll_to_cell(locationlistwidget , path,  column, TRUE,0.5,0.5);
				}
			}
			
			row_count ++;
			valid = gtk_tree_model_iter_next (model, &iter);
         }
         
			gtk_widget_show(timezonewindow);

}
 
void on_clist_cursor_changed(GtkTreeView       *treeview, 
                                GtkTreePath       *path, 
                                GtkTreeViewColumn *column,
                                gpointer userdata) {

   gchar *language, *locale;
   GtkWidget *label;
   GtkTreeIter iter;
   GtkListStore *list;
   GtkTreeModel *model;
   model = gtk_tree_view_get_model(treeview);
   list = GTK_LIST_STORE(model);
   GtkTreeSelection *selection;
   selection = gtk_tree_view_get_selection(treeview);
  
   /* if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
	  gtk_tree_model_get (model, &iter, 1, &locale, -1);
	  label = (GtkWidget *) gtk_builder_get_object(widgetstree, "label20"); 
	  }  */
}

void on_clist_row_activated(GtkTreeView       *treeview, 
                                GtkTreePath       *path, 
                                GtkTreeViewColumn *column,
                                gpointer userdata) {

	gchar *language, *locale;
	GtkWidget *label;
	GtkTreeIter iter;
	GtkListStore *list;
	GtkTreeModel *model = gtk_tree_view_get_model(treeview);
	list = GTK_LIST_STORE(model);
	GtkTreeSelection *selection;
	selection = gtk_tree_view_get_selection(treeview);
	
 if (gtk_tree_model_get_iter(model, &iter, path)) {
	gtk_tree_model_get (model, &iter, 0, &language, 1, &locale, -1);
    label = (GtkWidget *) gtk_builder_get_object(widgetstree, "label20"); 
    gtk_label_set_text(GTK_LABEL(label),locale);
    // g_free (language);
    // g_free(locale);
   }
}

void on_ntpcheckbutton_toggled (GtkWidget *widget, gpointer user_data) {
 	if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "ntpcheckbutton"))) {
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "button_sync_now"), FALSE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "datelabel"), FALSE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "timelabel"), FALSE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "calendar"), FALSE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "spinbutton_hrs"), FALSE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "spinbutton_min"), FALSE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "spinbutton_sec"), FALSE);

	} else {
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "button_sync_now"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "datelabel"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "timelabel"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "calendar"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "spinbutton_hrs"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "spinbutton_min"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "spinbutton_sec"), TRUE);
		}
}

void on_location_clicked (GtkWidget *widget, gpointer user_data) {
		GtkWidget *clockwindow,*calendar,*spinbutton_hrs,*spinbutton_min,*spinbutton_sec,*ntpcheckbutton,*utccheckbutton,*button_sync_now;
		
		guint day, month, year, hour, min, sec, ntp;
		GtkWidget *label;
		gchar timezonelabel_text[80];
		gchar current_zone[2][30], **dummy;
		
		get_current_zone(current_zone);
		strcpy(timezonelabel_text,current_zone[0]);
		strcat(timezonelabel_text,"/");
		strcat(timezonelabel_text,current_zone[1]);
		label = (GtkWidget *) gtk_builder_get_object(widgetstree, "label25"); 	  
		gtk_label_set_text(GTK_LABEL(label),timezonelabel_text);
	   
		time_t current_time = time(NULL);
		struct  tm tm = *localtime(&current_time);

		//	printf("System Date is: %02d/%02d/%04d\n",tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
		//	printf("System Time is: %02d:%02d:%02d\n",tm.tm_hour, tm.tm_min, tm.tm_sec);
		clockwindow = (GtkWidget *) gtk_builder_get_object(widgetstree, "clockwindow");
		calendar = (GtkWidget *) gtk_builder_get_object(widgetstree, "calendar");
		spinbutton_hrs = (GtkWidget *) gtk_builder_get_object(widgetstree, "spinbutton_hrs");
		spinbutton_min = (GtkWidget *) gtk_builder_get_object(widgetstree, "spinbutton_min");
		spinbutton_sec = (GtkWidget *) gtk_builder_get_object(widgetstree, "spinbutton_sec");
		gtk_widget_show(clockwindow);
		gtk_calendar_select_day (GTK_CALENDAR (calendar),tm.tm_mday);
		gtk_calendar_select_month (GTK_CALENDAR (calendar),tm.tm_mon,tm.tm_year+1900);
		//	printf("%d/%d/%d\n", month + 1, year, day);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON(spinbutton_hrs),tm.tm_hour);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON(spinbutton_min),tm.tm_min);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON(spinbutton_sec),tm.tm_sec);
		gtk_calendar_get_date (GTK_CALENDAR (calendar),&year, &month, &day);
		//printf("day=%02d month=%02d year=%04d\n", day, month+1, year);
		utccheckbutton = (GtkWidget *) gtk_builder_get_object(widgetstree, "utccheckbutton");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (utccheckbutton),utcstate());
		
		button_sync_now = (GtkWidget *) gtk_builder_get_object(widgetstree, "button_sync_now");
		ntpcheckbutton = (GtkWidget *) gtk_builder_get_object(widgetstree, "ntpcheckbutton");
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "ntpcheckbutton"),ntppresent());
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "button_sync_now"), ! ntppresent());
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (ntpcheckbutton), ntpstate());
		on_ntpcheckbutton_toggled(ntpcheckbutton,user_data) ;
}

void on_Language_clicked (GtkWidget *widget, gpointer user_data) {
	GtkWidget *localewindow;
	GtkTreeView *listwidget;
	GtkTreeIter iter;
	GtkListStore *list;
	gchar *clist;
	gchar current_locale[10];
	GtkWidget *scrolledwindow;
	GtkWidget *label;
	GtkTreeSortable *sortable;
	gboolean valid;
	gint row_count = 0;
	//
	GtkTreePath *path;
	GtkTreeModel *model;
	GtkTreeViewColumn *column;
	// 
	label = (GtkWidget *) gtk_builder_get_object(widgetstree, "label20");
	localewindow = (GtkWidget *) gtk_builder_get_object(widgetstree, "localewindow");
	scrolledwindow = (GtkWidget *) gtk_builder_get_object(widgetstree, "scrolledwindow");    
	listwidget = (GtkTreeView *) gtk_builder_get_object(widgetstree, "clist");
	list = (GtkListStore *) gtk_tree_view_get_model(listwidget);
	model = gtk_tree_view_get_model(listwidget);
	
	sortable = GTK_TREE_SORTABLE(list);
	gtk_tree_sortable_set_sort_column_id(sortable, 0, GTK_SORT_ASCENDING);
	
	gtk_widget_show(localewindow);
	if (strlen(gtk_label_get_text(GTK_LABEL(label)))==0){
		get_system_locale(current_locale);		
		 if (g_file_test("get_system_locale.sh", G_FILE_TEST_EXISTS)) {
	        system("rm get_system_locale.sh");
	      }	
	     if (g_file_test("system_locale", G_FILE_TEST_EXISTS)) {
	        system("rm system_locale");
	      }		
		}
	else {
		strcpy(current_locale,gtk_label_get_text(GTK_LABEL(label)));
		}

	valid = gtk_tree_model_get_iter_first (model, &iter);
	path = gtk_tree_model_get_path (model, &iter);
	column=gtk_tree_view_get_column(listwidget , 0);
	gtk_tree_view_set_cursor (listwidget , path, column, FALSE);
	while (valid)
	    {
		gchar *str_data0;
		gchar   *str_data1;
		gtk_tree_model_get (model, &iter,
                               0, &str_data0,
                               1, &str_data1,
                               -1);
		path = gtk_tree_model_get_path (model, &iter);
		column=gtk_tree_view_get_column(listwidget , 0);
		if  (strncmp(current_locale,str_data1,10)==0) { 
			path = gtk_tree_model_get_path (model, &iter);
			column=gtk_tree_view_get_column(listwidget , 0);
			gtk_tree_view_set_cursor (listwidget , path, column, FALSE);
			if (row_count>5){
				gtk_tree_view_scroll_to_cell(listwidget , path,  column, TRUE,0.5,0.5);
				}
			}		
		row_count ++;
		valid = gtk_tree_model_iter_next (model, &iter);
      }         
}

void init_locale_list()
{
	GtkTreeView *listwidget;
	GtkTreeIter iter;
	GtkListStore *list;
	gchar *clist ;
	gchar **lines, **lines_split, *output,*locale;
	
	gchar locale_name[10];
	
	GtkTreePath *path;
	GtkTreeModel *model;
	GtkTreeViewColumn *column;
	gint i;
	gint status;
	listwidget = (GtkTreeView *) gtk_builder_get_object(widgetstree, "clist");
	list = (GtkListStore *) gtk_tree_view_get_model(listwidget);
	model = gtk_tree_view_get_model(listwidget);
	localecount = 0;
    
	get_system_locale(locale_name);
    
	g_spawn_command_line_sync("sli-locale_list-detection.sh locale", &output, NULL, &status, NULL);
	if (status == 0) {
		lines = g_strsplit(output, "\n", 0);
		for (i=0; lines[i] != NULL && strlen(lines[i])>0; i++) {
			gtk_list_store_append(list, &iter);
			lines_split=g_strsplit(lines[i],",",0);
			gtk_list_store_set(list, &iter, 0, g_strdup(lines_split[1]), -1);
			gtk_list_store_set(list, &iter, 1, g_strdup(lines_split[0]), -1);			     
		}
		localecount = i;	
		g_strfreev(lines);
	}
	g_free(output);
	if (g_file_test("locale_file", G_FILE_TEST_EXISTS)) {
	    system("rm locale_file");
	}
	if (g_file_test("system_locale", G_FILE_TEST_EXISTS)) {
	    system("rm system_locale");
	}
}



void on_copy_btn_clicked (GtkWidget *widget, gpointer user_data) {
	do_action(TRUE);
}

void on_install_btn_clicked (GtkWidget *widget, gpointer user_data) {
	GtkWidget *dialog;
	GtkWidget *rootpassword;
	GtkWidget *username;
	GtkWidget *userpassword;
	GtkWidget *rootpassword1;
	GtkWidget *userpassword1;
	
	gchar *fstype, *usbfstype;
	GtkComboBox *listwidget;
	GtkTreeIter iter;
	GtkListStore *list;
		
	rootpassword = (GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword");
	username = (GtkWidget *) gtk_builder_get_object(widgetstree, "username");
	userpassword = (GtkWidget *) gtk_builder_get_object(widgetstree, "userpassword");
    
	rootpassword1 = (GtkWidget *) gtk_builder_get_object(widgetstree, "rootpassword1");
	userpassword1 = (GtkWidget *) gtk_builder_get_object(widgetstree, "userpassword1");
	
	listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, "filesystem");
	gtk_combo_box_get_active_iter(listwidget, &iter);
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	gtk_tree_model_get((GtkTreeModel *) list, &iter, 0, &fstype, -1);
	
	listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, "usbfilesystem");
	gtk_combo_box_get_active_iter(listwidget, &iter);
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	gtk_tree_model_get((GtkTreeModel *) list, &iter, 0, &usbfstype, -1);
	
	if (gtk_entry_get_text_length (GTK_ENTRY(rootpassword)) == 0  ||  gtk_entry_get_text_length (GTK_ENTRY(username)) == 0
			|| (gtk_entry_get_text_length (GTK_ENTRY(userpassword)) == 0)) {
				dialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialogusers");
				gtk_widget_show(dialog);
	       }
	else if (gtk_entry_get_text_length (GTK_ENTRY(rootpassword)) < 5  ||  gtk_entry_get_text_length (GTK_ENTRY(userpassword)) < 5) {
		 	    dialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialogusers");
				gtk_widget_show(dialog);
	       }  
	else if  ( strcmp(gtk_entry_get_text(GTK_ENTRY(rootpassword)),gtk_entry_get_text (GTK_ENTRY(rootpassword1)))!=0 ) {
			    dialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialogrootpass");
				gtk_widget_show(dialog);				
		   }
	else if  (strcmp(gtk_entry_get_text (GTK_ENTRY(userpassword)),gtk_entry_get_text (GTK_ENTRY(userpassword1)))!=0 ) {
				dialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialoguserpass");
				gtk_widget_show(dialog);				
		   }
	else if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "lilo")) 
		&& gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "grub"))){ 
			dialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialogbootloader");
			gtk_widget_show(dialog);}
	else if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "grub")) && (strcmp(fstype,"xfs") == 0)){
		dialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialog_grub_xfs");
		gtk_widget_show(dialog);}	
	else {
	do_action(FALSE);
	}
}

void on_cancel_btn_clicked (GtkWidget *widget, gpointer user_data) {
	kill (pid, SIGTERM);
}


void on_exitp (GtkWidget *widget, gpointer user_data) {
	if (pid != 0) {
	    kill (pid, SIGTERM);
	}
	gtk_main_quit();
}

gboolean progressbar_handler(gpointer data) {
	GtkProgressBar *progressbar;
	gchar *output;
	guint64 installsize;
	gdouble progressfraction;
	gchar *s_progressfraction;
	
	progressbar = (GtkProgressBar *) gtk_builder_get_object(widgetstree,"progressbar");
	
	if (pulsebar && ! fullpercent && g_file_test("/mnt/install",  G_FILE_TEST_IS_DIR)) {
		pulsebar = FALSE;
		g_source_remove(progressbar_handler_id);
		progressbar_handler_id = g_timeout_add(5000, progressbar_handler, NULL);
	}
	
	if (pulsebar) {
	    gtk_progress_bar_pulse(progressbar);
	} else {
		g_spawn_command_line_sync("du -s -m /mnt/install", &output, NULL, NULL, NULL);
		installsize = g_ascii_strtoull(output, NULL, 10);
		progressfraction = installsize * 100 / totalsize;
		if (progressfraction >= 100) {
			gtk_progress_bar_set_text(progressbar, "100 %");
			gtk_progress_bar_set_fraction(progressbar, 1.0);
			fullpercent = TRUE;
			pulsebar = TRUE;
			g_source_remove(progressbar_handler_id);
			progressbar_handler_id = g_timeout_add(100, progressbar_handler, NULL);
		} else {
			gtk_progress_bar_set_fraction(progressbar, progressfraction / 100);
			s_progressfraction = g_strdup_printf("%2.0f %c", progressfraction, '%');
			gtk_progress_bar_set_text(progressbar, s_progressfraction);
			g_free(s_progressfraction);
		}
	}
	return TRUE;
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
	GtkProgressBar *progressbar;

	pid = 0;
	g_free(location);
	location = NULL;
	g_source_remove(progressbar_handler_id);
	
	progressbar = (GtkProgressBar *) gtk_builder_get_object(widgetstree,"progressbar");
	gtk_progress_bar_set_fraction(progressbar, 0);
	gtk_progress_bar_set_text(progressbar, "");

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
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "checkbutton_root"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "checkbutton_user"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "gparted"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "grub"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "lilo"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "format_home"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "filesystem"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "usbfilesystem"), TRUE);		
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "core"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "basic"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "full"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "copydevices"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "installdevices"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "homedevices"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "Language"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "keyboard"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "location"), TRUE);
	}
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "cancel_btn"), FALSE);
	
	if (0 == status) {
		dialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialogfinished");
	} else {
		dialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialogerror");
	}
	
	gtk_widget_show(dialog);
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
	location = NULL;
		
	mainwindow = (GtkWindow *) gtk_builder_get_object(widgetstree, "mainwindow");
	gtk_window_set_icon_from_file(mainwindow, APP_ICON, NULL);
	aboutdialog = (GtkAboutDialog *) gtk_builder_get_object(widgetstree, "aboutdialog");
	
	gtk_about_dialog_set_version(aboutdialog, PROJECT_VERSION);
	gtk_widget_show((GtkWidget *) mainwindow);
	
	if (g_file_test("/live/media/boot/liveboot", G_FILE_TEST_EXISTS)) {
		initlocations();
		init_locale_list();
	} else {
		notlivedialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "notlivedialog");
		gtk_widget_show(notlivedialog);
	}	
	gtk_main();
	return 0;
}
