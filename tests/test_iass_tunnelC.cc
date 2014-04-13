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
 *  g++ -otunnelC -lTAO_CosNaming -lTAO_PortableServer test_iass_tunnelC.cc ../libiass/src/iass_corba_orb_mngr.cc ../libiass/src/idls iass_tunnel_idlC.cpp ../libiass/src/idls/iass_corba_rpc_typesC.cpp
 */


#include <iostream>
#include <string>
#include <iass/idls/iass_tunnel_idlC.h>
#include <iass/iass_corba_orb_mngr.hh>
#include <orbsvcs/orbsvcs/CosNamingC.h>
#include <sys/time.h>

inline unsigned int iass_utils_time_passed(timeval& old, timeval& current);

inline unsigned int iass_utils_time_passed(timeval& old, timeval& current) {
	return ( (1000000 * (current.tv_sec - old.tv_sec) + current.tv_usec) - old.tv_usec);
}

#define sample_mult 10 
#define sample_time 10000000 /*usec*/


int main (int argc, char** argv)
{
	std::cout << "\n *  iass|intelligent autonomous systems studio\n"
		  << " |  test|iass_tunnel *C*\n";


	/* get access to the orb */
	iass_corba_orb_mngr* orb_mngr = iass_corba_orb_mngr::instance();

	/* run orb */
	orb_mngr->run_separate_thread();
	if (!orb_mngr->orb_is_running()) {
		std::cout << "\n ! iass|tunnelC, orb->run_separate_thread() didn't work as exspected, exiting ...\n";
		orb_mngr->shutdown_orb();
		return 1;
	}

	/* ativate poa */
	orb_mngr->activate_poa();
	if (!orb_mngr->poa_is_active()) {
		std::cout << "\n ! iass|tunnelC, poa init didn't work as exspected, exiting ...\n";
		orb_mngr->shutdown_orb();
		return 1;
	}

	/* find the remote tunnel obj */
	CORBA::Object_ptr obj = NULL;
	iass_tunnel_idl* tunnel = NULL;
	std::string ns_name("test_tunnel");

	obj = iass_corba_orb_mngr::instance()->naming_service_obj(ns_name);

	/* check if naming_service search succeded */
	if (!obj) {
		std::cout << "\n ! iass|tunnelC, didn't find remote obj\n";
		return 1;
	}

	try {
		tunnel = iass_tunnel_idl::_narrow(obj);
	}
	catch (CORBA::Exception &) {
		std::cout << "warning exception on _narrow\n";
		return 1;
	}

	timeval last;
	timeval current;

	/* get current time */
	gettimeofday(&last, NULL);
	iass_corba_raw_stream stream;
	stream.length(1);

	unsigned int count = 0;

	while (1) {
   
		gettimeofday(&current, NULL);
		unsigned int time_passed = iass_utils_time_passed(last, current);
		if (time_passed > sample_time) {
			stream.length(sample_mult * stream.length());
			memset (stream.get_buffer(), 1, stream.length());

			count ++;
			std::cout << " -" << count << "-, stream length is "<< stream.length()<<"\n";

			/* copy contents of current time to last time */
			last.tv_sec = current.tv_sec;
			last.tv_usec = current.tv_usec;
		}

		/* send data */
		tunnel->push(stream);
		usleep(10);

	}

	orb_mngr->shutdown_poa();
	orb_mngr->shutdown_orb();

	return 0;
}
