## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_srcdir)/cdt \
	-I$(top_srcdir)/agraph \
	-I$(top_srcdir)/dynagraph \
	-I$(top_builddir)/dynagraph

LIBS = @LIBS@

noinst_HEADERS = Pattern.h Search.h gscmds.h gsxep.h gsgram.h

bin_PROGRAMS = graphsearch

graphsearch_SOURCES = graphsearch.cpp Pattern.cpp Search.cpp gscmds.cpp \
	gsscan.cpp gsgram.cpp ${noinst_HEADERS}

graphsearch_LDADD = ${top_builddir}/dynagraph/common/libdynagraph.la \
	${top_builddir}/agraph/libagraph.la \
	${top_builddir}/cdt/libcdt.la

gsscan.cpp: ${top_srcdir}/dynagraph/graphsearch/gsscan.lpp gsgram.h

gsgram.h : gsgram.cpp

gsgram.cpp : ${top_srcdir}/dynagraph/graphsearch/gsgram.ypp

SUFFIXES = .cpp .lpp .ypp

.lpp.cpp:
	@LEX@ $<
	@SED@ "s/yy/gs_yy/g" < @LEX_OUTPUT_ROOT@.c > $@
	rm @LEX_OUTPUT_ROOT@.c

.ypp.cpp:
	@YACC@ -dv $<
	@SED@ "s/yy/gs_yy/g" < y.tab.c > $@
	@SED@ "s/yy/gs_yy/g" < y.tab.h > $*.h
	rm y.tab.c y.tab.h

DISTCLEANFILES = y.output gsscan.cpp gsgram.cpp gsgram.h

EXTRA_DIST = Collapse.cpp gsscan.cpp gsgram.cpp gsgram.h gsscan.lpp gsgram.ypp
