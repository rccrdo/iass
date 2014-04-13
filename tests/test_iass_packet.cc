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
#include <iass/iass_packet.hh>

unsigned int random_integer (unsigned int lowest, unsigned int highest) {
	if (lowest >= highest)
		std::cout << " ! test_iass_packet::random_integer, lowest >= highest\n";
	int range=(highest-lowest)+1; 
	unsigned int ri = lowest + (unsigned int)(rand() % range);
	return ri;
}

#define max_pattern_len 25


int main (int argc, char** argv)
{
	std::cout << "\n *  iass|intelligent autonomous systems studio\n"
		  << " |  test|iass_packet\n";

	srand((unsigned)time(0));
	unsigned int times_failed = 0;
	unsigned int times = 100000000;
	char buffer[100000];
	char content[100000];
	memset (buffer, 0, 100000);
	memset (content, 0, 100000);

	std::cout << " * creating, verifying serialization and destroying " << times << " iass_packets\n";

	for (unsigned int i=0; i< times; i++) {

		// generate a random pattern 
		unsigned int pattern_len = (i%max_pattern_len) + 1;
		char* test_pattern = (char*) malloc(pattern_len +1);
		test_pattern[pattern_len]=0;

		for (unsigned int j = 0; j < pattern_len; j++) {
			test_pattern[j] = 'a' + (char) (j%20);
		}

		unsigned int packet_len = iass_packet_serialize(buffer, test_pattern, pattern_len);

		iass_packet_info info;
		iass_packet_unserialize(info, content, buffer + IASS_PACKET_HEADER_STRING_LEN);

		if (strncmp(content, test_pattern, pattern_len)!=0) {
			std::cout << "unserialized content is different from gen pattern\n";
		}
		if (info.packet_len!=(packet_len - IASS_PACKET_HEADER_STRING_LEN - IASS_PACKET_TAIL_STRING_LEN)) {
			std::cout << "unserialized content len is different from gen pattern len\n";
		}

		unsigned int begin_cur = 0;
		unsigned int end_cur = 0;
		unsigned int discardable = iass_packet_search_stream(buffer, packet_len, begin_cur, end_cur);
		iass_packet_unserialize(info, content, buffer + begin_cur);
		if (strncmp(content, test_pattern, pattern_len)!=0) {
			std::cout << "(search_stream) unserialized content is different from gen pattern\n";
		}
		if (info.packet_len!=(packet_len - IASS_PACKET_HEADER_STRING_LEN - IASS_PACKET_TAIL_STRING_LEN)) {
			std::cout << "(search_stream) unserialized content len is different from gen pattern len\n";
		}


		free(test_pattern);
	}

	std::cout << "\n";  
	if (!times_failed)
		std::cout << " * ok.. test passed !\n";
	else
		std::cout << " * test failed " << times_failed << " times\n";

	return 0;
}
