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

#include "iass_configs.hh"

/* to add a new parameter:
 *   - allocate the right type (iass_configs_{int,uint,boolean...}_entry} on the heap
 *   - call store_parameter(parameter*)
 */

iass_configs* iass_configs::st_instance = 0;
ACE_Thread_Mutex iass_configs::st_mutex;

iass_configs* iass_configs::instance()
{
	/* queue concurrent threads */
	st_mutex.acquire();
	if (st_instance==0) st_instance = new iass_configs();
	st_mutex.release();
	return st_instance;
}


iass_configs::iass_configs() {
	std::cout << " * initing component: iass_configs\n";
	iass_configs_entry* p = NULL;



	p = new iass_configs_string_entry(SCENES_SERVER_SHARED_FOLDER_ID,
					  SCENES_SERVER_SHARED_FOLDER_DESC, 
					  SCENES_SERVER_SHARED_FOLDER_DEF);
	store_parameter(p);

	/* scenes_server */
	p = new iass_configs_string_entry(SCENES_SERVER_NAMING_SERVICE_ID,
					  SCENES_SERVER_NAMING_SERVICE_DESC, 
					  SCENES_SERVER_NAMING_SERVICE_DEF);
	store_parameter(p);



	/* check for a default configs file */
	std::cout << "warning iass_configs(), should check for default configs file!\n";
}

void iass_configs::save_to_file(char* path) {
	if (!path) {
		std::cout << "warning iass_configs::save_to_file(char* path), path is a NULL ptr\n";
		return;
	}
	std::cout << "warning iass_configs::save_to_file(), implement me !\n";
}

void iass_configs::load_from_file(char* path) {
	if (!path) {
		std::cout << "warning iass_configs::load_from_ nefile(char* path), path is a NULL ptr\n";
		return;
	}
	std::cout << "warning iass_configs::load_from_file(), implement me !\n";

	/* check if a config file is present */
/*  xmlDocPtr doc;
    doc = xmlParseFile("configs.xml");
    if (doc== NULL ) {
    std::cout << "warning iass_configs(), configuration file not found\n";
    write_configs_file();
    return;
    }*/

	/* read configuration from file */  
/*  xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
    std::cout << "warning iass_configs(), configuration file is empty\n";
    write_configs_file();
    return;
    }

    if (xmlStrcmp(cur->name, (const xmlChar *) "iass_configs")) {
    std::cout << "warning iass_configs(), document of the wrong type\n";
    xmlFreeDoc(doc);
    return;
    }


    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *)"storyinfo"))) {
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *)"keyword"))) {
    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
    printf("keyword: %s\n", key);
    xmlFree(key);
    }
    cur = cur->next;
    }
    }
    cur = cur->next;
    }*/


	/*free the document */
//  xmlFreeDoc(doc);
}



void iass_configs::store_parameter(iass_configs_entry* p) {
	if (!p) {
		std::cout << "warning iass_configs::store_parameter(NULL ptr)\n";
		return;
	}
	access_mutex.acquire();

	std::string id(p->get_path());
	entries_map_iter = entries_map.find(id);
	if( entries_map_iter != entries_map.end() ) {
		std::cout << "warning iass_configs::store_parameter, path: " << id << " already used!\n";
		access_mutex.release(); 
		return;
	}

	entries_vector.push_back(p);
	entries_map.insert( std::make_pair(id, entries_vector.size()-1) );  
	access_mutex.release();
}

void iass_configs::write_configs_file() {

	xmlDocPtr doc = NULL;       /* document pointer */
	xmlNodePtr root_node = NULL;
	std::vector<xmlNodePtr> doc_nodes;

	/* creates a new xml document and adds the root node */
	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "iass_configs");
	xmlDocSetRootElement(doc, root_node);
	doc_nodes.push_back(root_node);

	/* store entries alfabethically in the xml tree */
	entries_map_iter=entries_map.begin();

	iass_configs_entry* last = NULL;
	while (entries_map_iter!=entries_map.end()) {
		iass_configs_entry* p = entries_vector[entries_map_iter->second];    

		unsigned int ne = p->get_num_path_elements();    
		if (ne==0) continue;

		if (last==NULL) {
			for (unsigned int i=0; i < ne; i++) {
				doc_nodes.push_back(xmlNewChild(doc_nodes[(doc_nodes.size()-1)], NULL, BAD_CAST (p->get_path_element((doc_nodes.size()-1))).c_str(), NULL ));
			}
		}
		else {
			/* find matching depth */
			unsigned int shorter = ne;
			unsigned int match_depth = 0;
			if (last->get_num_path_elements()<ne) shorter = last->get_num_path_elements();

			for (unsigned int i=0; i<shorter; i++) {
				if (p->get_path_element(i)== last->get_path_element(i)) match_depth = i+1;
				else break;
			}

			while ((doc_nodes.size()-1)>match_depth) doc_nodes.pop_back();
			while ((doc_nodes.size()-1)<p->get_num_path_elements()) {
				doc_nodes.push_back(xmlNewChild(doc_nodes[(doc_nodes.size()-1)], NULL, BAD_CAST (p->get_path_element((doc_nodes.size()-1))).c_str(), NULL ));
			}
		}

		last = p;
  
		std::stringstream ss;
		ss.clear();
		switch (p->get_type()) {
		case (INT): {
			iass_configs_int_entry* cp = (iass_configs_int_entry*) p;
			xmlNewProp(doc_nodes[(doc_nodes.size()-1)], BAD_CAST "type", BAD_CAST "INT");
			int v = cp->get_value();
			ss << v;  
			xmlNewProp(doc_nodes[(doc_nodes.size()-1)], BAD_CAST "value", BAD_CAST std::string(ss.str()).c_str());

			break;
		}
		case (UINT): {
			iass_configs_uint_entry* cp = (iass_configs_uint_entry*) p;
			xmlNewProp(doc_nodes[(doc_nodes.size()-1)], BAD_CAST "type", BAD_CAST "UINT");
			unsigned int v = cp->get_value();
			ss << v;  
			xmlNewProp(doc_nodes[(doc_nodes.size()-1)], BAD_CAST "value", BAD_CAST std::string(ss.str()).c_str());
			break;
		}
		case (BOOLEAN): {
			iass_configs_boolean_entry* cp = (iass_configs_boolean_entry*) p;
			xmlNewProp(doc_nodes[(doc_nodes.size()-1)], BAD_CAST "type", BAD_CAST "BOOL");
			bool v = cp->get_value();
			ss << v;  
			xmlNewProp(doc_nodes[(doc_nodes.size()-1)], BAD_CAST "value", BAD_CAST std::string(ss.str()).c_str());
			break;
		}
		case (DOUBLE): {
			iass_configs_double_entry* cp = (iass_configs_double_entry*) p;
			xmlNewProp(doc_nodes[(doc_nodes.size()-1)], BAD_CAST "type", BAD_CAST "DOUBLE");
			double v = cp->get_value();
			ss << v;  
			xmlNewProp(doc_nodes[(doc_nodes.size()-1)], BAD_CAST "value", BAD_CAST std::string(ss.str()).c_str());
			break;
		}
		case (STRING): {
			iass_configs_string_entry* cp = (iass_configs_string_entry*) p;
			xmlNewProp(doc_nodes[(doc_nodes.size()-1)], BAD_CAST "type", BAD_CAST "STRING");
			std::string v = cp->get_value();
			ss << v;  
			xmlNewProp(doc_nodes[(doc_nodes.size()-1)], BAD_CAST "value", BAD_CAST std::string(ss.str()).c_str());
			break;
		}
		default : {
			std::cout << "warning iass_configs::write_configs_file, found unknown parameter type\n";
			break;
		}
		}

		/* process next parameter */
		entries_map_iter++;
	}

	/* save document to file */
	xmlSaveFormatFile("configs.xml", doc , 1);

	/*free the document */
	xmlFreeDoc(doc);
}


int iass_configs::set_parameter(std::string path, int _value) {
	access_mutex.acquire();
	entries_map_iter = entries_map.find(path);
	if( entries_map_iter == entries_map.end() ) {
		std::cout << "warning iass_configs::set_parameter[INT] for non existing path: " << path << "\n";
		access_mutex.release();
		return -1;
	}
	iass_configs_entry* p = entries_vector[entries_map_iter->second];
	if (p->get_type()!=INT) {
		std::cout << "warning iass_configs::set_parameter[INT] for non-INT parameter type, path: " << path << "\n";
		access_mutex.release();
		return -2;
	}
	iass_configs_int_entry* ip = (iass_configs_int_entry*) p;
	ip->set_value(_value);  
	access_mutex.release();
	return 0;
}


int iass_configs::set_parameter(std::string path, unsigned int _value) {
	access_mutex.acquire();
	entries_map_iter = entries_map.find(path);
	if( entries_map_iter == entries_map.end() ) {
		std::cout << "warning iass_configs::set_parameter[UINT] for non existing path: " << path << "\n";
		access_mutex.release();
		return -1;
	}
	iass_configs_entry* p = entries_vector[entries_map_iter->second];
	if (p->get_type()!=UINT) {
		std::cout << "warning iass_configs::set_parameter[UINT] for non-UINT parameter type, path: " << path << "\n";
		access_mutex.release();
		return -2;
	}
	iass_configs_uint_entry* ip = (iass_configs_uint_entry*) p;
	ip->set_value(_value);  
	access_mutex.release();
	return 0;
}


int iass_configs::set_parameter(std::string path, bool _value) {
	access_mutex.acquire();
	entries_map_iter = entries_map.find(path);
	if( entries_map_iter == entries_map.end() ) {
		std::cout << "warning iass_configs::set_parameter[BOOL] for non existing path: " << path << "\n";
		access_mutex.release();
		return -1;
	}
	iass_configs_entry* p = entries_vector[entries_map_iter->second];
	if (p->get_type()!=BOOLEAN) {
		std::cout << "warning iass_configs::set_parameter[BOOL] for non-BOOLEAN parameter type, path: " << path << "\n";
		access_mutex.release();
		return -2;
	}
	iass_configs_boolean_entry* ip = (iass_configs_boolean_entry*) p;
	ip->set_value(_value);  
	access_mutex.release();
	return 0;
}


int iass_configs::set_parameter(std::string path, double _value) {
	access_mutex.acquire();
	entries_map_iter = entries_map.find(path);
	if( entries_map_iter == entries_map.end() ) {
		std::cout << "warning iass_configs::set_parameter[DOUBLE] for non existing path: " << path << "\n";
		access_mutex.release();
		return -1;
	}
	iass_configs_entry* p = entries_vector[entries_map_iter->second];
	if (p->get_type()!=DOUBLE) {
		std::cout << "warning iass_configs::set_parameter[DOUBLE] for non-DOUBLE parameter type, path: " << path << "\n";
		access_mutex.release();
		return -2;
	}
	iass_configs_double_entry* ip = (iass_configs_double_entry*) p;
	ip->set_value(_value);  
	access_mutex.release();
	return 0;
}


int iass_configs::set_parameter(std::string path, std::string _value) {
	access_mutex.acquire();
	entries_map_iter = entries_map.find(path);
	if( entries_map_iter == entries_map.end() ) {
		std::cout << "warning iass_configs::set_parameter[STRING] for non existing path: " << path << "\n";
		access_mutex.release();
		return -1;
	}
	iass_configs_entry* p = entries_vector[entries_map_iter->second];
	if (p->get_type()!=STRING) {
		std::cout << "warning iass_configs::set_parameter[STRING] for non-STRING parameter type, path: " << path << "\n";
		access_mutex.release();
		return -2;
	}
	iass_configs_string_entry* ip = (iass_configs_string_entry*) p;
	ip->set_value(_value);  
	access_mutex.release();
	return 0;
}



int iass_configs::get_parameter(std::string path, int* _value) {
	access_mutex.acquire();
	entries_map_iter = entries_map.find(path);
	if( entries_map_iter == entries_map.end() ) {
		std::cout << "warning iass_configs::get_parameter[INT] for non existing path: " << path << "\n";
		access_mutex.release();
		return -1;
	}
	iass_configs_entry* p = entries_vector[entries_map_iter->second];
	if (p->get_type()!=INT) {
		std::cout << "warning iass_configs::get_parameter[INT] for non-INT parameter type, path: " << path << "\n";
		access_mutex.release();
		return -2;
	}
	iass_configs_int_entry* ip = (iass_configs_int_entry*) p;
	*_value = ip->get_value();
	access_mutex.release();
	return 0;
}


int iass_configs::get_parameter(std::string path, unsigned int* _value) {
	access_mutex.acquire();
	entries_map_iter = entries_map.find(path);
	if( entries_map_iter == entries_map.end() ) {
		std::cout << "warning iass_configs::get_parameter[UINT] for non existing path: " << path << "\n";
		access_mutex.release();
		return -1;
	}
	iass_configs_entry* p = entries_vector[entries_map_iter->second];
	if (p->get_type()!=UINT) {
		std::cout << "warning iass_configs::get_parameter[UINT] for non-UINT parameter type, path: " << path << "\n";
		access_mutex.release();
		return -2;
	}
	iass_configs_uint_entry* ip = (iass_configs_uint_entry*) p;
	*_value = ip->get_value();
	access_mutex.release();
	return 0;
}


int iass_configs::get_parameter(std::string path, bool* _value) {
	access_mutex.acquire();
	entries_map_iter = entries_map.find(path);
	if( entries_map_iter == entries_map.end() ) {
		std::cout << "warning iass_configs::get_parameter[BOOLEAN] for non existing path: " << path << "\n";
		access_mutex.release();
		return -1;
	}
	iass_configs_entry* p = entries_vector[entries_map_iter->second];
	if (p->get_type()!=BOOLEAN) {
		std::cout << "warning iass_configs::get_parameter[BOOLEAN] for non-BOOLEAN parameter type, path: " << path << "\n";
		access_mutex.release();
		return -2;
	}
	iass_configs_boolean_entry* ip = (iass_configs_boolean_entry*) p;
	*_value = ip->get_value();
	access_mutex.release();
	return 0;
}


int iass_configs::get_parameter(std::string path, double* _value) {
	access_mutex.acquire();
	entries_map_iter = entries_map.find(path);
	if( entries_map_iter == entries_map.end() ) {
		std::cout << "warning iass_configs::set_parameter[DOUBLE] for non existing path: " << path << "\n";
		access_mutex.release();
		return -1;
	}
	iass_configs_entry* p = entries_vector[entries_map_iter->second];
	if (p->get_type()!=DOUBLE) {
		std::cout << "warning iass_configs::set_parameter[DOUBLE] for non-DOUBLE parameter type, path: " << path << "\n";
		access_mutex.release();
		return -2;
	}
	iass_configs_double_entry* ip = (iass_configs_double_entry*) p;
	*_value = ip->get_value();
	access_mutex.release();
	return 0;
}


int iass_configs::get_parameter(std::string path, std::string* _value) {
	access_mutex.acquire();
	entries_map_iter = entries_map.find(path);
	if( entries_map_iter == entries_map.end() ) {
		std::cout << "warning iass_configs::set_parameter[STRING] for non existing path: " << path << "\n";
		access_mutex.release();
		return -1;
	}
	iass_configs_entry* p = entries_vector[entries_map_iter->second];
	if (p->get_type()!=STRING) {
		std::cout << "warning iass_configs::set_parameter[STRING] for non-STRING parameter type, path: " << path << "\n";
		access_mutex.release();
		return -2;
	}
	iass_configs_string_entry* ip = (iass_configs_string_entry*) p;
	*_value = ip->get_value().c_str();
	access_mutex.release();
	return 0;
}



