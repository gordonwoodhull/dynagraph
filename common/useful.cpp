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
#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include "Geometry.h"
#include <vector>

using namespace std;


std::vector<FILE*> g_files;
bool g_shush = false;
static bool gotIt(int rt) {
	return ! (rt<0 || unsigned(rt)>=g_files.size() || !g_files[rt]);
}
void enableReport(int rt,FILE *f) {
	if(unsigned(rt)>=g_files.size())
		g_files.resize(rt+1,0);
	g_files[rt] = f;
}
bool reportEnabled(int rt) {
	return !g_shush && gotIt(rt);
}
void vreport(int rt, char *msg,va_list va) {
	if(!gotIt(rt))
		return;
#ifdef REPORT_WIN32DEBUG
	char buf[1024];
	vsprintf(buf,msg,va);
	_RPT0(_CRT_WARN,buf);
#else
	FILE *f = g_files[rt];
	vfprintf(f,msg,va);
#endif
}
void report(int rt, char *msg,...) {
	va_list va;
	va_start(va,msg);
	vreport(rt,msg,va);
}
void shush(bool whether) {
	g_shush = whether;
}
LoopMinder loops;
void LoopMinder::Start(int rt) {
	if(reportEnabled(rt)) {
		FieldSet &f = m_fieldSets[g_files[rt]];
		if(f.state==FieldSet::loop)
			return; // loop already started
		if(f.state==FieldSet::start) {
			f.state = FieldSet::first;
			f.names.clear();
		}
		if(f.state==FieldSet::done)
			f.state = FieldSet::loop;
		f.data.clear();
	}
}
void LoopMinder::doField(FieldSet &f,char *colname,double val) {
	if(f.state==FieldSet::first)
		f.names.push_back(colname);
#ifdef _DEBUG
	else {
		int i = f.data.size();
		string &s = f.names[i];
		assert(s==colname);
	}
#endif
	f.data.push_back(val);
}
void LoopMinder::Field(int rt,char *colname,double val) {
	if(rt==-1)
		for(FieldSets::iterator fi = m_fieldSets.begin(); fi!=m_fieldSets.end(); ++fi)
			doField(fi->second,colname,val);
	else if(reportEnabled(rt))
		doField(m_fieldSets[g_files[rt]],colname,val);
}
void LoopMinder::Finish(int rt) {
	if(reportEnabled(rt)) {
		FieldSet &f = m_fieldSets[g_files[rt]];
		if(f.state==FieldSet::done)
			return;
		if(f.state==FieldSet::first) {
			vector<string>::iterator si;
			for(si = f.names.begin(); si!=f.names.end(); ++si)
				if(si!=f.names.end()-1)
					report(rt,"\"%s\"%c",si->c_str(),sep);
				else
					report(rt,"\"%s\"\n",si->c_str());
		}
		vector<double>::iterator di;
		for(di = f.data.begin(); di!=f.data.end(); ++di)
			if(di!=f.data.end()-1)
				report(rt,"%.4f%c",*di,sep);
			else
				report(rt,"%.4f\n",*di);
		f.state = FieldSet::done;
	}
}
void LoopMinder::Cancel() {
	for(FieldSets::iterator fi = m_fieldSets.begin(); fi!=m_fieldSets.end(); ++fi)
		fi->second.state = FieldSet::done;
}
