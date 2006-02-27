## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_srcdir)/dynagraph \
	-I$(top_builddir)/dynagraph \
	-I$(top_srcdir)/agraph \
	-I$(top_srcdir)/cdt

LIBS =

noinst_HEADERS = createLayoutServer.h DynaView.h incrcmds.h incrglobs.h incrout.h incrxep.h

noinst_LTLIBRARIES = libincrface.la

libincrface_la_SOURCES = createLayoutServer.cpp incrcmds.cpp incrout.cpp \
	incrgram.cpp incrscan.cpp DynaView.cpp ${noinst_HEADERS}

incrscan.o incrscan.lo : incrscan.cpp incrgram.h

incrgram.o incrgram.lo : incrgram.cpp incrgram.h

incrscan.cpp: ${top_srcdir}/dynagraph/incrface/incrscan.lpp

incrgram.h : incrgram.cpp

incrgram.cpp : ${top_srcdir}/dynagraph/incrface/incrgram.ypp

SUFFIXES = .cpp .ypp .lpp

.lpp.cpp:
	@LEX@ $<
	@SED@ "s/yy/incr_yy/g" < @LEX_OUTPUT_ROOT@.c > $@
	rm @LEX_OUTPUT_ROOT@.c

.ypp.cpp:
	@YACC@ -dv $<
	@SED@ "s/yy/incr_yy/g" < y.tab.c > $@
	@SED@ "s/yy/incr_yy/g" < y.tab.h > $*.h
	rm y.tab.c y.tab.h

DISTCLEANFILES = y.output incrgram.cpp incrgram.h incrscan.cpp

EXTRA_DIST = incrgram.cpp incrgram.h incrscan.cpp incrgram.ypp incrscan.lpp
