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

#include "iass_corba_orb_mngr.hh"

iass_corba_orb_mngr* iass_corba_orb_mngr::st_instance = 0;
ACE_Thread_Mutex iass_corba_orb_mngr::st_mutex;

iass_corba_orb_mngr* iass_corba_orb_mngr::instance()
{
	/* queue concurrent threads */
	st_mutex.acquire();
	if (st_instance==0) st_instance = new iass_corba_orb_mngr();
	st_mutex.release();
	return st_instance;
}


CORBA::Object* iass_corba_orb_mngr::obj_from_ior(std::string ior) {
	CORBA::Object* obj = NULL;

	mutex.acquire();
	if (!orb) {
		std::cout << "warning iass_corba_orb_mngr::obj_from_ior(std::string ior), orb not inited\n";
		mutex.release();
		return NULL;
	}

	/* try to resolve name */
	try {
		obj = orb->string_to_object (ior.c_str());
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_corba_orb_mngr::obj_from_ior(std::string ior), exception on string_to_obj(\""
			  << ior << "\")\n";
	}
	mutex.release();
	return obj;
}


CORBA::Object* iass_corba_orb_mngr::naming_service_obj(std::string name) {
	CORBA::Object_ptr obj = NULL;

	mutex.acquire();
	if (!orb) {
		std::cout << "iass_corba_orb_mngr::naming_service_obj(std::string name), orb not inited\n";
		mutex.release();
		return NULL;
	}

	/* find the Naming Service */
	CosNaming::NamingContext_var naming_context;
	try {
		CORBA::Object_var naming_context_object = orb->resolve_initial_references("NameService");
		naming_context = CosNaming::NamingContext::_narrow (naming_context_object.in());
		mutex.release();
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_corba_orb_mngr::naming_service_obj(std::string name), couldn't find the naming service\n"
			  << " - likely it isn't running; if it *is*, try re-launching it with NamingService -m1\n";
		mutex.release();
		return NULL;
	}

	/* find the requested obj by registration name */
	try {
		CosNaming::Name _name (1);
		_name.length (1);
		_name[0].id = CORBA::string_dup(name.c_str());
  
		obj = naming_context->resolve(_name);
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_corba_orb_mngr::naming_service_obj(std::string name), couldn't resolve \""
			  << name << "\"\n";
		return NULL;
	}
	return obj;
}


CORBA::Object* iass_corba_orb_mngr::resolve_initial_references(std::string name) {
	CORBA::Object_ptr obj = NULL;

	mutex.acquire();
	if (!orb) {
		std::cout << "iass_corba_orb_mngr::resolve_initial_references(std::string name), orb not inited\n";
		mutex.release();
		return NULL;
	}

	/* resolve reference */
	try {
		obj = orb->resolve_initial_references(name.c_str());
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_corba_orb_mngr::resolve_initial_references(std::string name), couldn't resolve service \"" <<  name << "\"\n";
	}
	mutex.release();
	return obj;
}


void iass_corba_orb_mngr::activate_poa(void) {
	mutex.acquire();
	if (!orb) {
		std::cout << "iass_corba_orb_mngr::activate_poa(void), orb not inited\n";
		mutex.release();
		return;
	}
	if (poa) {
		std::cout << "warning iass_corba_orb_mngr::activate_poa(), poa already active\n";
		mutex.release();
		return;
	}

	/* get root POA and activate poa manager */
	try {
		CORBA::Object_var poa_object = orb->resolve_initial_references ("RootPOA");
		poa = PortableServer::POA::_narrow (poa_object.in ());
		PortableServer::POAManager_var poa_manager = poa->the_POAManager ();
		poa_manager->activate ();
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_corba_orb_mngr::activate_poa(), poa activation failed\n";
	}
	mutex.release();
}


void iass_corba_orb_mngr::shutdown_poa(void) {
	mutex.acquire();
	if (!orb) {
		std::cout << "warning iass_corba_orb_mngr::shutdown_poa(void), orb not inited\n";
		mutex.release();
		return;
	}

	if (!poa) {
		std::cout << "warning iass_corba_orb_mngr::shutdown_poa(void), poa not active\n";
		mutex.release();
		return;
	}

	/* shutdown poa */
	try {
		poa->destroy (1, 1);
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_corba_orb_mngr::shutdown_poa(void), couldn't destroy poa\n";
	}
	mutex.release();
}


bool iass_corba_orb_mngr::poa_is_active(void) {
	mutex.acquire();
	bool return_value = (bool)poa;
	mutex.release();
	return return_value;
}

void iass_corba_orb_mngr::run_separate_thread(void) {
	mutex.acquire();
	if (orb==NULL) {
		std::cout << "warning iass_corba_orb_mngr::run_separate_thread(), orb not inited\n";
		mutex.release();
		return;
	}

	mutex.release();
	/* spawn thread */
	ACE_Thread_Manager::instance()->spawn (iass_corba_orb_mngr::internal_run_separate_thread,
					       (void*) this,
					       THR_DETACHED | THR_SCOPE_SYSTEM ); 

	/* wait until orb is effectively running (few millisecs in worst case) */
	while (true) {
		mutex.acquire();
		if (running) {
			mutex.release();
			break;
		}
		mutex.release();
		usleep(10);
	}
}


bool iass_corba_orb_mngr::orb_is_running(void) {
	mutex.acquire();
	bool return_value = running;
	mutex.release();
	return return_value;
}



void iass_corba_orb_mngr::shutdown_orb(void) {
	mutex.acquire();
	if (!orb) {
		std::cout << "warning iass_corba_orb_mngr::shutdown_orb(void), orb not inited\n";
		mutex.release();
		return;
	}

	if (!running) {
		std::cout << "warning iass_corba_orb_mngr::shutdown_orb(void), orb not running\n";
		mutex.release();
		return;
	}

	/* shutdown orb */
	try {
		orb->shutdown();
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_corba_orb_mngr::shutdown_orb(void), couldn't shutdown orb\n";
	}
	mutex.release();
}


void iass_corba_orb_mngr::shutdown(void) {
	mutex.acquire();
	if (poa) {
		mutex.release();
		shutdown_poa();
	}

	mutex.acquire();
	if (orb!=NULL) {
		mutex.release();
		shutdown_orb();
	}
}



/* **************** private **************** */

iass_corba_orb_mngr::iass_corba_orb_mngr() {
	std::cout << " * initing component: iass_corba_orb_mngr\n";

	orb = NULL;
	running = false;
	poa = NULL;

	/* initialize ORB */
	try {
		int argc = 0;
		char** argv = NULL;
		orb = CORBA::ORB_init (argc, argv, "" /* ORB name */);
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_corba_orb_mngr(constr.), couldn't do basic ORB init!\n";
		orb = NULL;
		return;
	}
}


void* iass_corba_orb_mngr::internal_run_separate_thread(void* args) {
	iass_corba_orb_mngr* orb_mngr;
	assert(args);

	orb_mngr = (iass_corba_orb_mngr*) args;
	assert(orb_mngr->orb);
	orb_mngr->mutex.acquire();
	orb_mngr->running = true;
	orb_mngr->mutex.release();
	try {
		orb_mngr->orb->run();
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_corba_orb_mngr::internal_run_separate_thread(void* args), orb->run(), exception\n";
	}
	orb_mngr->mutex.acquire();
	orb_mngr->running = false;
	orb_mngr->mutex.release();

	std::cout << "warning iass_corba_orb_mngr::internal_run_separate_thread(void* args), should join thread (?)\n";
	return 0;
}


