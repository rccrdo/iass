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

#include <iass/iass_queue.hh>
#include <queue>
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <cstdlib>


unsigned int random_integer (unsigned int lowest, unsigned int highest) {
	if (lowest >= highest)
		std::cout << " ! test_iass_collection_entities::random_integer, lowest >= highest\n";
	int range=(highest-lowest)+1; 
	unsigned int ri = lowest + (unsigned int)(rand() % range);
	return ri;
}

int main (int argc, char** argv)
{
	std::cout << "\n *  iass|intelligent autonomous systems studio\n"
		  << " |  test|iass_queue_char\n";

	srand((unsigned)time(0));
	unsigned int times_failed = 0;
	unsigned int times = 60000000;

	/* queues */
	iass_queue<void*> queue;
	std::queue<void*> std_queue;

	/* push simple */
	std::cout << " - pushing char by char...\n";
	for (unsigned int i=0; i< times; i++) {
		void* a =  (void*) random_integer(0,4000000);
		void* iass[5] ;
		iass[0] = a;
		queue.push(iass, 1);
		std_queue.push(a);
	}

	/* pop simple */
	std::cout << " - popping char by char\n";
	for (unsigned int i=0; i< times; i++) {
		void* std_res = std_queue.front();
		std_queue.pop();

		void* iass[5] ;
		unsigned int popped = queue.top(iass, 1);
		queue.pop(1);

		if (std_res!= iass[0]) {
			std::cout << "[" << i << "] - " << (unsigned int) std_res << "," << (unsigned int) iass[0] << "\n";
		}
	}

	std::cout << "\n =====================================\n"
		  << "  * test result for " << times << "\n";
	if (times_failed) {
		std::cout << "FAILED for " << times_failed << " times, ~" << (((double) times_failed)/((double)times))*100 << "% \n";
	}
	else std::cout << "PASSED\n";
	std::cout << " =====================================\n";

	return 0;
}


