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


#include "StringDict.h"
#include "parsestr.h"

using namespace std;

inline bool needsQuotes(const DString &s) {
	return needsQuotes(s.c_str());
}
ostream &operator<<(ostream &os,const mquote &mq) {
	if(mq.s.empty())
		os << "\"\"";
	else if(needsQuotes(mq.s))
		os << '"' << mq.s << '"';
	else
		os << mq.s;
	return os;
}
