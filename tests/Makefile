SHELL = /bin/sh
CC = g++ -g -O0 -fno-inline -Wall

srcdir = .

LIBXML2_CFLAGS = -I/usr/include/libxml2
LIBXML2_LIBS = -L/usr/lib -lxml2

LIBIASS_CFLAGS = -I/usr/local/include/iass
LIBIASS_LIBS = -liass


CFLAGS += $(LIBXML2_CFLAGS)
CFLAGS += $(LIBIASS_CFLAGS)

LDFLAGS += $(LIBIASS_LIBS)

TEST_QUEUE = test_iass_cstring

.PHONY: tests
    tests: purge $(TEST_QUEUE)

.PHONY: new
    new: clean tests


test_iass_cstring: test_iass_cstring.cc
	$(CC) $^ -o $@

test_iass_collection: test_iass_collection.cc
	$(CC)  $^ -o $@   

test_iass_filesystem: test_iass_filesystem.cc
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@   

test_iass_loop_sleeper: test_iass_loop_sleeper.cc
	$(CC) $^ -o $@   

test_iass_orb_basic_manager: test_iass_orb_basic_manager.cc
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@   

test_iass_packet: test_iass_packet.cc
	$(CC) $^ -o $@   

test_iass_priority_queue: test_iass_priority_queue.cc
	$(CC) $^ -o $@   

test_iass_queue: test_iass_queue.cc
	$(CC) $^ -o $@   

test_iass_queue_ts: test_iass_queue_ts.cc
	$(CC) $(LIBIASS_LIBS) $^ -o $@   

test_iass_scene: test_iass_scene.cc
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@   

test_iass_ipc_block: test_iass_ipc_block.cc
	$(CC) $(LDFLAGS) $^ -o $@   

test_iass_serialize_helpers: test_iass_serialize_helpers.cc
	$(CC) $^ -o $@   

test_iass_entity_ref: test_iass_entity_ref.cc
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@   

test_obj_sizes: test_obj_sizes.cc
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@   

test_iass_vector: test_iass_vector.cc
	$(CC) $(CFLAGS) $^ -o $@   



.PHONY: purge
    purge: 
	-rm -f *~

.PHONY: clean
    clean: purge
	-rm -f $(TEST_QUEUE)


