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

#include "iass_designer_orb_mngr.hh"


iass_designer_orb_mngr* iass_designer_orb_mngr::st_instance = 0;
ACE_Thread_Mutex iass_designer_orb_mngr::st_mutex;

iass_designer_orb_mngr* iass_designer_orb_mngr::instance()
{
	/* queue concurrent threads */
	st_mutex.acquire();
	if (st_instance==0) st_instance = new iass_designer_orb_mngr();
	st_mutex.release();
	return st_instance;
}


iass_designer_orb_mngr::iass_designer_orb_mngr() {
	std::cout << " * initing component: iass_designer_orb_mngr\n";
	inited = false;
	remote_scene = NULL;

	/* initialize the ORB */
	try {
		int argc = 0;
		char** argv = NULL;
		orb = CORBA::ORB_init (argc, argv, "" /* ORB name */);
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_designer_orb_mngr(), couldn't do basic ORB init!\n";
		return;
	}

	/* find the Naming Service */
	try {
		CORBA::Object_var naming_context_object = orb->resolve_initial_references("NameService");
		naming_context = CosNaming::NamingContext::_narrow (naming_context_object.in());
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_designer_orb_mngr(constr.), couldn't find the naming service\n"
			  << "   - likely it isn't running; if it *is*, try re-launching it with NamingService -m1\n";
		shutdown();
		return;
	}

	/* find the iass|simulations_server component */
	try {
		/* get the right service's name for component iass|simulations_server from iass_configs */
		std::string ns_sims_server_name("");
		if (iass_configs::instance()->get_parameter(SIMULATIONS_SERVER_NAMING_SERVICE_ID, &ns_sims_server_name) != 0 )
			ns_sims_server_name = SIMULATIONS_SERVER_NAMING_SERVICE_ID;
		std::cout << " * iass_designer_orb_mngr(), searching for the iass|simulations_server as:" << ns_sims_server_name << "\n";

		CosNaming::Name name (1);
		name.length (1);
		name[0].id = CORBA::string_dup (ns_sims_server_name.c_str());

		/* resolve service name */
		CORBA::Object_var simulations_server_obj = naming_context->resolve(name);
		remote_simulations_server = iass_simulations_server_idl::_narrow(simulations_server_obj.in());

	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_designer_orb_mngr(constr.), couldn't resolve service\n"
			  << "   - likely, or the component iass|simulations_server isn't running at all,"
			  << " or it isn't registered to the naming service or it is registered with a different name\n";
		shutdown();
		return;
	}

	try {
		remote_scene = remote_simulations_server->new_scene();
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_designer_orb_mngr(constr.), remote scene request failed\n";
		shutdown();
		return;
	}

	inited = true;
}


void iass_designer_orb_mngr::shutdown(void) {
	if (!inited) return;

	/* free ORB memory (BLOCKING) */
	inited = false;

	try {
//    orb->destroy();
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_designer_orb_mngr::shutdown(void), couldn't destroy orb gracefully\n";
		return;
	}
}


iass_scene_idl* iass_designer_orb_mngr::current_scene(void) {
	if (!inited) {
		std::cout << "warning iass_designer_orb_mngr::current_scene(void), orb manager is *not* inited\n";
	}

	if (!remote_scene) {
		std::cout << "warning iass_designer_orb_mngr::current_scene(void), current_scene is a NULL ptr\n";
	}
	return remote_scene;
}

