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

#include "iass_scene_opal.hh"

iass_scene_opal::iass_scene_opal(iass_scene_stats* stats) : iass_server_scene(stats)
{
	filesystem = iass_filesystem::instance();
	simulator = opal::createSimulator();
}

opal::Simulator* iass_scene_opal::internal_opal_simulator(void) {
	return simulator;
}

/*
  CORBA::Short iass_scene_opal::realize_entity (const raw_stream& _xml_ref ) throw () {
  unsigned int stream_len = _xml_ref.length();
  if (stream_len==0) {
  std::cout << "warning iass_entity_factory::realize_entity(const raw_stream& _xml_ref), _xml_ref has zero length\n";
  return -1;
  }

  const char* stream_buf = (const char*) _xml_ref.get_buffer();
  if (stream_buf==NULL) {
  std::cout << "warning iass_entity_factory::realize_entity(const raw_stream& _xml_ref), _xml_ref.buffer() is a NULL ptr\n";
  return -1;
  }

  iass_entity_ref xml_ref(stream_buf, stream_len);

  if (!xml_ref.consistency_check()) {
//   print out some debugging info 
std::cout << "warning iass_entity_factory::realize_entity(const raw_stream& _xml_ref), "
<< "xml_ref.consistency_check() failed\n"
<< "xml_ref.path.buf_ptr() " << xml_ref.path()->buf_ptr() << ", .len() " << xml_ref.path()->len() << "\n"
<< "xml_ref.node_type.buf_ptr() " << xml_ref.node_type()->buf_ptr() << ", .len() " << xml_ref.node_type()->len() << "\n"
<< "xml_ref.node_id.buf_ptr() " << xml_ref.node_id()->buf_ptr() << ", .len() " << xml_ref.node_id()->len() << "\n";
return -1;
}

std::cout << "warning iass_entity_factory::realize_entity(const raw_stream& _xml_ref), we should change the shared path in _xml_ref.path() to a local absolute path... implement me!\n";

iass_file_type file_t = filesystem->get_file_type(xml_ref.path()->buf_ptr());
iass_entity* new_entity = NULL;

if (file_t == file_t_basictype) {
new_entity = basictype_entities_factory->build_entity(xml_ref);
}
else if (file_t == file_t_unknown) {
std::cout << "warning iass_entity_factory::realize_entity(const raw_stream& _xml_ref), "
<< "file at path " << xml_ref.path()->buf_ptr() << " is of iass_file_type file_t_unknown\n";
return -1;
}
else {
std::cout << "warning iass_entity_factory::realize_entity(const raw_stream& _xml_ref), "
<< "filesystem->get_file_type(" << xml_ref.path()->buf_ptr() << ") returned an invalid value\n";
return -1;
}

//   was the entity really build by any iass_entities_factory ? 
if (!new_entity) {
std::cout << "warning iass_entity_factory::realize_entity(const raw_stream& _xml_ref), the new entity "
<< "wasn't really build by factories\n";
return -1;
}


unsigned int insert_pos = entities_collection.insert(new_entity);

//   update all spectators ... 

return insert_pos;
}*/





