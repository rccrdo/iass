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

#include "iass_designer_gui_mngr.hh"

iass_designer_gui_mngr* iass_designer_gui_mngr::st_instance = 0;
ACE_Thread_Mutex iass_designer_gui_mngr::st_mutex;

iass_designer_gui_mngr* iass_designer_gui_mngr::instance()
{
	/* queue concurrent threads */
	st_mutex.acquire();
	if (st_instance==0) st_instance = new iass_designer_gui_mngr();
	st_mutex.release();
	return st_instance;
}


iass_designer_gui_mngr::iass_designer_gui_mngr() {
	main_window = NULL;

	std::cout << " * initing component: iass_designer_gui_mngr\n";

	/* create the iass_designer_main_window obj,
	 * ! window won't be shown until we enter the gtk main loop */
	main_window = new iass_designer_main_window();
}


Gtk::Window* iass_designer_gui_mngr::designer_main_window(void) {
	if (main_window) return main_window->window();
	return NULL;
}


