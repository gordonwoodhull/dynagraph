all: libcdt.a

ROOT=..
include $(ROOT)/Config.mk
include $(ROOT)/makearch/$(ARCH)

INCS =	-I.. -I../.. \
	-I$(GRAPHVIZ_INC) \
	-I$(BOOST_INC)

DEFINES = -DHAVE_CONFIG_H

OBJS = dtclose.o	dthash.o	dtrenew.o	dtstrhash.o \
	dtdisc.o	dtlist.o	dtrestore.o	dttree.o \
	dtextract.o	dtmethod.o	dtsize.o	dtview.o \
	dtflatten.o	dtopen.o	dtstat.o	dtwalk.o



libcdt.a : $(OBJS)
	$(AR) cr libcdt.a $(OBJS)
	$(RANLIB) libcdt.a

install: libcdt.a
#	$(MKPATH) $(LIBDIR)
#	$(INSTALL) libcdt.a $(LIBDIR)

clean:
	$(RM) *.o core

distclean: clean
	$(RM) *.a lib*.so.*
