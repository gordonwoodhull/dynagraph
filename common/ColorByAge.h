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
#ifndef ColorByAge_h
#define ColorByAge_h

#include <sstream>
#include "breakList.h"

template<typename Layout>
struct ColorByAge : Server<Layout> {
	void Process(ChangeQueue<Layout> &Q);
	ColorByAge(Layout *client,Layout *currentLayout) : Server<Layout>(client,currentLayout) {}
	~ColorByAge() {}
};

inline void rotateColor(std::vector<DString> colors, StrAttrs2 &attrs2) {
  DString color = attrs2["color"];
  if(color.empty())
    attrs2.put("color",colors[0]);
  else for(size_t i=0; i<colors.size()-1; ++i)
    if(color==colors[i]) {
      attrs2.put("color",colors[i+1]);
      break;
    }
}
template<typename Layout>
void ColorByAge<Layout>::Process(ChangeQueue<Layout> &Q) {
    StrAttrs::iterator ai = gd<StrAttrs>(Q.current).find("agecolors");
    if(ai==gd<StrAttrs>(Q.current).end())
        return;
    std::vector<DString> colors;
    breakList(ai->second,colors);
    if(colors.size()==0)
        return;
    for(typename Layout::node_iter ni = Q.current->nodes().begin(); ni!=Q.current->nodes().end(); ++ni)
        rotateColor(colors,gd<StrAttrs2>(*ni));
    for(typename Layout::graphedge_iter ei = Q.current->edges().begin(); ei!=Q.current->edges().end(); ++ei)
        rotateColor(colors,gd<StrAttrs2>(*ei));
}

#endif //ColorByAge_h