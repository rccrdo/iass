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

#include <iass/iass_loop_sleeper.hh>

unsigned int random_integer (unsigned int lowest, unsigned int highest) {
	if (lowest >= highest)
		std::cout << " ! test_iass_pq_heap_uint::random_integer, lowest >= highest\n";
	int range = (highest-lowest)+1; 
	unsigned int ri = lowest + (unsigned int)(rand() % range);
	return ri;
}


int main (int argc, char** argv) {
	std::cout << "\n *  iass|intelligent autonomous systems studio\n"
		  << " |  test|iass_loop_sleeper\n";

	unsigned int times = 20000;

	iass_loop_sleeper lp;
	iass_loop_sleeper_set_frequency(lp, 100);

	for (unsigned int i = 0; i < times; i++) {
		iass_loop_sleeper_sleep(lp);
		for (unsigned int j=0; j< 5000; j++) random_integer(1,5);    
	}

	return 0;
}
