all: libpathplan.a

ROOT=..
include $(ROOT)/Config.mk
include $(ROOT)/makearch/$(ARCH)

INCS =	-I.. -I../.. \
	-I$(GRAPHVIZ_INC) \
	-I$(BOOST_INC)

DEFINES = -DHAVE_CONFIG_H

OBJS = cvt.o		route.o	shortestpth.o	triang.o	visibility.o \
	inpoly.o	shortest.o	solvers.o	util.o


libpathplan.a : $(OBJS)
	$(AR) cr libpathplan.a $(OBJS)
	$(RANLIB) libpathplan.a

install: libpathplan.a
#	$(MKPATH) $(LIBDIR)
#	$(INSTALL) libpathplan.a $(LIBDIR)

clean:
	$(RM) *.o core

distclean: clean
	$(RM) *.a lib*.so.*
