/**********************************************************
*      This software is part of the graphviz toolset      *
*                http://www.graphviz.org/                 *
*                                                         *
*            Copyright (c) 1994-2005 AT&T Corp.           *
*                and is licensed under the                *
*            Common Public License, Version 1.0           *
*                      by AT&T Corp.                      *
*                                                         *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
*                                                         *
*               This version available from               *
*                   http://dynagraph.org                  *
**********************************************************/


#include <string>
#include <vector>
#include <map>
#include <stdarg.h>

struct Timer {
	double Start();
	double Elapsed(int rt,double start,char *s,...);
	double Now(int rt,char *s,...);

	void LoopPoint(int rt,char *s);
	Timer();
private:
	double currentTime();
	double display(int rt,double last,char *format,char *s,va_list va);
#ifdef _WIN32
	__int64 m_frequency,m_start;
#endif
	double m_last;
	std::vector<double> m_looplasts;
};
extern Timer timer;
