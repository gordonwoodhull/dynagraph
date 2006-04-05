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
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include "Colors.h"
using namespace std;

namespace Dynagraph {

typedef struct hsbcolor_t {
	char			*name;
	unsigned char	h,s,b;
} hsbcolor_t;

#include "colortbl.h"

typedef map<DString,Color> named_colors;
named_colors g_namedColors;
void initNamed() {
    for(size_t i = 0; i<sizeof(color_lib)/sizeof(hsbcolor_t); ++i)
        g_namedColors[color_lib[i].name] = Color(color_lib[i].h/255.0f,color_lib[i].s/255.0f,color_lib[i].b/255.0f);
}
Color findColor(DString s) {
    if(g_namedColors.empty())
        initNamed();
    named_colors::iterator ci = g_namedColors.find(s);
    if(ci==g_namedColors.end())
        throw ColorNotFound(s);
    return ci->second;
}

} // namespace Dynagraph
