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


#ifndef iass_packet_hh
#define iass_packet_hh

#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>
#include "iass_libdebug.hh"
#include "iass_entity_ref.hh"
#include "iass_serialize_helpers.hh"


struct iass_packet_info {
	/* len in bytes of the packet without header and tail */
	unsigned int packet_len;

};

#define IASS_PACKET_INFO_SERIALIZED_LEN  IASS_SERIALIZED_LEN_UINT32

#define IASS_PACKET_HEADER_STRING     "<h"
#define IASS_PACKET_HEADER_STRING_LEN 2
#define IASS_PACKET_TAIL_STRING       "t>"
#define IASS_PACKET_TAIL_STRING_LEN   2
#define IASS_PACKET_MIN_LEN  (IASS_PACKET_HEADER_STRING_LEN	\
                              + IASS_PACKET_INFO_SERIALIZED_LEN \
                              + IASS_PACKET_TAIL_STRING_LEN )




/* serialize a iass_packet_info struct into dest
 * ! caller must make sure dest is accessible for at least
 IASS_PACKET_INFO_SERIALIZED_LEN bytes */
inline void iass_packet_info_serialize(char* dest, iass_packet_info& info);

  
/* unserialize a (valid)stream into a iass_packet_info struct
 * ! caller must make sure src is accessible for at least
 IASS_PACKET_INFO_SERIALIZED_LEN bytes */
inline void iass_packet_info_unserialize(iass_packet_info& info, const char* src);



/* returns the maximum len of the serialized packet for a given len of the content
 * ! see that header, tail and the serialized iass_packet_info struct always take
 *    a fixed amount of bytes when serialized */
inline unsigned int iass_packet_serialized_max_len (unsigned int content_len);

/* serialize a packet to a ginven buffer
 * ! in the future compression may be done here ...
 * ! the caller must make sure that dest's buf is accessible for at least
 iass_packet_serialized_max_len (unsigned int content_len)
 * - returned value is the number of bytes used in dest */
inline unsigned int iass_packet_serialize (char* dest,
                                           const char* content,
                                           unsigned int content_len );

/* unserialize a packet to a given buffer
 * ! in the future decompression may be done here ...
 * ! the caller must make sure that dest's buf is accessible for at least
 info.packet_len bytes
 * - returned value is the number of bytes used in content_dest */
inline unsigned int iass_packet_unserialize(iass_packet_info& info, char* content_dest, const char *packet);


/* search the first packet in a stream of bytes
 * - if found any set pckt_data_begin_cur to byte rigth fater the header and
 *    pckt_data_end_cur to the byte rigth before the first char of the tail
 * - if nothing was found just set pckt_data_end_cur to zero 
 * - the returned value is the number of discrdable bytes in src
 *    ie. when a stream is courrupted to have false positives (otherwise this 
 functions would always stop to the first false positive returning end_cur=0)*/
inline unsigned int iass_packet_search_stream(const char* src,
                                              unsigned int src_len, 
                                              unsigned int& pckt_data_begin_cur,
                                              unsigned int& pckt_data_end_cur);





unsigned int iass_packet_serialized_max_len (unsigned int content_len) {
	return (IASS_PACKET_HEADER_STRING_LEN + IASS_PACKET_INFO_SERIALIZED_LEN 
		+ content_len + IASS_PACKET_TAIL_STRING_LEN );
}


void iass_packet_info_serialize(char* dest, iass_packet_info& info) {
	assert(dest);
	assert(info.packet_len);

	/* ! always serialize the length as first the thing !
	 * !  it's used when searching a packet in a stream to avoid false positives */
	iass_serialize_uint32 (dest, info.packet_len);
}


void iass_packet_info_unserialize(iass_packet_info& info, const char* src) {
	assert(src);
	info.packet_len = iass_unserialize_uint32(src);
}


unsigned int iass_packet_serialize (char* dest,
                                    const char* content,
                                    unsigned int content_len ) {
	iass_packet_info packet_info;
	unsigned int cur = 0;

	assert(dest);
	assert(content);
	assert(content_len);

	/* copy the header */
	memcpy (dest, IASS_PACKET_HEADER_STRING, IASS_PACKET_HEADER_STRING_LEN);
	cur += IASS_PACKET_HEADER_STRING_LEN;

	/* set up the iass_packet info struct */
	packet_info.packet_len = IASS_PACKET_INFO_SERIALIZED_LEN + content_len;

	/* copy the iass_packet_info struct */
	iass_packet_info_serialize(dest + cur, packet_info);
	cur += IASS_PACKET_INFO_SERIALIZED_LEN;

	/* copy the content */
	memcpy (dest + cur, content, content_len);  
	cur += content_len;

	/* copy tail */
	memcpy (dest + cur, IASS_PACKET_TAIL_STRING, IASS_PACKET_TAIL_STRING_LEN);  
	cur += IASS_PACKET_TAIL_STRING_LEN;
	return cur;
}


unsigned int iass_packet_unserialize(iass_packet_info& info, char* content_dest, const char *packet) {
	unsigned int cur = 0;
	unsigned int content_len;

	assert(content_dest);
	assert(packet);

	iass_packet_info_unserialize(info, packet);
	cur += IASS_PACKET_INFO_SERIALIZED_LEN;

	content_len = info.packet_len - IASS_PACKET_INFO_SERIALIZED_LEN;
	memcpy(content_dest, packet + cur, content_len);
	cur += content_len;
	return cur;
}



unsigned int iass_packet_search_stream(const char* src,
                                       unsigned int src_len, 
                                       unsigned int& pckt_data_begin_cur,
                                       unsigned int& pckt_data_end_cur)
{
	char* search_result = NULL;
	unsigned int packet_len;
	assert(src);
	assert(src_len >= IASS_PACKET_MIN_LEN);
	/* reset cursors, indicating no packet found in src */
	pckt_data_begin_cur = 0;
	pckt_data_end_cur = 0;

	/* search the header string in src */
	search_result = (char*) memmem((const void*) src, src_len, 
				       (const void*) IASS_PACKET_HEADER_STRING, IASS_PACKET_HEADER_STRING_LEN);
	if (!search_result) {
		/* couldn't find a header, the whole stream can be discarded */
		return src_len;
	}

	/* if we got here we found the header string in src */
	pckt_data_begin_cur = (search_result -src) + IASS_PACKET_HEADER_STRING_LEN;

	/* check if there is enough space for the remaining part of the packet in src */
	if ( !((pckt_data_begin_cur + IASS_PACKET_MIN_LEN -1) <= src_len) ) {
		/* can discard the stream until the first char before the header */
		return pckt_data_begin_cur;
	}
	/* first 4 bytes of data store the packet len from iass_packet_info */
	packet_len = iass_unserialize_uint32(src + pckt_data_begin_cur);

	if (!packet_len) {
		/* false positive, the stream until the end of the header can be discarded */
		return (pckt_data_begin_cur + 1);
	}

	/* search the tail string in src */
	search_result = (char*) memmem((const void*) (src + pckt_data_begin_cur + IASS_PACKET_HEADER_STRING_LEN),
				       (src_len - pckt_data_begin_cur - IASS_PACKET_HEADER_STRING_LEN), 
				       (const void*) IASS_PACKET_TAIL_STRING, IASS_PACKET_TAIL_STRING_LEN  );
	if (!search_result) {
		/* couldn't find a tail; can discard the stream until the char where the header was found */
		return pckt_data_begin_cur;
	}

	if (search_result != (src + pckt_data_begin_cur + packet_len) ) {
		/* false positive, tail was found in a different position than for info.packet_len.
		 * can discard the stream until the first char after the end of the fake tail */
		return (search_result - src) + IASS_PACKET_TAIL_STRING_LEN;
	}
	/* if we got here we found the tail in the right position */
	pckt_data_end_cur = (search_result -src) - 1;

	/* after the caller ha worked on the packet the stream can be discarded until the last char
	 *  of the tail */
	return pckt_data_end_cur + IASS_PACKET_TAIL_STRING_LEN  + 1;
} 

#endif /*iass_packet_hh*/

