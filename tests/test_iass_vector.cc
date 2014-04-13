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

#include <iass/iass_vector.hh>
#include <iass/iass_entity.hh>
#include <iostream>
#include <vector>

#define WITH_IASS_VECTOR
//#define WITH_STD_VECTOR

int main (int argc, char** argv)
{
	std::cout << "\n *  iass|intelligent autonomous systems studio\n"
		  << " |  test|iass_vector\n";


	unsigned int times = 10;
	unsigned int repeat_times = 10000000;

	std::cout << " - inserting and clearing vector for " << repeat_times << " times...\n";
#ifdef WITH_IASS_VECTOR
	std::cout << "   - iass_vector\n"; 
#endif
#ifdef WITH_STD_VECTOR
	std::cout << "   - std::vector\n";
#endif


	for (unsigned int j=0; j < repeat_times; j++ ) {
#ifdef WITH_IASS_VECTOR
		iass_vector< iass_entity* > vect;
#endif
#ifdef WITH_STD_VECTOR
		std::vector< iass_entity* > std_v;
#endif

		for (unsigned int i=0; i< times+1; i++) {
#ifdef WITH_IASS_VECTOR
			vect.push_back((iass_entity*)i);
			if (vect[i]!=(iass_entity*)i) std::cout << " " << i << " failed\n";
#endif

#ifdef WITH_STD_VECTOR
			std_v.push_back((iass_entity*)i);
#endif
		}

#ifdef WITH_IASS_VECTOR
		vect.clear();
#endif

#ifdef WITH_STD_VECTOR
		std_v.clear();
#endif
	}

	return 0;
};
