all: libcgraph.a

ROOT=..
include $(ROOT)/Config.mk
include $(ROOT)/makearch/$(ARCH)

INCS =	-I.. -I../.. -I../cdt \
	-I$(GRAPHVIZ_INC) \
	-I$(BOOST_INC)

DEFINES = -DHAVE_CONFIG_H

OBJS = agerror.o	flatten.o	io.o		rec.o		write.o \
	agxbuf.o	grammar.o	mem.o		refstr.o \
	apply.o	graph.o	node.o		scan.o \
	attr.o		id.o		obj.o		subg.o \
	edge.o		imap.o		pend.o		utils.o

libcgraph.a : $(OBJS)
	$(AR) cr libcgraph.a $(OBJS)
	$(RANLIB) libcgraph.a

install: libcgraph.a
#	$(MKPATH) $(LIBDIR)
#	$(INSTALL) libcgraph.a $(LIBDIR)

clean:
	$(RM) *.o core

distclean: clean
	$(RM) *.a lib*.so.*
