## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_srcdir)/cdt \
	-I$(top_srcdir)/dynagraph

LIBS =

noinst_HEADERS = edges.h hedges.h info.h site.h voronoi.h 

noinst_LTLIBRARIES = libvoronoi.la

libvoronoi_la_SOURCES = edges.cpp heap.cpp hedges.cpp info.cpp \
	process.cpp site.cpp voronoi.cpp ${noinst_HEADERS}

EXTRA_DIST =
