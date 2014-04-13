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

#include <iass/iass_orb_basic_mngr.hh>
#include <iostream>
#include <sstream>
#include <string>


int main (int argc, char** argv)
{
	std::cout << "\n test for iass_orb_basic_mngr\n";

	/* get a iass_orb_basic_mngr instance */
	iass_orb_basic_mngr* orb_mngr = iass_orb_basic_mngr::instance();

	if (orb_mngr!=NULL) {
		/* iass_orb_basic_mngr->register_orb_user facilities */
		std::cout << " * iass_orb_basic_mngr->register_orb_user(\"...\") ...\n";
		std::cout << " ! this test should output only a warning for the user name \"reg_1\" being used two times\n";
		CORBA::ORB_var reg_1 = orb_mngr->register_orb_user("reg_1");
		if (reg_1==NULL) std::cout << "FAILED, register_orb_user for user reg_1 returned NULL\n";
  
		CORBA::ORB_var reg_2 = orb_mngr->register_orb_user("reg_2");
		if (reg_2==NULL) std::cout << "FAILED, register_orb_user for user reg_2 returned NULL\n";
		if (reg_2!=reg_1) std::cout << "FAILED, register_orb_user for user reg_2 different orb than in precedence with reg_1\n";

		CORBA::ORB_var reg_3 = orb_mngr->register_orb_user("reg_3");
		if (reg_3==NULL) std::cout << "FAILED, register_orb_user for user reg_3 returned NULL\n";
		if (reg_3!=reg_1) std::cout << "FAILED, register_orb_user for user reg_3 different orb than in precedence with reg_1 \n";
		if (reg_3!=reg_2) std::cout << "FAILED, register_orb_user for user reg_3 different orb than in precedence with reg_2\n";

		CORBA::ORB_var reg_4 = orb_mngr->register_orb_user("reg_4");
		if (reg_4==NULL) std::cout << "FAILED, register_orb_user for user reg_4 returned NULL\n";
		if (reg_4!=reg_1) std::cout << "FAILED, register_orb_user for user reg_4 different orb than in precedence with reg_1 \n";
		if (reg_4!=reg_2) std::cout << "FAILED, register_orb_user for user reg_4 different orb than in precedence with reg_2\n";
		if (reg_4!=reg_3) std::cout << "FAILED, register_orb_user for user reg_4 different orb than in precedence with reg_3\n";


		/* try registering two times a user with same name */
		CORBA::ORB_var reg_5 = orb_mngr->register_orb_user("reg_1");
		if (reg_5==NULL) std::cout << "FAILED, register_orb_user second time for user reg_1 returned NULL\n";
		if (reg_5!=reg_1) std::cout << "FAILED, register_orb_user second time for user reg_1 different orb than in precedence with reg_1 \n";
		if (reg_5!=reg_2) std::cout << "FAILED, register_orb_user second time for user reg_1 different orb than in precedence with reg_2\n";
		if (reg_5!=reg_3) std::cout << "FAILED, register_orb_user second time for user reg_1 different orb than in precedence with reg_3\n";

		/* iass_orb_basic_mngr->unregister_orb_user facilities */
		std::cout << "\n * iass_orb_basic_mngr->unregister_orb_user(\"...\") ...\n";
		std::cout << " ! for the following tests, if iass_orb_basic_mngr doesn't output anything than everithing works ok\n";
		if (reg_1!=NULL) {
			std::cout << " - unregistering user with name \"reg_1\"...\n";
			orb_mngr->unregister_orb_user("reg_1");
		}
		if (reg_2!=NULL) {
			std::cout << " - unregistering user with name \"reg_2\"...\n";
			orb_mngr->unregister_orb_user("reg_2");
		}
		if (reg_3!=NULL) {
			std::cout << " - unregistering user with name \"reg_3\"...\n";
			orb_mngr->unregister_orb_user("reg_3");
		}
		if (reg_4!=NULL) {
			std::cout << " - unregistering user with name \"reg_4\"...\n";
			orb_mngr->unregister_orb_user("reg_4");
		}
		if (reg_5!=NULL) {
			std::cout << " - unregistering user with name \"reg_1\" (previosly registered two times)...\n";
			orb_mngr->unregister_orb_user("reg_1");
		}

		/* iass_orb_basic_mngr->is_running() method */
		std::cout << "\n * iass_orb_basic_mngr->is_running() ...";
		if (orb_mngr->is_running()) std::cout << "FAILED, orb wasn't yet started\n";
		else std::cout << "OK, orb not yet running\n";
    
		/* iass_orb_basic_mngr->run_orb_separate_thread() */
		std::cout << "\n * iass_orb_basic_mngr->run_orb_separate_thread() ...\n";
		orb_mngr->run_orb_separate_thread();
		std::cout << " * checking if orb is really running: ";
		if (orb_mngr->is_running()) std::cout << "OK, orb seems to be running\n"; 
		else std::cout << "FAILED, orb seems not being running\n";
	}
	else std::cout << "FAILED, iass_orb_basic_mngr::instance() returned a NULL ptr\n";

	return 0;
}
