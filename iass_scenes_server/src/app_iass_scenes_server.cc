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

#include <iostream>
#include <string>
#include <getopt.h>
#include "iass_scenes_server_main_window.hh"
#include <iass/iass_scenes_server.hh>
#include <iass/idls/iass_scenes_server_idlS.h>
#include <iass/iass_configs.hh>
#include <iass/iass_configs_defaults.hh>
#include <iass/iass_corba_orb_mngr.hh>
#include <orbsvcs/orbsvcs/CosNamingC.h>


void print_help() {
  
	std::cout << "  -h\t\tprint this help\n"
		  << "  -f\t\tforce bind to the NamingService (discards any registered iass|scenes_server)\n";
	exit(0);
}

int main (int argc, char** argv)
{
	std::cout << "\n *  iass|intelligent autonomous systems studio\n"
		  << " |  iass|scenes_server\n";


	bool force_rebind = false;

	/* get command line options */
	int opt;
	while((opt = getopt(argc, argv, "hfp")) != -1) {
		switch(opt) {
		case 'h': {
			print_help();
			break;
		}
		case 'f': {
			force_rebind = true; 
			break;
		}
		default : {
			std::cout << "\n ! unrecognzed options in the command line\n"; 
		}
		}
	}

	/* init the configurations component */
	iass_configs::instance();

	/* get access to the orb */
	iass_corba_orb_mngr* orb_mngr = iass_corba_orb_mngr::instance();

	/* activate poa */
	orb_mngr->activate_poa();
	if (!orb_mngr->poa_is_active()) {
		std::cout << "\n ! iass|scenes_server, poa init didn't work as exspected, exiting ...\n";
		orb_mngr->shutdown_orb();
		return 1;
	}

	/* create the scenes_server servant */
	iass_scenes_server_idl_var scene_server = iass_scenes_server::instance()->_this();

	/* register the scenes_server to the NamingService */
	try {
		std::cout << " * iass|scenes_server, looking for the NamingService...\n";
		CORBA::Object_ptr naming_context_object = orb_mngr->resolve_initial_references("NameService");
		CosNaming::NamingContext_var naming_context = CosNaming::NamingContext::_narrow (naming_context_object);

		/* get our namingservice name from iass_configs */
		std::string ns_name("");
		if (iass_configs::instance()->get_parameter(SCENES_SERVER_NAMING_SERVICE_ID, &ns_name) != 0 )
			ns_name = SCENES_SERVER_NAMING_SERVICE_DEF;
		std::cout << " * iass|scenes_server, registering to the naming service as \"" << ns_name << "\"\n";

		CosNaming::Name name(1);
		name.length(1);
		name[0].id = CORBA::string_dup (ns_name.c_str());
		/* ! bind() fails if the name is already in the naming service [use rebind() to force instead] */
		if (!force_rebind)  naming_context->bind(name, scene_server.in());
		else naming_context->rebind(name, scene_server.in());
	}
	catch (CORBA::Exception &) {
		std::cout << " ! iass|scenes_server, couldn't bind to the NamingService!\n"
			  << "   - if a scenes_server is already running use the -f option; otherwise\n"
			  << "     is likely no naming service at all is running in the network\n";
	}

	/* run the orb (using iass_corba_orb_mngr non blocking helper method */
	std::cout << " * iass|scenes_server, entering ORB loop... ";
	orb_mngr->run_separate_thread();
	if (!orb_mngr->orb_is_running()) {
		std::cout << "failed, exiting...\n";
		return 1;
	} else std::cout << "ok\n";


	/* initialize the Gtkmm lib */
	Gtk::Main gtkmmMain(argc, argv);

	/* start gui */
	iass_scenes_server_main_window* main_window = new iass_scenes_server_main_window();
	Gtk::Window* window = main_window->get_window();
	if (window) {
		/* enter Gtkmm event loop */
		gtkmmMain.run(*window);
	} else std::cout << " * iass|scenes_server, couldn't realize the main window, exiting...\n";

	/* shutdown the orb_mngr */
	orb_mngr->shutdown();

	std::cout << " * iass|scenes_server, what should we free and stop before exiting ?\n";

	return 0;
}

