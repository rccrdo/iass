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
#include "iass_designer_gui_mngr.hh"
#include <iass/iass_configs.hh>
#include <iass/iass_corba_orb_mngr.hh>
#include <orbsvcs/orbsvcs/CosNamingC.h>
#include <gtkmm.h>



int main (int argc, char* argv[])
{
	std::cout << "\n *  iass|intelligent autonomous systems studio\n"
		  << " |  iass|designer\n";

	/* initialize the Gtkmm lib */
	Gtk::Main gtkmmMain(argc, argv);

	/* init the configurations component */
	iass_configs::instance();

	/* get access to the orb */
	iass_corba_orb_mngr* orb_mngr = iass_corba_orb_mngr::instance();

	/* run orb */
	orb_mngr->run_separate_thread();
	if (!orb_mngr->orb_is_running()) {
		std::cout << "\n ! iass|designer, orb->run_separate_thread() didn't work as exspected, exiting ...\n";
		orb_mngr->shutdown_orb();
		return 1;
	}

	/* cativate poa */
	orb_mngr->activate_poa();
	if (!orb_mngr->poa_is_active()) {
		std::cout << "\n ! iass|designer, poa init didn't work as exspected, exiting ...\n";
		orb_mngr->shutdown_orb();
		return 1;
	}

	/* get the designer main window */
	Gtk::Window* win = iass_designer_gui_mngr::instance()->designer_main_window();
	if (!win) return 1;

	/* enter Gtkmm event loop */
	gtkmmMain.run(*win);

	std::cout << "\n ! iass|designer, exiting, we should delete the scene from the server or something\n";
	/* delete scene from designer */
	// options (kill, wait spectators)

	/* ask server to stop the incoming tunnel, th thread, tunnel memory ecc...*/


	std::cout << " ! iass|designer, what should we stop running and free here ?\n";   

	orb_mngr->shutdown_poa();
	orb_mngr->shutdown_orb();

	return 0;
}

