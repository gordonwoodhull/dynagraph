# configuration for good old make.
# select an architecture from makearch/*
# ...one of:  sgi.mips3 sgi.mips2 sol.sun4 sol6.sun4 sun4 osx linux.i386
ARCH=linux.x86_64

# specify root directory for installation
INSTALLROOT=/usr/local

# compile options
CLANGOPTS=-Wno-logical-op-parentheses -Wno-dangling-else --std=c++20
GCCOPTS=-Wno-parentheses -Wno-dangling-else

# specify where to find graphviz
GRAPHVIZ_INC = /usr/local/include/graphviz
GRAPHVIZ_LIB = /usr/local/lib

# specify where to find boost
BOOST_INC = /usr/local/include

# automatic dependencies
DEPDIR = .deps
df = $(DEPDIR)/$(*F)
.c.o:
	@$(MAKEDEPEND); \
	cp $(df).d $(df).P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	  -e '/^$$/ d' -e 's/$$/ :/' < $(df).d >> $(df).P; \
	rm -f $(df).d
	$(CCCOMMAND)

.cpp.o:
	@$(MAKEDEPEND); \
	cp $(df).d $(df).P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	  -e '/^$$/ d' -e 's/$$/ :/' < $(df).d >> $(df).P; \
	rm -f $(df).d
	$(CPPCOMMAND)


###################################################
#  Typically, nothing below should be changed.    #
###################################################

BINDIR=$(INSTALLROOT)/bin
LIBDIR=$(INSTALLROOT)/lib
INCDIR=$(INSTALLROOT)/include
XPLDIR=$(INSTALLROOT)/examples
DOCDIR=$(INSTALLROOT)/doc

MANDIR=$(INSTALLROOT)/man/man1
LIBMANDIR=$(INSTALLROOT)/man/man3
PKGMANDIR=$(INSTALLROOT)/man/mann

# postscipt and html docs
DOCDIR=$(INSTALLROOT)/doc

VERSION=1.0
