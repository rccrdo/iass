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

#include <iass/iass_priority_queue.hh>
//#include <iass/iass_priority_queue_uint.hh>
#include <vector>
#include <map>
#include <iostream>

unsigned int random_integer (unsigned int lowest, unsigned int highest) {
	if (lowest >= highest)
		std::cout << " ! test_iass_priority_queue_uint::random_integer, lowest >= highest\n";
	int range = (highest-lowest)+1; 
	unsigned int ri = lowest + (unsigned int)(rand() % range);
	return ri;
}


int main (int argc, char** argv)
{
	std::cout << "\n *  iass|intelligent autonomous systems studio\n"
		  << " |  test|iass_priority_queue_uint\n";

	iass_priority_queue<unsigned int> pq;

	unsigned int test_failed = 0;
	unsigned int times = 80000000;

	std::cout << " - insertion...\n";
	unsigned int v = 5;
	for (unsigned int i=0; i< times; i++) {
		v = 5 /*random_integer(0,499)*/;
		pq.insert(v);

		std::cout << " - remove_min...\n";
		unsigned int min;
		for (unsigned int i=0; i< times; i++) {
			if (!(pq.remove_min(min))) std::cout << " ["<<i<<"]\t, error on remove_min: " << min << "\n";

			std::cout << "\n =====================================\n"
				  << "  * test result for " << times << "\n";
			if (test_failed) {
				std::cout << "FAILED for " << test_failed << " times, ~" << (((double) test_failed)/((double)times))*100 << "% \n";
			}
			else std::cout << "PASSED\n";
			std::cout << " =====================================\n";

			return 0;
		}
