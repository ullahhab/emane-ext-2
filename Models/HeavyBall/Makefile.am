lib_LTLIBRARIES = libHBeventscheduler.la

libHBeventscheduler_la_CPPFLAGS= \
 -I@top_srcdir@/include              \
 $(AM_CPPFLAGS)                      \
 $(libemane_CFLAGS)

libHBeventscheduler_la_LIBADD=   \
 $(libuuid_LIBS)                             \
 $(libxml2_LIBS)                             \
 @top_srcdir@/src/libemane/.libs/libemane.la \
 ../.libs/libHBbase.la

libHBeventscheduler_la_LDFLAGS= \
 $(AM_LDFLAGS)                       \
 -avoid-version

libHBeventscheduler_la_SOURCES = \
HBeventscheduler.cc		 \
eventscheduler.h		 \
type.h				 \

EXTRA_DIST=                          \
 hbmodel.xml.in                      \
 hbshimmodel.xml.in

BUILT_SOURCES =                      \
 hbshim.xml                          \
 hbmodel.xml

edit = sed                                \
       -e 's|@datadir[@]|$(pkgdatadir)|g'  

hbmodel.xml: hbmodel.xml.in
	$(edit) $< > $@

hbshim.xml: hbshim.xml.in
	$(edit) $< > $@

clean-local:
	rm -f $(BUILT_SOURCES)

install-exec-hook:	
	$(mkinstalldirs) $(DESTDIR)$(datadir)/$(PACKAGE)/xml/models/shim/heavyball/hbeventscheduler
	cp -f tdmanem.xml $(DESTDIR)$(datadir)/$(PACKAGE)/xml/models/shim/heavyball/hbeventscheduler
	cp -f tdmaradiomodel.xml $(DESTDIR)$(datadir)/$(PACKAGE)/xml/shim/heavyball/hbeventscheduler
	cp -f ../tdmabasemodelpcr.xml $(DESTDIR)$(datadir)/$(PACKAGE)/xml/models/shim/heavyball/hbeventscheduler

uninstall-local:
	rm -f $(DESTDIR)$(datadir)/$(PACKAGE)/xml/models/shim/heavyball/hbeventscheduler/hbmodel.xml
	rm -f $(DESTDIR)$(datadir)/$(PACKAGE)/xml/models/shim/heavyabll/hbeventscheduler/hbshim.xml
	rm -f $(DESTDIR)$(datadir)/$(PACKAGE)/xml/models/shim/heavyball/hbeventscheduler/hbbasemodelpcr.xml
