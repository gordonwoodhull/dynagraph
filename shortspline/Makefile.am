## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
        -I$(top_srcdir) \
	-I$(top_srcdir)/cdt \
        -I$(top_srcdir)/pathplan \
	-I$(top_srcdir)/dynagraph

noinst_HEADERS = legal_arrangement.h shortspline.h simple.h
if ENABLE_DYNAGRAPH
noinst_LTLIBRARIES = libshortspline.la
endif

libshortspline_la_SOURCES = legal_arrangement.c shortspline.cpp

EXTRA_DIST = $(libshortspline_la_SOURCES)
