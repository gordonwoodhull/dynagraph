## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
        -I$(top_srcdir) \
	-I$(top_srcdir)/cdt \
	-I$(top_srcdir)/dynagraph

noinst_HEADERS = edges.h hedges.h info.h site.h voronoi.h 
if ENABLE_DYNAGRAPH
noinst_LTLIBRARIES = libvoronoi.la
endif

libvoronoi_la_SOURCES = edges.cpp heap.cpp hedges.cpp info.cpp \
	process.cpp site.cpp voronoi.cpp

EXTRA_DIST = $(libvoronoi_la_SOURCES)
