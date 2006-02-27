## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_srcdir)/cdt \
	-I$(top_srcdir)/dynagraph \
	-I$(top_srcdir)/agraph

LIBS =

noinst_HEADERS = FVSCombo.h fdp.h grid.h macros.h

noinst_LTLIBRARIES = libfdp.la

libfdp_la_SOURCES = grid.cpp layout.cpp process.cpp ${noinst_HEADERS}

EXTRA_DIST =
