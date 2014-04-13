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

#ifndef iass_corba_orb_mngr_hh
#define iass_corba_orb_mngr_hh

#include <iostream>
#include "iass_libdebug.hh"
#include <tao/PortableServer/PortableServer.h>
#include <orbsvcs/orbsvcs/CosNamingC.h>
#include <ace/Thread_Mutex.h>
#include <ace/Thread_Manager.h>

/** class iass_corba_orb_mngr
 *  \brief Singleton helper class used to manage the CORBA orb
 */
class iass_corba_orb_mngr
{
/* functions */
public:
	/** 
	 *  \brief Get singleton instance
	 */
	static iass_corba_orb_mngr* instance();

	/** 
	 *  \brief Get a obj from its string addres(IOR)
	 *  \param ior Stringified address of the obj
	 *  \return A local ptr to the remote obj (trough the CORBA machinery)
	 *
	 *  Can return NULL when obj couldn't be found
	 */
	CORBA::Object* obj_from_ior(std::string ior);


	/** 
	 *  \brief Get a obj by its registration name to the NamingService
	 *  \param name registration name to the NamingService
	 *  \return A local ptr to the remote obj (trough the CORBA machinery)
	 *
	 *  Can return NULL when obj couldn't be found
	 */
	CORBA::Object* naming_service_obj(std::string name);


	/** 
	 *  \brief Get a obj using the CORBA resolve_initial_references method
	 *  \param name reference name of the service requested
	 *  \return A ptr to an obj offering the requested service
	 *
	 *  Can return NULL when obj couldn't be found
	 */
	CORBA::Object* resolve_initial_references(std::string name);


	/** 
	 *  \brief Activates the Corba POA
	 *
	 *  If called when POA is already active, simply returns
	 */
	void activate_poa(void);


	/** 
	 *  \brief Shutdowns the Corba POA
	 *
	 *  If called when POA isn't active, simply returns
	 */
	void shutdown_poa(void);

  
	/** 
	 *  \brief Returns the active state of the Corba POA
	 *  \return true when POA ia active, false otherwise
	 */
	bool poa_is_active(void);


	/** 
	 *  \brief Runs the CORBA Orb in a separate thread
	 *
	 *  During the call a new thread is spawned to run the Orb's main loop. The call
	 *  will return only when the Orb is effectively running (in the worst case only few msecs).
	 *  If callid while the Orb is already running, simply returns.
	 */
	void run_separate_thread(void);


	/** 
	 *  \brief Returns the running state of the Corba ORB
	 *  \return true when the ORB is running, false otherwise
	 */
	bool orb_is_running(void);


	/** 
	 *  \brief Shutdowns the Corba ORB
	 *
	 *  If called when ORB isn't running, simply returns
	 */
	void shutdown_orb(void);


	/** 
	 *  \brief Shutdowns the singleton class
	 */
	void shutdown(void);

private:
	/** 
	 *  \brief Singleton's private constructor
	 */
	iass_corba_orb_mngr();


	/** 
	 *  \brief Internal helper method used to spawn the thread that runs the orb main loop
	 *  \param args a ptr to the singletone instance of this obj
	 */
	static void* internal_run_separate_thread(void* args);

/* members */
private:
	/** 
	 *  Mutex protecting access to the class members.
	 *  Always acquire it before working on orb, 
	 */

	/* always acquire/release the mutex when accessing members */
	ACE_Thread_Mutex mutex;
	CORBA::ORB_ptr orb;
	bool running;
	PortableServer::POA_var poa;

	/* singleton stuff */
	static iass_corba_orb_mngr* st_instance;	// singleton instance
	static ACE_Thread_Mutex st_mutex;		// singleton obj creation mutex
};

#endif /*iass_corba_orb_mngr_hh*/

