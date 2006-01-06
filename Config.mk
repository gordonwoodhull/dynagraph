# configuration for good old make.
# select an architecture from makearch/*
# ...one of:  sgi.mips3 sgi.mips2 sol.sun4 sol6.sun4 sun4 osx linux.i386
ARCH=linux.i386

# specify root directory for installation
INSTALLROOT=/usr/local/dynagraph

#specify where to find graphviz 
GRAPHVIZ_INC = /usr/local/graphviz/include
GRAPHVIZ_LIB = /usr/local/graphviz/lib

#specify where to find boost
BOOST_INC = /usr/local/include

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
