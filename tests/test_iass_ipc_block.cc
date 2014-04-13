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

#include <iostream>
#include <string>
#include <iass/iass_ipc_block.hh>

unsigned int random_integer (unsigned int lowest, unsigned int highest) {
	if (lowest >= highest)
		std::cout << " ! test_iass_ipc_block::random_integer, lowest >= highest\n";
	int range=(highest-lowest)+1; 
	unsigned int ri = lowest + (unsigned int)(rand() % range);
	return ri;
}

#define max_num_elements 3
#define max_pattern_len 5


int main (int argc, char** argv)
{
	std::cout << "\n *  iass|intelligent autonomous systems studio\n"
		  << " |  test|iass_ipc_block\n";

	srand((unsigned)time(0));
	unsigned int times_failed = 0;
	unsigned int times = 1000000;
	char buffer[100000];
	memset (buffer, 255, 100000);

	std::cout << " * creating, verifying serialization and destroying " << times << " iass_ipc_blocks\n";

	for (unsigned int i=0; i< times; i++) {
		iass_ipc_block update;
		unsigned num_elements;

		/* int8_vector */
		num_elements = random_integer (0,max_num_elements);
		for (unsigned int j = 0; j < num_elements; j++) {
			char c =  random_integer(0,20);
			update.push_int8(c);
		}

		/* int16_vector */
		num_elements = random_integer (0,max_num_elements);
		for (unsigned int j = 0; j < num_elements; j++) {
			unsigned int v = random_integer(0,10000);
			update.push_int16(v);
		}

		/* int32_vector */
		num_elements = random_integer (0,max_num_elements);
		for (unsigned int j = 0; j < num_elements; j++) {
			int v = (random_integer(0,10000) - 5000);
			update.push_int32(v);
		}

		/* float_vector */
		num_elements = random_integer (0,max_num_elements);
		for (unsigned int j = 0; j < num_elements; j++) {
			float num = random_integer(0,10000);
			float den = random_integer(1,10000);
			float v = num / den;
			update.push_float(v);
		}

		/* double_vector */
		num_elements = random_integer (0,max_num_elements);
		for (unsigned int j = 0; j < num_elements; j++) {
			double num = random_integer(0,10000);
			double den = random_integer(1,10000);
			double v = num / den;
			update.push_double(v);
		}

		/* cstrings_vector */
		num_elements = random_integer (0,max_num_elements);
		for (unsigned int j = 0; j < num_elements; j++) {
			// generate a random pattern 
			unsigned int pattern_len = random_integer (1,max_pattern_len);
			char* test_pattern = (char*) malloc(pattern_len +1);
			test_pattern[pattern_len]=0;

			for (unsigned int j = 0; j < pattern_len; j++) {
				test_pattern[j] = 'a' + (char) (j%20);
			}

			iass_memchunk* cstring = new iass_memchunk(test_pattern, pattern_len);
			update.push_cstring(cstring);
			free (test_pattern);
		}


		/* serialize to buffer */
		unsigned int used_bytes =  update.serialize(buffer);
		if (used_bytes!=update.serialized_len()) {
			std::cout << "failed: ! serialized buffer bytes count " << used_bytes
				  << ", update.serialized_len() " << update.serialized_len() << "\n";
		}

		iass_ipc_block update2;
		update2.unserialize(buffer, used_bytes);

		/* check int8_vector */    
		if (update.int8_vector.size()!=update2.int8_vector.size()) {
			std::cout << "failed: ! update.int8_vector.size() " << update.int8_vector.size()
				  << ", update2.int8_vector.size() " << update2.int8_vector.size() << "\n";
			times_failed++;
		}
		for (unsigned int j=0; j < update.int8_vector.size(); j++) {
			if (update.int8_vector[j]!= update2.int8_vector[j]) {
				std::cout << "failed: ! update.int8_vector[" << j << "]=" << update.int8_vector[j] << "!="
					  << "update2.int8_vector[" << j << "]=" << update2.int8_vector[j]  << "\n";
				times_failed++;
			}
		}

		/* check int16_vector */    
		if (update.int16_vector.size()!=update2.int16_vector.size()) {
			std::cout << "failed: ! update.int16_vector.size() " << update.int16_vector.size()
				  << ", update2.int16_vector.size() " << update2.int16_vector.size() << "\n";
			times_failed++;
		}
		for (unsigned int j=0; j < update.int16_vector.size(); j++) {
			if (update.int16_vector[j]!= update2.int16_vector[j]) {
				std::cout << "failed: ! update.int16_vector[" << j << "]=" << update.int16_vector[j] << "!="
					  << "update2.int16_vector[" << j << "]=" << update2.int16_vector[j]  << "\n";
				times_failed++;
			}
		}

		/* check int32_vector */    
		if (update.int32_vector.size()!=update2.int32_vector.size()) {
			std::cout << "failed: ! update.int32_vector.size() " << update.int32_vector.size()
				  << ", update2.int32_vector.size() " << update2.int32_vector.size() << "\n";
			times_failed++;
		}
		for (unsigned int j=0; j < update.int32_vector.size(); j++) {
			if (update.int32_vector[j]!= update2.int32_vector[j]) {
				std::cout << "failed: ! update.int32_vector[" << j << "]=" << update.int32_vector[j] << "!="
					  << "update2.int32_vector[" << j << "]=" << update2.int32_vector[j]  << "\n";
				times_failed++;      
			}
		}

		/* check float_vector */    
		if (update.float_vector.size()!=update2.float_vector.size()) {
			std::cout << "failed: ! update.float_vector.size() " << update.float_vector.size()
				  << ", update2.float_vector.size() " << update2.float_vector.size() << "\n";
			times_failed++;
		}
		for (unsigned int j=0; j < update.float_vector.size(); j++) {
			if (update.float_vector[j]!= update2.float_vector[j]) {
				std::cout << "failed: ! update.float_vector[" << j << "]=" << update.float_vector[j] << "!="
					  << "update2.float_vector[" << j << "]=" << update2.float_vector[j]  << "\n";
				times_failed++;      
			}
		}

		/* check double_vector */    
		if (update.double_vector.size()!=update2.double_vector.size()) {
			std::cout << "failed: ! update.double_vector.size() " << update.double_vector.size()
				  << ", update2.double_vector.size() " << update2.double_vector.size() << "\n";
			times_failed++;
		}
		for (unsigned int j=0; j < update.double_vector.size(); j++) {
			if (update.double_vector[j]!= update2.double_vector[j]) {
				std::cout << "failed: ! update.double_vector[" << j << "]=" << update.double_vector[j] << "!="
					  << "update2.double_vector[" << j << "]=" << update2.double_vector[j]  << "\n";
				times_failed++;
			}
		}

		/* check cstrings_vector */    
		if (update.cstrings_vector.size()!=update2.cstrings_vector.size()) {
			std::cout << "failed: ! update.cstrings_vector.size() " << update.cstrings_vector.size()
				  << ", update2.cstrings_vector.size() " << update2.cstrings_vector.size() << "\n";
			times_failed++;
		}
		for (unsigned int j=0; j < update.cstrings_vector.size(); j++) {
			/* check for same lengths */
			if (update.cstrings_vector[j]->used!= update2.cstrings_vector[j]->used) {
				std::cout << "failed: ! update.cstrings_vector[" << j << "]->used=" << update.cstrings_vector[j]->used << " != "
					  << "update2.cstrings_vector[" << j << "]->used=" << update2.cstrings_vector[j]->used  << "\n";
				times_failed++;
			}
			/* check for same contents */
			if ( strncmp ((const char*)update.cstrings_vector[j]->mem,
				      (const char*)update2.cstrings_vector[j]->mem,
				      update.cstrings_vector[j]->used )!=0 ) {
				std::cout << "failed: ! update.cstrings_vector[" << j << "]->mem != "
					  << "update2.cstrings_vector[" << j << "]->mem\n";
				times_failed++;
			}
		}

	}

	std::cout << "\n";  
	if (!times_failed) std::cout << " * ok.. test passed !\n";
	else std::cout << " * test failed " << times_failed << " times\n";

	return 0;
}
