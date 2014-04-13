SHELL = /bin/sh
CC = g++ -g -O0 -fno-inline -Wall

LIBIASS_SCENES_SERVER_CFLAGS = -I/usr/local/include/iass/ -I/usr/local/include/iass/idls/
LIBIASS_SCENES_SERVER_LDFLAGS = -L/usr/local/lib -liass_scenes_server

LIBXML2_CFLAGS = -I/usr/include/libxml2
LIBXML2_LIBS = -L/usr/lib -lxml2

LIBTAO_LDFLAGS = -lTAO_CosNaming

LIBGTK_CFLAGS = -I/usr/include/gtkmm-2.4 -I/usr/lib/gtkmm-2.4/include -I/usr/include/glibmm-2.4 -I/usr/lib/glibmm-2.4/include -I/usr/include/gdkmm-2.4 -I/usr/lib/gdkmm-2.4/include -I/usr/include/pangomm-1.4 -I/usr/include/atkmm-1.6 -I/usr/include/gtk-2.0 -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/lib/gtk-2.0/include -I/usr/include/cairomm-1.0 -I/usr/include/pango-1.0 -I/usr/include/cairo -I/usr/include/freetype2 -I/usr/include/libpng12 -I/usr/include/atk-1.0 

LIBGTK_LIBS = -lgtkmm-2.4

#  pkg-config libglademm-2.4 --cflags
LIBGLADE_CFLAGS = -I/usr/include/libglademm-2.4 -I/usr/lib/libglademm-2.4/include -I/usr/include/gtkmm-2.4 -I/usr/lib/gtkmm-2.4/include -I/usr/include/libglade-2.0 -I/usr/include/glibmm-2.4 -I/usr/lib/glibmm-2.4/include -I/usr/include/gdkmm-2.4 -I/usr/lib/gdkmm-2.4/include -I/usr/include/pangomm-1.4 -I/usr/include/atkmm-1.6 -I/usr/include/gtk-2.0 -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/lib/gtk-2.0/include -I/usr/include/cairomm-1.0 -I/usr/include/pango-1.0 -I/usr/include/cairo -I/usr/include/freetype2 -I/usr/include/libpng12 -I/usr/include/atk-1.0 -I/usr/include/libxml2

#  pkg-config libglademm-2.4 --libs
LIBGLADE_LIBS = -lglademm-2.4





# ************       app misc stuff       **************
APPNAME = iass_scenes_server

APPOBJ = app_iass_scenes_server.o iass_scenes_server_main_window.o 

LDFLAGS += $(LIBIASS_SCENES_SERVER_LDFLAGS)
LDFLAGS += $(LIBGTK_LIBS)
LDFLAGS += $(LIBGLADE_LIBS)
LDFLAGS += $(LIBTAO_LDFLAGS)



# ************          make app          **************
.PHONY: app
    app: purge $(IDLS_OBJ) $(APPOBJ)
	$(CC) -o $(APPNAME) $(LDFLAGS) $(IDLS_OBJ) $(APPOBJ)
	mv $(APPNAME) ../




# ************       app  objs rules      **************
app_iass_scenes_server.o: app_iass_scenes_server.cc
	$(CC) $(LIBXML2_CFLAGS) $(LIBGTK_CFLAGS) $(LIBGLADE_CFLAGS) $(LIBIASS_SCENES_SERVER_CFLAGS) -c $^ -o $@

iass_scenes_server_main_window.o: iass_scenes_server_main_window.cc
	$(CC) $(LIBXML2_CFLAGS) $(LIBGTK_CFLAGS) $(LIBGLADE_CFLAGS) $(LIBIASS_SCENES_SERVER_CFLAGS) -c $^ -o $@





# ************            new             **************
.PHONY: new
    new: clean app



# ************       clean and purge      **************
.PHONY: purge
    purge: 
	-rm -f *~

.PHONY: clean
    clean: purge
	-rm -f $(APPOBJ)
	-rm -f $(APPNAME)

