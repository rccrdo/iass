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

#ifndef iass_designer_server_link_mngr_hh
#define iass_designer_server_link_mngr_hh

#include <iostream>
#include <iass/iass_configs.hh>
#include <iass/iass_corba_orb_mngr.hh>
#include <iass/iass_ipctunnel_client.hh>
#include <iass/idls/iass_tunnel_idlC.h>
#include <iass/idls/iass_scenes_server_idlC.h>
#include <orbsvcs/orbsvcs/CosNamingC.h>
#include <ace/Thread_Mutex.h>
#include <ace/Thread_Manager.h>

/** class iass_designer_server_link_mngr
 *  \brief Singleton helper class used to manage the link with the iass|scenes_server component
 */
class iass_designer_server_link_mngr
{
/* functions */
public:
	/**
	 *  Get the singleton instance
	 */
	static iass_designer_server_link_mngr* instance();

	/**
	 *  \brief Request a new scene on the iass|scenes_server
	 *  \return A ptr to a tunnel client that can be used to send data to the scene_mngr on the server side
	 *  
	 *  Return value can be NULL, when the iass|scenes_server cannot be found in the network
	 */
	iass_ipctunnel_client* new_scene(void);


private:
	/**
	 *  Singleton private constructor
	 */
	iass_designer_server_link_mngr();


	/**
	 *  Helper method used to find the iass_scene_server obj (remote implementor)
	 *
	 *  Implemented using the NamingService facilities. The name used for the regstered service is
	 *  looked up fro the iass_configs singleton class.
	 */
	iass_scenes_server_idl* find_scenes_server(void); 



/* members */
private:
	/**
	 *  Mutex queueing access to the members of this class
	 */
	ACE_Thread_Mutex server_link_mutex;

	/**
	 *  Ptr saving the current_scene_tunnel_client
	 */
	iass_ipctunnel_client* current_scene_tunnel;


	/*  singletone stuff  */
	/**
	 *  Singleton instance
	 */
	static iass_designer_server_link_mngr* st_instance;	// singletone instance

	/**
	 *  Mutex protecting the singleton instance obj creation
	 */
	static ACE_Thread_Mutex st_mutex;		// singletone obj creation mutex
};

#endif /*iass_designer_server_link_mngr_hh*/

