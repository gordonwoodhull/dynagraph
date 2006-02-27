## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_srcdir)/cdt \
        -I$(top_srcdir)/pathplan \
	-I$(top_srcdir)/dynagraph

LIBS =

noinst_HEADERS = legal_arrangement.h shortspline.h simple.h

noinst_LTLIBRARIES = libshortspline.la

libshortspline_la_SOURCES = legal_arrangement.c shortspline.cpp \
	${noinst_HEADERS}

EXTRA_DIST =
