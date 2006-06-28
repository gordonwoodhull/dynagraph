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

namespace Dynagraph {

template<typename Layout>
struct ColorByAge : LinkedChangeProcessor<Layout> {
	ColorByAge(ChangingGraph<Layout> *world) : LinkedChangeProcessor<Layout>(world) {}
	void Process();
};

template<typename GO>
inline void rotateColor(std::vector<DString> colors, GO *go) {
	DString color = gd<StrAttrs>(go)["color"];
	if(color.empty())
		SetAndMark(go,"color",colors[0]);
	else for(size_t i=0; i<colors.size()-1; ++i)
		if(color==colors[i]) {
			SetAndMark(go,"color",colors[i+1]);
			break;
		}
}
template<typename Layout>
void ColorByAge<Layout>::Process() {
    StrAttrs::iterator ai = gd<StrAttrs>(&this->world_->current_).find("agecolors");
    if(ai==gd<StrAttrs>(&this->world_->current_).end())
        return;
    std::vector<DString> colors;
    breakList(ai->second,colors);
    if(colors.size()==0)
        return;
    for(typename Layout::node_iter ni = this->world_->current_.nodes().begin(); ni!=this->world_->current_.nodes().end(); ++ni)
        rotateColor(colors,*ni);
    for(typename Layout::graphedge_iter ei = this->world_->current_.edges().begin(); ei!=this->world_->current_.edges().end(); ++ei)
        rotateColor(colors,*ei);
	this->NextProcess();
}

} // namespace Dynagraph

#endif //ColorByAge_h
