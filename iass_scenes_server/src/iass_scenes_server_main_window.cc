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

#include "iass_scenes_server_main_window.hh"


iass_scenes_server_main_window::iass_scenes_server_main_window() {
	window = NULL;
	scenes_stats_refresh_tbutton = NULL;
	scene_stats_scrolled_window = NULL;
	scenes_stats_treeview = NULL;
	stats_update_loop_keep_running = true;

	/* get a ptr to the simulations server */
	scenes_server = iass_scenes_server::instance();
	if (!scenes_server) {
		std::cout << "warning iass_scenes_server_main_window(constr.), scenes_server is a NULL ptr\n";
		return;
	}

	/* load gui from .glade file */
	Glib::RefPtr<Gnome::Glade::Xml> xml_ref;
	try {
		xml_ref = Gnome::Glade::Xml::create("gui/gui.glade");
	}
	catch (Gnome::Glade::XmlError e) {
		std::cout << "warning iass_scenes_server_main_window(constr.), couldn't find 'gui/gui.glade'; no gui available !\n"
			  << "        ! likely the file was moved/replaced\n";
		return;
	}

	/* get main window */
	xml_ref->get_widget("main_window", window);
	if (!window) return;
	window->move( 50, 50);

	/* get the scrolled window which contains the treeview widget listing scenes*/
	xml_ref->get_widget("stats_scrolled_window", scene_stats_scrolled_window);

	/* get tree view */
	xml_ref->get_widget("stats_treeview", scenes_stats_treeview);
	if (scenes_stats_treeview) {
		scenes_stats_treeview->set_reorderable();
		scenes_stats_treeview_ref_tree_model = Gtk::TreeStore::create(scenes_stats_treeview_columns);
		scenes_stats_treeview->set_model(scenes_stats_treeview_ref_tree_model);
		scenes_stats_treeview->append_column("id", scenes_stats_treeview_columns.scene_id);
		scenes_stats_treeview->append_column("name", scenes_stats_treeview_columns.scene_name);
//      scenes_stats_treeview->signal_row_activated().connect( sigc::mem_fun(*this, &iass_scenes_server_main_window::on_scenes_stats_treeview_row_activated) );
	}

	/* get the refresh stats toggle button */
	xml_ref->get_widget("scene_stats_refresh_tbutton", scenes_stats_refresh_tbutton);
	if (scenes_stats_refresh_tbutton) 
		scenes_stats_refresh_tbutton->signal_clicked().connect( sigc::mem_fun(*this, &iass_scenes_server_main_window::on_scenes_stats_refresh_tbutton_clicked) );

	// signal used to update statistics on gui
	Glib::signal_timeout().connect( sigc::mem_fun(*this, &iass_scenes_server_main_window::update_stats_gui), MAIN_WINDOW_UPDATE_STATS_GUI_REFRESH_TIME );

	/* show all */
	window->show_all_children();
};


iass_scenes_server_main_window::~iass_scenes_server_main_window() {}


Gtk::Window* iass_scenes_server_main_window::get_window() {
	return window;
}


int iass_scenes_server_main_window::update_stats_gui(void) {
	stats_update_mutex.acquire();
	iass_scene_stats** stats = scenes_server->get_scene_stats();
	if (!stats) {
		stats_update_mutex.release();
		/* retutning 0 here stops the update signal from glib's side */
		return stats_update_loop_keep_running;
	}

	scenes_stats_treeview_ref_tree_model->clear();
	Gtk::TreeModel::Row row;

	unsigned int stats_cur = 0;
	while (stats[stats_cur]!=NULL) {
		row = *(scenes_stats_treeview_ref_tree_model->append());
		row[scenes_stats_treeview_columns.scene_id] = stats[stats_cur]->id;
		row[scenes_stats_treeview_columns.scene_name] = stats[stats_cur]->name;
		stats_cur++;
	}

	while (Gtk::Main::events_pending()) Gtk::Main::iteration();

	free (stats);

	/* reselect previous row */
//  row = movs_treeview_ref_tree_model->children()[movements->getActualMovPos()]; 
//  if(row) movs_treeview->get_selection()->select(row);


	/* retutning 0 here stops the update signal from glib's side */
	stats_update_mutex.release();
	return stats_update_loop_keep_running;
}


void iass_scenes_server_main_window::on_scenes_stats_treeview_row_activated() {
	std::cout << "stats treeview row activated\n";
}


void iass_scenes_server_main_window::on_scenes_stats_refresh_tbutton_clicked() {
	update_stats_gui();
}


