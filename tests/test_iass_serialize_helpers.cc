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

#include <iass/iass_serialize_helpers.hh>

unsigned int random_integer (unsigned int lowest, unsigned int highest) {
	if (lowest >= highest)
		std::cout << " ! test_iass_serialize_helpers::random_integer, lowest >= highest\n";
	int range=(highest-lowest)+1; 
	unsigned int ri = lowest + (unsigned int)(rand() % range);
	return ri;
}


int main (int argc, char** argv)
{
	std::cout << "\n *  iass|intelligent autonomous systems studio\n"
		  << " |  test|iass_serialize_helpers\n";

	char buffer[1000];
	unsigned int times = 100000000;
	unsigned int test_failed = 0;

	std::cout << " * issuing iass_serialize* and iass_unserialize* " << times << " times on different types\n\n";

	std::cout << " * int8 ...\n";
	for (unsigned int j = 0; j < times; j++) {
		char a =  random_integer(0,255);
		iass_serialize_int8(buffer, a);    
		char b = iass_unserialize_int8(buffer); 
		if (a!=b) {
			std::cout << " int8 " << j << ", a " << (int) a << ", " << (int) b <<"\n";
			test_failed++;
		}
	}


	std::cout << " * int16 ...\n";
	for (unsigned int j = 0; j < times; j++) {
		int16_t a =  random_integer(0,65000);
		iass_serialize_int16(buffer, a);    
		int16_t b = iass_unserialize_int16(buffer); 
		if (a!=b) {
			std::cout << " int16 " << j << ", a " << a << ", " <<  b <<"\n";
			test_failed++;
		}
	}


	std::cout << " * int32 ...\n";
	for (unsigned int j = 0; j < times; j++) {
		unsigned int a =  random_integer(0,5000000);
		iass_serialize_int32(buffer, a);    
		unsigned int b = iass_unserialize_int32(buffer); 
		if (a!=b) {
			std::cout << " int32 " << j << ", a " << a << ", " <<  b <<"\n";
			test_failed++;
		}  
	}

	std::cout << " * floats ...\n";
	for (unsigned int j = 0; j < times; j++) {
		float num = random_integer(0,10000);
		float den = random_integer(1,10000);
		float a = num / den;
		iass_serialize_float(buffer, a);    
		float b = iass_unserialize_float(buffer); 
		if (a!=b) {
			std::cout << " floats" << j << ", a " << a << ", " <<  b <<"\n";
			test_failed++;
		}
	}

	std::cout << " * doubles ...\n";
	for (unsigned int j = 0; j < times; j++) {
		double num = random_integer(0,10000);
		double den = random_integer(1,10000);
		double a = num / den;
		iass_serialize_double(buffer, a);    
		double b = iass_unserialize_double(buffer); 
		if (a!=b) {
			std::cout << " doubles " << j << ", a " << a << ", " <<  b <<"\n";
			test_failed++;
		}
	}

	std::cout << "\n";  
	if (!test_failed) std::cout << " * ok.. test passed !\n";
	else std::cout << " * test failed " << test_failed << " times\n";

	return 0;
}
