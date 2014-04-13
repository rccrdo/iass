SHELL = /bin/sh
CC = g++ -g -O0 -fno-inline -Wall

LIB_OBJ_INSTALL_DIR = /usr/local/lib
LIB_HEADERS_INSTALL_DIR = /usr/local/include/iass
LIB_IDLS_INSTALL_DIR = /usr/local/include/iass/idls

LIBXML2_CFLAGS = -I/usr/include/libxml2
LIBXML2_LIBS = -L/usr/lib -lxml2

LIBACE_CFLAGS = -I/usr/include/ace
LIBACE_LIBS = -lACE

LIBTAO_CFLAGS = -I/usr/include/tao
LIBTAO_LIBS = -lTAO_PortableServer -lTAO_CosNaming

LIBOPAL_CFLAGS = -I/usr/local/include/opal
LIBOPAL_LIBS = -L/usr/local/lib -lopal_riccardo



# ************     libiass misc stuff     **************
LIBIASS_OBJ = iass_configs.o iass_configs_entry.o iass_corba_orb_mngr.o\
               iass_entity.o  iass_entity_ref.o iass_filesystem.o iass_ipcblock.o iass_ipctunnel_client.o \
               iass_ipctunnel.o iass_material.o iass_material_instance.o iass_materials_store.o \
               iass_scene.o iass_scene_mngr.o iass_sharedfile_data.o iass_tunnel_client.o 

LIBIASS_IDLS_OBJ = ./idls/iass_corba_rpc_typesC.o ./idls/iass_corba_rpc_typesS.o \
                    ./idls/iass_tunnel_idlC.o ./idls/iass_tunnel_idlS.o 

LIBIASS_NAME = libiass.so

LIBIASS_BUILD_LDFLAGS += $(LIBXML2_LIBS)
LIBIASS_BUILD_LDFLAGS += $(LIBTAO_LIBS)




# ********* libiass_scenes_server misc stuff ***********
LIBIASS_SCENES_SERVER_OBJ = iass_contact_geometry_opal.o iass_entity_factory_basictype_to_opal.o \
                             iass_server_scene_mngr.o iass_scene_opal.o iass_server_scene.o \
                             iass_scene_spectators_mngr.o iass_scenes_server.o iass_solid_brick_opal.o \
                             iass_solid_opal.o

LIBIASS_SCENES_SERVER_IDLS_OBJ = ./idls/iass_scenes_server_idlC.o ./idls/iass_scenes_server_idlS.o

LIBIASS_SCENES_SERVER_NAME = libiass_scenes_server.so

LIBIASS_SCENES_SERVER_BUILD_LDFLAGS += -L$(LIB_OBJ_INSTALL_DIR) -liass
LIBIASS_SCENES_SERVER_BUILD_LDFLAGS += $(LIBOPAL_LIBS)




# *********   libiass_spectator misc stuff   ***********
LIBIASS_SPECTATOR_OBJ = iass_client_scene_mngr.o iass_client_spectator_mngr.o

LIBIASS_SPECTATOR_IDLS_OBJ = ./idls/iass_scenes_server_idlC.o

LIBIASS_SPECTATOR_NAME = libiass_spectator.so

LIBIASS_SPECTATOR_BUILD_LDFLAGS += -L$(LIB_OBJ_INSTALL_DIR) -liass




# ************         make libs          **************
.PHONY: all
    all: libiass libiass_scenes_server libiass_spectator


.PHONY: libiass
    libiass: purge $(LIBIASS_IDLS_OBJ) $(LIBIASS_OBJ)
	$(CC) -o $(LIBIASS_NAME) -shared $(LIBIASS_BUILD_LDFLAGS) $(LIBIASS_IDLS_OBJ) $(LIBIASS_OBJ)


.PHONY: libiass_scenes_server
    libiass_scenes_server: purge $(LIBIASS_SCENES_SERVER_IDLS_OBJ) $(LIBIASS_SCENES_SERVER_OBJ)
	$(CC) -o $(LIBIASS_SCENES_SERVER_NAME) -shared $(LIBIASS_SCENES_SERVER_BUILD_LDFLAGS) $(LIBIASS_SCENES_SERVER_IDLS_OBJ) $(LIBIASS_SCENES_SERVER_OBJ)


.PHONY: libiass_spectator
    libiass_spectator: purge $(LIBIASS_SPECTATOR_IDLS_OBJ) $(LIBIASS_SPECTATOR_OBJ)
	$(CC) -o $(LIBIASS_SPECTATOR_NAME) -shared $(LIBIASS_SPECTATOR_BUILD_LDFLAGS) $(LIBIASS_SPECTATOR_IDLS_OBJ) $(LIBIASS_SPECTATOR_OBJ)





# ************     libiass objs rules     **************
./idls/iass_corba_rpc_typesC.o: ./idls/iass_corba_rpc_typesC.cpp
	$(CC) -c $^ -o $@

./idls/iass_corba_rpc_typesS.o: ./idls/iass_corba_rpc_typesS.cpp
	$(CC) -c $^ -o $@

./idls/iass_tunnel_idlC.o: ./idls/iass_tunnel_idlC.cpp
	$(CC) -c $^ -o $@

./idls/iass_tunnel_idlS.o: ./idls/iass_tunnel_idlS.cpp
	$(CC) -c $^ -o $@

iass_configs.o: iass_configs.cc
	$(CC) $(LIBXML2_CFLAGS) -c $^ -o $@

iass_configs_entry.o: iass_configs_entry.cc
	$(CC) -c $^ -o $@

iass_corba_orb_mngr.o: iass_corba_orb_mngr.cc
	$(CC) -c $^ -o $@

iass_entity.o: iass_entity.cc
	$(CC) -c $^ -o $@

iass_entity_ref.o: iass_entity_ref.cc
	$(CC) -c $^ -o $@

iass_filesystem.o: iass_filesystem.cc
	$(CC) $(LIBXML2_CFLAGS) -c $^ -o $@

iass_material.o: iass_material.cc
	$(CC) -c $^ -o $@

iass_material_instance.o: iass_material_instance.cc
	$(CC) -c $^ -o $@

iass_materials_store.o: iass_materials_store.cc
	$(CC) -c $^ -o $@

iass_ipcblock.o: iass_ipcblock.cc
	$(CC) -c $^ -o $@

iass_ipctunnel_client.o: iass_ipctunnel_client.cc
	$(CC) -c $^ -o $@

iass_ipctunnel.o: iass_ipctunnel.cc
	$(CC) -c $^ -o $@

iass_scene.o: iass_scene.cc
	$(CC) -c $^ -o $@

iass_scene_mngr.o: iass_scene_mngr.cc
	$(CC) -c $^ -o $@

iass_sharedfile_data.o: iass_sharedfile_data.cc
	$(CC) -c $^ -o $@

iass_tunnel_client.o: iass_tunnel_client.cc
	$(CC) -c $^ -o $@




# ********* libiass_scenes_server objs rules ***********
./idls/iass_scenes_server_idlS.o: ./idls/iass_scenes_server_idlS.cpp
	$(CC) -c $^ -o $@

./idls/iass_scenes_server_idlC.o: ./idls/iass_scenes_server_idlC.cpp
	$(CC) -c $^ -o $@

iass_contact_geometry_opal.o: iass_contact_geometry_opal.cc
	$(CC) -c $^ -o $@

iass_entity_factory_basictype_to_opal.o: iass_entity_factory_basictype_to_opal.cc
	$(CC) $(LIBXML2_CFLAGS) -c $^ -o $@

iass_server_scene_mngr.o: iass_server_scene_mngr.cc
	$(CC) $(LIBXML2_CFLAGS) -c $^ -o $@

iass_scene_opal.o: iass_scene_opal.cc
	$(CC) $(LIBXML2_CFLAGS) -c $^ -o $@

iass_server_scene.o: iass_server_scene.cc
	$(CC) -c $^ -o $@

iass_scene_spectators_mngr.o: iass_scene_spectators_mngr.cc
	$(CC) -c $^ -o $@

iass_scenes_server.o: iass_scenes_server.cc
	$(CC) $(LIBXML2_CFLAGS) -c $^ -o $@

iass_solid_brick_opal.o: iass_solid_brick_opal.cc
	$(CC) -c $^ -o $@

iass_solid_opal.o: iass_solid_opal.cc
	$(CC) -c $^ -o $@





# *********   libiass_spectator objs rules   ***********

# this one is already defined in libiass_scenes_server obj rules ...
#./idls/iass_scenes_server_idlC.o: ./idls/iass_scenes_server_idlC.cpp
#	$(CC) -c $^ -o $@

iass_client_spectator_mngr.o: iass_client_spectator_mngr.cc
	$(CC) $(LIBXML2_CFLAGS) -c $^ -o $@

iass_client_scene_mngr.o: iass_client_scene_mngr.cc
	$(CC) -c $^ -o $@




# ************            new             **************
.PHONY: new_libiass
    new_libiass: clean_libiass libiass

.PHONY: new_libiass_scenes_server
    new_libiass_scenes_server: clean_libiass_scenes_server libiass_scenes_server

.PHONY: new_libiass_spectator
    new_libiass_spectator: clean_libiass_spectator libiass_spectator

.PHONY: new
    new: clean idls all



# ************          idls gen          **************
.PHONY: idls
    idls: 
	tao_idl -o ./idls/ -I./idls ./idls/*.idl




# ************           install          **************
.PHONY: install
    install: install_libiass install_libiass_scenes_server install_libiass_spectator

.PHONY: install_libiass
    install_libiass: libiass
	# copy headers
	mkdir -p $(LIB_HEADERS_INSTALL_DIR)
	cp -a *.hh $(LIB_HEADERS_INSTALL_DIR)
	# copy idls skeletons
	mkdir -p $(LIB_IDLS_INSTALL_DIR)
	cp -a idls/iass_corba_rpc_types*.h $(LIB_IDLS_INSTALL_DIR)
	cp -a idls/iass_corba_rpc_types*.cpp $(LIB_IDLS_INSTALL_DIR)
	cp -a idls/iass_corba_rpc_types*.inl $(LIB_IDLS_INSTALL_DIR)
	cp -a idls/iass_tunnel_idl*.h $(LIB_IDLS_INSTALL_DIR)
	cp -a idls/iass_tunnel_idl*.cpp $(LIB_IDLS_INSTALL_DIR)
	cp -a idls/iass_tunnel_idl*.inl $(LIB_IDLS_INSTALL_DIR)
	# copy lib
	cp -a $(LIBIASS_NAME) $(LIB_OBJ_INSTALL_DIR)
	# update ld
	ldconfig


.PHONY: install_libiass_scenes_server
    install_libiass_scenes_server: libiass_scenes_server
	# copy idls skeletons
	mkdir -p $(LIB_IDLS_INSTALL_DIR)
	cp -a idls/iass_scenes_server_idl*.h $(LIB_IDLS_INSTALL_DIR)
	cp -a idls/iass_scenes_server_idl*.cpp $(LIB_IDLS_INSTALL_DIR)
	cp -a idls/iass_scenes_server_idl*.inl $(LIB_IDLS_INSTALL_DIR)
	# copy lib
	cp -a $(LIBIASS_SCENES_SERVER_NAME) $(LIB_OBJ_INSTALL_DIR)
	# update ld
	ldconfig

.PHONY: install_libiass_spectator
    install_libiass_spectator: libiass_spectator
	# copy lib
	cp -a $(LIBIASS_SPECTATOR_NAME) $(LIB_OBJ_INSTALL_DIR)
	# update ld
	ldconfig




# ************       clean and purge      **************
.PHONY: purge
    purge: 
	-rm -f *~
	-rm -f idls/*~

.PHONY: clean_idls
    clean_idls: purge
	-rm -f idls/iass_corba_rpc_types*.h
	-rm -f idls/iass_corba_rpc_types*.cpp
	-rm -f idls/iass_corba_rpc_types*.inl
	-rm -f idls/iass_corba_rpc_types*.o
	-rm -f idls/iass_tunnel_idl*.h
	-rm -f idls/iass_tunnel_idl*.cpp
	-rm -f idls/iass_tunnel_idl*.inl
	-rm -f idls/iass_tunnel_idl*.o
	-rm -f idls/iass_scenes_server_idl*.h
	-rm -f idls/iass_scenes_server_idl*.cpp
	-rm -f idls/iass_scenes_server_idl*.inl
	-rm -f idls/iass_scenes_server_idl*.o

.PHONY: clean_libiass
    clean_libiass: purge
	-rm -f $(LIBIASS_OBJ)
	-rm -f $(LIBIASS_NAME)

.PHONY: 
    clean_libiass_scenes_server: purge
	-rm -f $(LIBIASS_SCENES_SERVER_OBJ)
	-rm -f $(LIBIASS_SCENES_SERVER_NAME)

.PHONY: 
    clean_libiass_spectator: purge
	-rm -f $(LIBIASS_SPECTATOR_OBJ)
	-rm -f $(LIBIASS_SPECTATOR_IDLS_OBJ)
	-rm -f $(LIBIASS_SPECTATOR_NAME)

.PHONY: clean
    clean: clean_libiass clean_libiass_scenes_server clean_libiass_spectator

