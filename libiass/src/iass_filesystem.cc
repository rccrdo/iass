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

#include "iass_filesystem.hh"

iass_filesystem* iass_filesystem::st_instance = 0;
ACE_Thread_Mutex iass_filesystem::st_mutex;

iass_filesystem* iass_filesystem::instance()
{
	/* queue concurrent threads */
	st_mutex.acquire();
	if (st_instance==0) st_instance = new iass_filesystem();
	st_mutex.release();
	return st_instance;
}



iass_filesystem::iass_filesystem() {
	std::cout << " * initing component: iass_filesystem\n";

	shared_folder_path = NULL;

	/* get the nfs shared folder path */
	/*iass_configs* cfg = iass_configs::instance();
	  std::string cfg_path("");
	  int cfg_err;*/


/*  cfg_path = SCENES_SERVER_SHARED_FOLDER_ID;
    if ( (cfg_err = cfg->get_parameter(cfg_path, &shared_folder_path)) < 0) {
    shared_folder_path = SCENES_SERVER_SHARED_FOLDER_DEF;
    }*/

	shared_folder_path = new iass_cstring (SCENES_SERVER_SHARED_FOLDER_DEF, strlen(SCENES_SERVER_SHARED_FOLDER_DEF));

	/* check if the specified shared folder exists */
	DIR *dp = opendir((const char*)shared_folder_path->buf);
	if (dp==NULL) {
		std::cout << "warning iass_filesystem, looks like the shared folder doesn't already exists... trying to mkdir "
			  << shared_folder_path->buf << "\n";
		if (mkdir ((const char*)shared_folder_path->buf, S_IRWXU | S_IRWXG) ==-1) {
			std::cout << "warning ::iass_filesystem(), not enough permissions for mkdir\n"
				  << "        ! we are running with no shared folder; only clients running on this machine will work properly\n";
			delete shared_folder_path;
			shared_folder_path = NULL;
		}
	}
  
	if (shared_folder_path) std::cout << " * iass_filesystem, using " << shared_folder_path->buf << " as shared folder\n";
}



iass_sharedfile_data* iass_filesystem::make_shared(const char* path) {
	/* check arguments */
	if (path==NULL) {
		std::cout << "warning iass_filesystem::make_shared, path is a NULL ptr\n";
		return NULL;
	}

	access_mutex.acquire();
	/* check if a make_shared request was already issued on this file */
	sources_map_iter = sources_map.find(path);
	if( sources_map_iter != sources_map.end() ) {
		/* this file was already asked to be shared */
		access_mutex.release();
		return shared_files_vector[sources_map_iter->second];
	}

	/* what file type is it ? if we can't handle it just return */
	iass_file_type file_t = file_t_unknown;//get_file_type(path);
/*  if (file_t == file_t_unknown) {
    std::cout << "warning iass_filesystem::make_shared, get_file_type returned file_t_unknown for :" << path << "\n";
    return NULL;
    }*/

	/* find the file name in the whole path string */
	unsigned int sep_pos;
	for (sep_pos = (strlen(path) -1); sep_pos>=0; sep_pos --) {
		if (path[sep_pos] == '/') break;
	}
	if (sep_pos == 0 and path[0]!='/' ) {
		std::cout << "warning iass_filesystem::make_shared, couldn't find the filename in path: " << path << "\n";
		access_mutex.release();
		return NULL;
	}

	/* separate the filename from the whole path */
	char* shared_path = tempnam ((const char*)shared_folder_path->buf, &path[sep_pos+1]);
	if (!shared_path) {
		std::cout << "warning iass_filesystem::make_shared, tempnam(" << shared_folder_path->buf
			  << ", " << &path[sep_pos+1] << ") returned a NULL ptr\n";
		access_mutex.release();
		return NULL;
	}

	/* copy file from local source to the shared destination */  
	if (copy_file (path, shared_path) <0) {
		access_mutex.release();
		return NULL;
	}

	/* save that we share this file */
	iass_sharedfile_data* sf = NULL;
	sf = new iass_sharedfile_data(path, strlen(path), &path[sep_pos+1], (strlen(path)-sep_pos), file_t );

	shared_files_vector.push_back(sf);
	sources_map.insert( std::make_pair(path, shared_files_vector.size()-1) );  

	/* free tempnam malloc'ed memory */
	free(shared_path);

	access_mutex.release();
	return sf;
}



int iass_filesystem::copy_file(const char* source, const char* dest) {
	/* check arguments */
	if (source==NULL) {
		std::cout << "warning iass_filesystem::copy_file, source is a NULL ptr\n";
		return -1;
	}
	if (dest==NULL) {
		std::cout << "warning iass_filesystem::copy_file, dest is a NULL ptr\n";
		return -1;
	}

	if (source[0]!='/') {
		std::cout << "warning iass_filesystem::copy_file, source seems not being an absolute path\n";
	}
	if (dest[0]!='/') {
		std::cout << "warning iass_filesystem::copy_file, dest seems not being an absolute path\n";
	}


	/* open source and destination streams */
	std::ifstream source_file;
	source_file.open(source, ios::in | ios::binary);
	if (!source_file.is_open())
	{
		std::cout << "warning iass_filesystem::copy_file, couldn't open the source file: "
			  << source << "\n";
		return -1;
	}

	std::ofstream dest_file;
	dest_file.open(dest, ios::out | ios::binary | ios::trunc);
	if (!dest_file.is_open())
	{
		std::cout << "warning iass_filesystem::copy_file, couldn't open destination file: " 
			  << dest << "\n";
		source_file.close();
		return -1;
	}

	/* calculate file size */
	source_file.seekg (0, ios::end);
	std::ifstream::pos_type source_size = source_file.tellg();
	source_file.seekg (0, ios::beg);

	/* copy chunks of size FILECOPY_BUFFER_SIZE */
	while (source_size > FILECOPY_BUFFER_SIZE ) {
		source_file.read (filecopy_buffer, FILECOPY_BUFFER_SIZE);
		dest_file.write(filecopy_buffer, FILECOPY_BUFFER_SIZE);
		source_size -= FILECOPY_BUFFER_SIZE;
	}
	/* copy remaining bytes */
	if (source_size) {
		source_file.read (filecopy_buffer, source_size);
		dest_file.write(filecopy_buffer, source_size);
	}
  
	dest_file.close();
	source_file.close();
	return 0;
}



iass_file_type iass_filesystem::get_file_type(const char* path) {
	if (path==NULL) {
		std::cout << "warning iass_filesystem::get_file_type(const char* path), path is a NULL ptr\n";
		return file_t_unknown;
	}


	/* *** for know we handle only very basic (internally hacky handled model types ) *** */
	if (strcmp(path, IASS_ENTITY_REF_BASICTYPE_PATH )==0) return file_t_basictype;

	/* try to open file */
/*  xmlDocPtr doc;
    doc = xmlParseFile(path);
    if (doc== NULL ) {
    std::cout << "warning iass_filesystem::get_file_type, " << path << " isn't a xml file\n";
    return file_t_unknown;
    }
*/

	/* read file's root element */  
//  xmlNodePtr cur = xmlDocGetRootElement(doc);
//  if (cur == NULL) {
//    std::cout << "warning iass_filesystem::get_file_type, " << path << " is empty\n";
	/*free the document */
//    xmlFreeDoc(doc);
//    return file_t_unknown;    
//  }

//  iass_file_type file_t = file_t_unknown;
//  if (xmlStrcmp(cur->name, (const xmlChar *) "COLLADA version=\"1.4.0\"")) file_t = file_t_collada;
//  else if (xmlStrcmp(cur->name, (const xmlChar *) "file type something")) file_t = file_t_type_something;

	/*free document */
//  xmlFreeDoc(doc);

	std::cout << "warning iass_filesystem::get_file_type(" << path  << "), file at path is of iass_file_type file_t_unknown\n";
	return file_t_unknown;    
}



