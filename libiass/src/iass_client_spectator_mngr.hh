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

#ifndef iass_client_spectator_mngr_hh
#define iass_client_spectator_mngr_hh

#include <iostream>
#include "iass/iass_configs.hh"
#include "iass_corba_orb_mngr.hh"
#include "idls/iass_scenes_server_idlC.h"
#include <orbsvcs/orbsvcs/CosNamingC.h>
#include <ace/Thread_Mutex.h>


class iass_client_spectator_mngr
{
/* functions */
public:
	/* get singletone instance */
	static iass_client_spectator_mngr* instance();

	bool subscribe_spectator(unsigned int scene_id, std::string tunnel_ior);
	void unsubscribe_spectator(void);

private:
	iass_client_spectator_mngr();
	iass_scenes_server_idl* find_scenes_server(void); 


/* members */
private:
	bool subscribed;
	ACE_Thread_Mutex subscribe_mutex;
	unsigned int subscribed_scene_id;
	iass_corba_raw_stream subscriber_ior;

	/* singletone stuff */
	static iass_client_spectator_mngr* st_instance;	// singletone instance
	static ACE_Thread_Mutex st_mutex;		// singletone obj creation mutex
};

#endif /*iass_client_spectator_mngr_hh*/



