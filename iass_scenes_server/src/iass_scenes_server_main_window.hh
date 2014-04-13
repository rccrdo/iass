/*
 * Copyright (c) 2006-2007 Riccardo Lucchese, riccardo.lucchese at gmail.com
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 *    1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 
 *    2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 
 *    3. This notice may not be removed or altered from any source
 *    distribution.
 */


#ifndef iass_scenes_server_main_window_hh
#define iass_scenes_server_main_window_hh

#include <iostream>
#include <sstream>
#include <string>
#include <iass/iass_scenes_server.hh>
#include <gtkmm.h>
#include <sigc++/sigc++.h>
#include <libglademm.h>

#define MAIN_WINDOW_UPDATE_STATS_GUI_REFRESH_TIME 1000 /* msec */

class iass_scenes_server_main_window {  
  
/* functions */
public:
	iass_scenes_server_main_window();
	~iass_scenes_server_main_window();

	Gtk::Window* get_window(void);

private:
	void on_scenes_stats_treeview_row_activated();
	void on_scenes_stats_refresh_tbutton_clicked();

	int update_stats_gui(void);

/* members */
private:  
	iass_scenes_server* scenes_server;

	ACE_Thread_Mutex stats_update_mutex;
	bool stats_update_loop_keep_running;



/* GTKmm stuff */
	Gtk::Window* window;
	Gtk::ToggleButton* scenes_stats_refresh_tbutton;

/* ******* scenes frame ********* */
	Gtk::Button* refresh_button;

	Gtk::ScrolledWindow* scene_stats_scrolled_window;
	/* movements tree colums model */
	class scenes_stats_colums_model : public Gtk::TreeModel::ColumnRecord {
	public:
		scenes_stats_colums_model() { add(scene_id),
				add(scene_name),  
				add(col_id)       ;}

		Gtk::TreeModelColumn<int> scene_id;
		Gtk::TreeModelColumn<Glib::ustring> scene_name;
		/* not shown */
		Gtk::TreeModelColumn<int> col_id;
	};

	scenes_stats_colums_model scenes_stats_treeview_columns;
	Gtk::TreeView* scenes_stats_treeview;
	Glib::RefPtr<Gtk::TreeStore> scenes_stats_treeview_ref_tree_model;

};

#endif /*iass_scenes_server_main_window_hh*/
