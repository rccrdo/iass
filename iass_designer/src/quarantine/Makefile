SHELL = /bin/sh
CC = g++ -g -O0 -fno-inline -Wall

srcdir = .

LIBXML2_CFLAGS = -I/usr/include/libxml2
LIBXML2_LIBS = -L/usr/lib -lxml2

LIBIASS_CFLAGS = -I/usr/local/include/iass/
LIBIASS_LIBS = -liass

LIBGTK_CFLAGS = -I/usr/include/gtkmm-2.4 -I/usr/lib/gtkmm-2.4/include -I/usr/include/glibmm-2.4 -I/usr/lib/glibmm-2.4/include -I/usr/include/gdkmm-2.4 -I/usr/lib/gdkmm-2.4/include -I/usr/include/pangomm-1.4 -I/usr/include/atkmm-1.6 -I/usr/include/gtk-2.0 -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/lib/gtk-2.0/include -I/usr/include/cairomm-1.0 -I/usr/include/pango-1.0 -I/usr/include/cairo -I/usr/include/freetype2 -I/usr/include/libpng12 -I/usr/include/atk-1.0 

LIBGTK_LIBS = -lgtkmm-2.4 -lgdkmm-2.4 -latkmm-1.6 -lgtk-x11-2.0 -lpangomm-1.4 -lcairomm-1.0 -lglibmm-2.4 -lsigc-2.0 -lgdk-x11-2.0 -latk-1.0 -lgdk_pixbuf-2.0 -lm -lpangocairo-1.0 -lfontconfig -lXext -lXrender -lXinerama -lXi -lXrandr -lXcursor -lXcomposite -lXdamage -lpango-1.0 -lcairo -lX11 -lXfixes -lgobject-2.0 -lgmodule-2.0 -ldl -lglib-2.0

#  pkg-config libglademm-2.4 --cflags
LIBGLADE_CFLAGS = -I/usr/include/libglademm-2.4 -I/usr/lib/libglademm-2.4/include -I/usr/include/gtkmm-2.4 -I/usr/lib/gtkmm-2.4/include -I/usr/include/libglade-2.0 -I/usr/include/glibmm-2.4 -I/usr/lib/glibmm-2.4/include -I/usr/include/gdkmm-2.4 -I/usr/lib/gdkmm-2.4/include -I/usr/include/pangomm-1.4 -I/usr/include/atkmm-1.6 -I/usr/include/gtk-2.0 -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/lib/gtk-2.0/include -I/usr/include/cairomm-1.0 -I/usr/include/pango-1.0 -I/usr/include/cairo -I/usr/include/freetype2 -I/usr/include/libpng12 -I/usr/include/atk-1.0 -I/usr/include/libxml2

#  pkg-config libglademm-2.4 --libs
LIBGLADE_LIBS = -lglademm-2.4 -lgtkmm-2.4 -lglade-2.0 -lgdkmm-2.4 -latkmm-1.6 -lpangomm-1.4 -lcairomm-1.0 -lglibmm-2.4 -lsigc-2.0 -lgtk-x11-2.0 -lxml2 -lgdk-x11-2.0 -latk-1.0 -lgdk_pixbuf-2.0 -lm -lpangocairo-1.0 -lfontconfig -lXext -lXrender -lXinerama -lXi -lXrandr -lXcursor -lXcomposite -lXdamage -lpango-1.0 -lcairo -lX11 -lXfixes -lgobject-2.0 -lgmodule-2.0 -ldl -lglib-2.0 

LIBOGRE_CFLAGS = -I/usr/include/OGRE
LIBOGRE_LIBS =  -lOgreMain

CFLAGS += $(LIBIASS_CFLAGS)
CFLAGS += $(LIBGTK_CFLAGS)
CFLAGS += $(LIBGLADE_CFLAGS)
CFLAGS += $(LIBOGRE_CFLAGS)

LDFLAGS += $(LIBIASS_LIBS)
LDFLAGS += $(LIBGTK_LIBS)
LDFLAGS += $(LIBGLADE_LIBS)
LDFLAGS += $(LIBOGRE_LIBS)

APPNAME = iass_designer


APPSRC = app_iass_designer.cc iass_designer_manager.cc  iass_designer_main_window.cc \
          iass_designer_orb_mngr.cc iass_renderer.cc iass_ts_renderer_queue.cc

APPOBJ = app_iass_designer.o iass_designer_manager.o iass_designer_main_window.o \
          iass_designer_orb_mngr.o iass_renderer.o iass_ts_renderer_queue.o

.PHONY: app
    app: purge $(APPOBJ)
	$(CC) -o $(APPNAME) $(LDFLAGS) $(APPOBJ)
	mv $(APPNAME) ../

.PHONY: new
    new: clean app



app_iass_designer.o: app_iass_designer.cc
	$(CC) $(LIBXML2_CFLAGS) $(LIBOGRE_CFLAGS) $(LIBGLADE_CFLAGS) $(LIBIASS_CFLAGS) -c $^ -o $@

iass_designer_manager.o: iass_designer_manager.cc
	$(CC) $(LIBXML2_CFLAGS) $(LIBOGRE_CFLAGS) $(LIBGLADE_CFLAGS) $(LIBIASS_CFLAGS) -c $^ -o $@

iass_designer_main_window.o: iass_designer_main_window.cc
	$(CC) $(LIBXML2_CFLAGS) $(LIBGLADE_CFLAGS) $(LIBIASS_CFLAGS) -c $^ -o $@

iass_designer_orb_mngr.o: iass_designer_orb_mngr.cc
	$(CC) $(LIBXML2_CFLAGS) $(LIBIASS_CFLAGS) -c $^ -o $@

iass_renderer.o: iass_renderer.cc
	$(CC) $(LIBXML2_CFLAGS) $(LIBOGRE_CFLAGS) $(LIBIASS_CFLAGS) -c $^ -o $@

iass_ts_renderer_queue.o: iass_ts_renderer_queue.cc
	$(CC) $(LIBXML2_CFLAGS) $(LIBIASS_CFLAGS) -c $^ -o $@


.PHONY: purge
    purge: 
	-rm -f *~

.PHONY: clean
    clean: purge
	-rm -f *o
	-rm -f $(APPNAME)

