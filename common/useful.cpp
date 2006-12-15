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
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/null.hpp>

using namespace std;

namespace Dynagraph {

dgr reports;

std::ostream &dgr::null_stream() {
	typedef boost::iostreams::basic_null_sink<char> null_sink_t;
	typedef boost::iostreams::stream<null_sink_t> null_stream_t;
	static null_sink_t *s_absorb=0;
	static null_stream_t *s_null_output_stream=0;
	if(!s_null_output_stream) {
		s_absorb = new null_sink_t;
		s_null_output_stream = new null_stream_t(*s_absorb);
	}
	return *s_null_output_stream;
}
LoopMinder loops;
void LoopMinder::Start(dgr::reportType rt) {
	if(reports.enabled(rt)) {
		FieldSet &f = getFieldSet(rt);
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
		dgassert(s==colname);
	}
#endif
	f.data.push_back(val);
}
void LoopMinder::Field(dgr::reportType rt,char *colname,double val) {
	if(rt==-1)
		for(FieldSets::iterator fi = m_fieldSets.begin(); fi!=m_fieldSets.end(); ++fi)
			doField(fi->second,colname,val);
	else if(reports.enabled(rt))
		doField(getFieldSet(rt),colname,val);
}
void LoopMinder::Finish(dgr::reportType rt) {
	if(reports.enabled(rt)) {
		FieldSet &f = getFieldSet(rt);
		if(f.state==FieldSet::done)
			return;
		if(f.state==FieldSet::first) {
			vector<string>::iterator si;
			for(si = f.names.begin(); si!=f.names.end(); ++si)
				if(si!=f.names.end()-1)
					reports[rt] << '"' << si->c_str() << '"' << sep;
				else
					reports[rt] << '"' << si->c_str() << '"';
		}
		vector<double>::iterator di;
		for(di = f.data.begin(); di!=f.data.end(); ++di)
			if(di!=f.data.end()-1)
				reports[rt] << *di << sep;
			else
				reports[rt] << *di;
		f.state = FieldSet::done;
	}
}
void LoopMinder::Cancel() {
	for(FieldSets::iterator fi = m_fieldSets.begin(); fi!=m_fieldSets.end(); ++fi)
		fi->second.state = FieldSet::done;
}

} // namespace Dynagraph
