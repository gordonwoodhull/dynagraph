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

#ifdef _WIN32
#include <windows.h>
#endif

Timer timer;

Timer::Timer() {
	Start();
}
double Timer::currentTime() {
#ifdef _WIN32
	LARGE_INTEGER curr;
	check(QueryPerformanceCounter(&curr));
	return double(curr.QuadPart-m_start)/double(m_frequency);
#else
	return 0;
#endif
}
double Timer::Start() {
#ifdef _WIN32
	check(QueryPerformanceFrequency((PLARGE_INTEGER)&m_frequency));
	check(QueryPerformanceCounter((PLARGE_INTEGER)&m_start));
#endif
	return m_last = currentTime();
}
char *m_timeSinceLast = "%.4f (+%.4f): %s",
	*m_timeElapsed = "%.4f: %.4f elapsed since %s";
double Timer::display(int rt,double last,char *format,char *s,va_list va) {
	char prebuf[2000];
	double t = currentTime();
	sprintf(prebuf,format,t,t-last,s,prebuf);
	vreport(rt,prebuf,va);
	return t;
}
double Timer::Now(int rt,char *s,...) {
	va_list va;
	va_start(va,s);
	return m_last = display(rt,m_last,m_timeSinceLast,s,va);
}
double Timer::Elapsed(int rt,double start,char *s,...) {
	va_list va;
	va_start(va,s);
	return display(rt,start,m_timeElapsed,s,va);
}
void Timer::LoopPoint(int rt,char *s) {
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
/*
void Timer::StartLoop(int rt,char *name) {
	if(!reportEnabled(rt))
		return;
	Loop &loop = m_loops[rt];
	if(loop.name!=name) {
		loop.name = name;
		loop.points.clear();
		loop.recording = true;
		loop.last = currentTime();
	}
	loop.data.clear();
}
void Timer::Point(int rt,char *name) {
	if(!reportEnabled(rt))
		return;
	Loop &loop = m_loops[rt];
	if(loop.recording)
		loop.points.push_back(name);
	double ct = currentTime();
	loop.data.push_back(ct-loop.last);
	loop.last = ct;
}

void Timer::EndLoop(int rt) {
	if(!reportEnabled(rt))
		return;
	Loop &loop = m_loops[rt];
	if(loop.recording) {
		vector<string>::iterator si;
		for(si = loop.points.begin(); si!=loop.points.end(); ++si)
			report(rt,"%s ",si->c_str());
		report(rt,"\n");
		loop.recording = false;
	}
	double tot = 0;
	vector<double>::iterator di;
	for(di = loop.data.begin(); di!=loop.data.end(); ++di)
		report(rt,"%.4f ",*di), tot += *di;
	report(rt,"\n");
}
*/
