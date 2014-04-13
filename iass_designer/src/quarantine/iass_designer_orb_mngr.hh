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

#ifndef iass_designer_orb_mngr_hh
#define iass_designer_orb_mngr_hh

#include <iostream>
#include <string>
#include <orbsvcs/orbsvcs/CosNamingC.h>
#include <ace/Thread_Mutex.h>
#include <iass/idls/iass_scene_idlC.h>
#include <iass/idls/iass_simulations_server_idlC.h>
#include <iass/iass_configs.hh>
#include <iass/iass_configs_defaults.hh>

class iass_designer_orb_mngr
{
/* functions */
public:
	/* get singletone instance */
	static iass_designer_orb_mngr* instance();

//  iass_scene_idl* request_new_scene(void);
	iass_scene_idl* current_scene(void);

	void shutdown(void);



private:
	iass_designer_orb_mngr();


/* members */
private:
	bool inited;

	iass_scene_idl* remote_scene;

	CORBA::ORB_var orb;
	CosNaming::NamingContext_var naming_context;
	iass_simulations_server_idl_var remote_simulations_server;

	/* singletone stuff */
	static iass_designer_orb_mngr* st_instance;	// singletone instance
	static ACE_Thread_Mutex st_mutex;		// singletone obj creation mutex
};

#endif /*iass_designer_orb_mngr_hh*/

