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


#include <stdio.h>
#include <stdarg.h>
#include "Geometry.h"
#include "time-o-matic.h"

#ifdef _WIN32
#include <windows.h>
#endif

namespace Dynagraph {

Timer timer;

Timer::Timer() {
	Start();
}
double Timer::currentTime() {
#ifdef _WIN32
	LARGE_INTEGER curr;
	dgcheck(QueryPerformanceCounter(&curr));
	return double(curr.QuadPart-m_start)/double(m_frequency);
#else
	return 0;
#endif
}
double Timer::Start() {
#ifdef _WIN32
	dgcheck(QueryPerformanceFrequency((PLARGE_INTEGER)&m_frequency));
	dgcheck(QueryPerformanceCounter((PLARGE_INTEGER)&m_start));
#endif
	return m_last = currentTime();
}
char *m_timeSinceLast = "%.4f (+%.4f): %s",
	*m_timeElapsed = "%.4f: %.4f elapsed since %s";
double Timer::display(dgr::reportType rt,double last,char *format,char *s,va_list va) {
	char prebuf[2000],buf2[2000];
	double t = currentTime();
	sprintf(prebuf,format,t,t-last,s,prebuf);
	vsprintf(buf2,prebuf,va);
	reports[rt] << buf2;
	return t;
}
double Timer::Now(dgr::reportType rt,char *s,...) {
	va_list va;
	va_start(va,s);
	return m_last = display(rt,m_last,m_timeSinceLast,s,va);
}
double Timer::Elapsed(dgr::reportType rt,double start,char *s,...) {
	va_list va;
	va_start(va,s);
	return display(rt,start,m_timeElapsed,s,va);
}
void Timer::LoopPoint(dgr::reportType rt,char *s) {
	double t = currentTime(),
		dt;
	if(unsigned(rt)>=m_looplasts.size())
		m_looplasts.resize(rt+1,0.0);
	if(m_looplasts[rt]==0.0)
		dt = 0.0;
	else
		dt = t-m_looplasts[rt];
	loops.Field(rt,s,dt);
	m_looplasts[rt] = t;
}

} // namespace Dynagraph
