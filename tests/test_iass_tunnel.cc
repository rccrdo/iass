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

/*
 * g++ -otunnelS -lACE -lTAO_PortableServer -lTAO_CosNaming test_iass_tunnel.cc ../libiass/src/iass_corba_orb_mngr.cc ../libiass/src/idls/iass_tunnel_idl{C,S}.cpp ../libiass/src/idls/iass_corba_rpc_types{C,S}.cpp
 */

#include <iostream>
#include <string>
#include <iass/idls/iass_tunnel_idlS.h>
#include <iass/iass_corba_orb_mngr.hh>
#include <orbsvcs/orbsvcs/CosNamingC.h>
#include <sys/time.h>

inline unsigned int iass_utils_time_passed(timeval& old, timeval& current);

inline unsigned int iass_utils_time_passed(timeval& old, timeval& current) {
	return ( (1000000 * (current.tv_sec - old.tv_sec) + current.tv_usec) - old.tv_usec);
}

#define sample_time 250000 /*usec*/

class iass_tunnel_i : public POA_iass_tunnel_idl {

public:
	iass_tunnel_i() {
		/* get current time */
		gettimeofday(&last, NULL);
		rec_bytes = 0;   
	}

/* CORBA rpc methods */
public:
	void push(const iass_corba_raw_stream& stream) throw() {
		gettimeofday(&current, NULL);
		unsigned int time_passed = iass_utils_time_passed(last, current);
		rec_bytes += stream.length();

		if (time_passed > sample_time) {
			/* print statistics and reset */
			double passed = (double) time_passed;
			passed = passed /1000000;
			double rate = rec_bytes / passed;
			rate = rate / 1000;
			std::cout << "[" << current.tv_sec << "," << current.tv_usec << "], " << rate << "KBps\n";
			/* copy contents of current time to last time */
			last.tv_sec = current.tv_sec;
			last.tv_usec = current.tv_usec;
			rec_bytes = 0;
		}
	}

/* members */
public:
	unsigned int rec_bytes;
	timeval last;
	timeval current;
};


int main (int argc, char** argv) {
	std::cout << "\n *  iass|intelligent autonomous systems studio\n"
		  << " |  test|iass_tunnel *S*\n";

	/* get access to the orb */
	iass_corba_orb_mngr* orb_mngr = iass_corba_orb_mngr::instance();

	/* activate poa */
	orb_mngr->activate_poa();
	if (!orb_mngr->poa_is_active()) {
		std::cout << "\n ! iass|tunnel_server, poa init didn't work as exspected, exiting ...\n";
		orb_mngr->shutdown_orb();
		return 1;
	}

	/* create the tunnel implementor */
	iass_tunnel_idl_var tunnel = (new iass_tunnel_i)->_this();

	/* register the tunnel_server to the NamingService */
	try {
		std::cout << " * iass|tunnel_server, looking for the NamingService...\n";
		CORBA::Object_ptr naming_context_object = orb_mngr->resolve_initial_references("NameService");
		CosNaming::NamingContext_var naming_context = CosNaming::NamingContext::_narrow (naming_context_object);

		CosNaming::Name name(1);
		name.length(1);
		name[0].id = CORBA::string_dup ("test_tunnel");
		naming_context->rebind(name, tunnel.in());
	}
	catch (CORBA::Exception &) {
		std::cout << " ! iass|tunnel_server, couldn't bind to the NamingService!\n"
			  << "   - if a tunnel_server is already running use the -f option; otherwise\n"
			  << "     is likely no naming service at all is running in the network\n";
	}

	/* run the orb (using iass_corba_orb_mngr non blocking helper method */
	std::cout << " * iass|tunnel_server, entering ORB loop... ";
	orb_mngr->run_separate_thread();
	if (!orb_mngr->orb_is_running()) {
		std::cout << "failed, exiting...\n";
		return 1;
	} else std::cout << "ok\n";


	while (1) usleep (1000000);


	std::cout << "...\n";  
	return 0;
}
