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

#include <iass/iass_cstring.hh>

unsigned int random_integer (unsigned int lowest, unsigned int highest) {
	if (lowest >= highest)  std::cout << " ! test_iass_cstring::random_integer, lowest >= highest\n";
	int range=(highest-lowest)+1; 
	unsigned int ri = lowest + (unsigned int)(rand() % range);
	return ri;
}


int main (int argc, char** argv)
{
	std::cout << "\n *  iass|intelligent autonomous systems studio\n"
		  << " |  test|iass_cstring\n";

	srand((unsigned)time(0));
	unsigned int test_failed = 0;
	unsigned int times = 10000000;
	std::string a;

	unsigned int test_pattern_length = 50;
	std::cout << " * creating and destroying " << times << " random patterns " << test_pattern_length << " bytes long\n";
	for (unsigned int i=0; i< times; i++) {
		char* test_pattern = (char*) malloc(test_pattern_length +1);
		test_pattern[test_pattern_length]=0;

		/* generate random pattern test_pattern_length long */
		for (unsigned int j = 0; j < test_pattern_length; j++) {
			test_pattern[j] = 'a' + (char) j;
		}

		/* check if something whent wrong */
		iass_cstring* cs = new iass_cstring(test_pattern, test_pattern_length);
		if (cs->buf==NULL or cs->len==0) {
			std::cout << " ! failed iass_cstring creation, buf or len are 0s\n";
			test_failed ++;
		}

		if (strcmp( test_pattern, cs->buf)!=0) {
			std::cout << " ! failed iass_cstring creation, test_pattern is " << test_pattern << ", and iass_cstring is " << cs->buf <<"\n";
			test_failed ++;
		}

		free(test_pattern);
		delete cs;
	}

	std::cout << "\n =====================================\n"
		  << "  * test result for " << times << " patterns: ";
	if (test_failed)
		std::cout << "FAILED for " << test_failed << " times, ~" << (((double) test_failed)/((double)times))*100 << "% \n";
	else
		std::cout << "PASSED\n";
	std::cout << " =====================================\n";

	return 0;
}
