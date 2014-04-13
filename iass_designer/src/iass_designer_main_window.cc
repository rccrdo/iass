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

#include "iass_designer_main_window.hh"


iass_designer_main_window::iass_designer_main_window() {
	remote_scene = NULL;
	main_window = NULL;
	create_box = NULL;
	create_sphere = NULL;

	/* load gui from .glade file */
	Glib::RefPtr<Gnome::Glade::Xml> xml_ref;
	try {
		xml_ref = Gnome::Glade::Xml::create("gui/gui.glade");
	}
	catch (Gnome::Glade::XmlError e) {
		std::cout << "warning iass_designer_main_window(constr.), couldn't find 'gui/gui.glade'; no gui available !\n"
			  << "        ! likely the file was moved/replaced\n";
		return;
	}

	/* get main window */
	xml_ref->get_widget("main_window", main_window);
	if (!main_window) return;

	/* get set_scene_name button and the scene_name entry */
	xml_ref->get_widget("scene_set_name", set_scene_name_button);
	if (set_scene_name_button) {
		set_scene_name_button->signal_clicked().connect( sigc::mem_fun(*this, &iass_designer_main_window::scene_on_set_name_clicked) );
	}
	xml_ref->get_widget("scene_name_entry", scene_name_entry);

	/* get create - buttons */
	xml_ref->get_widget("create_box", create_box);
	if (create_box) {
		create_box->signal_clicked().connect( sigc::mem_fun(*this, &iass_designer_main_window::create_on_box_clicked) );
	}
	xml_ref->get_widget("create_sphere", create_sphere);
	if (create_sphere) {
		create_sphere->signal_clicked().connect( sigc::mem_fun(*this, &iass_designer_main_window::create_on_sphere_clicked) );

	}


	/* request for a new remote scene on the scenes_server */
	remote_scene = iass_designer_server_link_mngr::instance()->new_scene();

	/* update interface */
  

	/* show all */
	main_window->show_all_children();

};


iass_designer_main_window::~iass_designer_main_window() {}


Gtk::Window* iass_designer_main_window::window() {
	return main_window;
}


void iass_designer_main_window::scene_on_set_name_clicked() {
	if (!remote_scene) {
		std::cout << "warning iass_designer_main_window::scene_on_set_name_clicked(), remote_scene is a NULL ptr\n";
		return;
	}

	std::string name = scene_name_entry->get_text();
	if (name.size()==0) return;
	iass_cstring* _name = new iass_cstring(name.c_str(), name.size());

	iass_ipcblock* data = iass_ipc__scene_mngr__set_name(_name);

	temp[0] = data;
	remote_scene->push(temp, 1);
}


void iass_designer_main_window::create_on_box_clicked() {
	std::cout << "create_on_box_clicked()\n";

	if (!remote_scene) {
		std::cout << "warning iass_designer_main_window::create_on_box_clicked(), remote_scene is a NULL ptr\n";
		return;
	}

	char buffer[1000];
	iass_serialize_uint16(buffer, strlen(IASS_ENTITY_REF_BASICTYPE_PATH));
	iass_serialize_uint16(buffer + 2, strlen(IASS_ENTITY_REF_BASICTYPE_NODE_TYPE_PHYSICS));
	iass_serialize_uint16(buffer + 4, strlen(IASS_ENTITY_REF_BASICTYPE_NODE_ID_PHY_BOX));

	memcpy (buffer + 6, IASS_ENTITY_REF_BASICTYPE_PATH, strlen(IASS_ENTITY_REF_BASICTYPE_PATH));
	memcpy (buffer + 6 + strlen(IASS_ENTITY_REF_BASICTYPE_PATH), IASS_ENTITY_REF_BASICTYPE_NODE_TYPE_PHYSICS, strlen(IASS_ENTITY_REF_BASICTYPE_NODE_TYPE_PHYSICS));
	memcpy (buffer + 6 + strlen(IASS_ENTITY_REF_BASICTYPE_PATH) + strlen(IASS_ENTITY_REF_BASICTYPE_NODE_TYPE_PHYSICS), IASS_ENTITY_REF_BASICTYPE_NODE_ID_PHY_BOX, strlen(IASS_ENTITY_REF_BASICTYPE_NODE_ID_PHY_BOX));

	unsigned int content_len = 6 + strlen(IASS_ENTITY_REF_BASICTYPE_PATH)
		+ strlen(IASS_ENTITY_REF_BASICTYPE_NODE_TYPE_PHYSICS)
		+ strlen(IASS_ENTITY_REF_BASICTYPE_NODE_ID_PHY_BOX);

	iass_entity_ref ref(buffer, content_len);
	iass_ipcblock* data = iass_ipc__scene_mngr_server_side__realize_physic_brick(ref);
	temp[0] = data;
	remote_scene->push(temp, 1);
}


void iass_designer_main_window::create_on_sphere_clicked() {
	std::cout << "create_on_sphere_clicked()\n";

	if (!remote_scene) {
		std::cout << "warning iass_designer_main_window::create_on_sphere_clicked(), remote_scene is a NULL ptr\n";
		return;
	}

	iass_cstring* name = new iass_cstring("sphere", 6);
	iass_ipcblock* data = iass_ipc__scene_mngr__set_name(name);
	temp[0] = data;
	remote_scene->push(temp, 1);
}



