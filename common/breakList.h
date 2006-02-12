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

#ifndef breakList_h
#define breakList_h

namespace Dynagraph {

inline void breakList(DString l,std::vector<DString> &out) {
	for(DString::size_type i=0,com = 0;i<l.size() && com<l.size();i = com+1) {
		while(isspace(l[i])) ++i;
		com = l.find(',',i);
		out.push_back(l.substr(i,com-i));
    }
}

} // namespace Dynagraph

#endif // breakList_h
