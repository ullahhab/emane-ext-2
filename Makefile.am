SUBDIRS=                        \
.                               \
Models/HeavyBall                



lib_LTLIBRARIES = libHBbase.la

libtdmabase_la_CPPFLAGS=             \
 -I@top_srcdir@/include              \
 $(AM_CPPFLAGS)                      \
 $(libemane_CFLAGS)



libtdmabase_la_LIBADD=                       \
 $(libuuid_LIBS)                             \
 $(libxml2_LIBS)                             \
 $(protobuf_LIBS)                            \
 @top_srcdir@/src/libemane/.libs/libemane.la






libHBbase_la_LDFLAGS=             \
$(AM_LDFLAGS)                     \
-avoid-version                    


libHBbase_la_SOURCES =            \
HBQueue.cc                        \
HBQueue.h                         \
HBbasemodelmessage.h              \
HBbasemodelmessage.cc             \
HBbasemodelmessage.inl            \
HBbasicqueuemanager.h             \
HBmodel.h                         \
HBmodel.cc                        \
HBmodelimpl.h                     \
HBmodelimpl.cc                    \
HBqueuemanager.h                  \
HBscheduleEvent.h                 \
HBscheduleEvent.cc                \
HBscheduleuser.h                  \
HBscheduleevent.py                \
HeavyBall.h                       \
HeavyBall.inl                     \
HeavyballshimLayerimpl.h          \
hbschedule.py                     \
HeavyballshimLayerimpl.inl        \
messageComponent.h                \
messageComponent.inl              \
packerstatuspublisher.h           \
packetstatuspublisherimpl.h       \
pathlossholder.h                  \
schedule.h



nodit_libHBbase_la_SOURCES =      \
hbmodelmessage.proto.cc
hbmodelmessage.proto.h


BUILT_SOURCES =                   \
$(nodist_libHBbase_la_SOURCES)

EXTRA_DIST=                          \
 hbmodelmodelmessage.proto          \
 hbmodelpcr.xml

clean-local:
      rm-f $(BUILT_SOURCES)
      
      
hbbasemodelmessage.pb.cc hbbasemodelmessage.pb.h:  hbbasemodelmessage.proto
           protoc -I=. --cpp_out=. $<

