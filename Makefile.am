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



//TODO: check for all the libraries that we need


libHBbase_la_LDFLAGS=             \
$(AM_LDFLAGS)                     \
-avoid-version                    

//TODO: check for all the libraries that we need

libHBbase_la_SOURCES =            \
HBQueue.cc                        \
HBQueue.h                         \
HBbasemodelmessage.h              \
HBbasemodelmessage.inl            \
HBbasicqueuemanager.h             \
HBmodel.h                         \
HBmodel.cc                        \
HBmodelimpl.h                     \
HBmodelimpl.cc                    \
HBqueuemanager.h                  \
HBscheduleEvent.h                 \
HBscheduleuser.h                  \
HeavyBall.h                       \
HeavyBall.inl                     \
HeavyballshimLayerimpl.h          \
HeavyballshimLayerimpl.inl        \
messageComponent.h                \
messageComponent.inl              \
packerstatuspublisher.h           \
packetstatuspublisherimpl.h       \
pathlossholder.h                  \



nodit_libHBbase_la_SOURCES =      \
//TODO: some files that we need for 


BUILT_SOURCES =                   \
$(nodist_libHBbase_la_SOURCES)


clean-local:
      rm-f $(BUILT_SOURCES)
      
      
hbbasemodelmessage.pb.cc hbbasemodelmessage.pb.h:  hbbasemodelmessage.proto
           protoc -I=. --cpp_out=. $<

