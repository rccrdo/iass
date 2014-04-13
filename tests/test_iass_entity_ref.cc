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

#include <iass/iass_entity_ref.hh>
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <cstdlib>

#include <string.h>


int main (int argc, char** argv)
{
	std::cout << "\n *  iass|intelligent autonomous systems studio\n"
		  << " |  test|iass_entity_ref\n";

	srand((unsigned)time(0));
	unsigned int test_failed = 0;
	unsigned int times = 10000000;

	for (unsigned int i=0; i< times; i++) {
		unsigned int file_path_length = 30;
		unsigned int node_type_length = 20;
		unsigned int node_id_length = 20;

		unsigned int cur;
		char* string = (char*) malloc (6 + file_path_length + node_type_length + node_id_length +1 );
		string [6 + file_path_length + node_type_length + node_id_length +1 ] = 0;


		string[0] = (char) ((file_path_length & 0xFF00) >> 8);
		string[1] = (char) (file_path_length & 0xFF);
		string[2] =  (char) ((node_type_length & 0xFF00) >> 8);
		string[3] =  (char) (node_type_length & 0xFF);
		string[4] =  (char) ((node_id_length & 0xFF00) >> 8);
		string[5] =  (char) (node_id_length & 0xFF);
		cur = 6;

		for (unsigned int j = 0; j < file_path_length; j++) {
			string[cur] = 'a' + (char) (j%20);
			cur++;
		}

		for (unsigned int j = 0; j < node_type_length; j++) {
			string[cur] = 'a' + (char) (j%20);
			cur++;
		}

		for (unsigned int j = 0; j < node_id_length; j++) {
			string[cur] = 'a' + (char) (j%20);
			cur++;
		}

		iass_entity_ref ref(string, (6 + file_path_length + node_type_length + node_id_length));

		if ( ref.serialized->len != (6 + file_path_length + node_type_length + node_id_length) ) {
			std::cout << " ! failed serialized( orig="<< string << ", generated=" << ref.serialized->buf << "\n";
			test_failed ++;
		}
  
		free (string);
	}


	std::cout << "\n =====================================\n"
		  << "  * test result for " << times << " patterns: ";
	if (test_failed) {
		std::cout << "FAILED for " << test_failed << " times, ~" << (((double) test_failed)/((double)times))*100 << "% \n";
	}
	else std::cout << "PASSED\n";
	std::cout << " =====================================\n";

	return 0;
}
