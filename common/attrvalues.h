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


extern DString g_NLPNames[5]; // in stringsOut.cpp
NodeLabelAlignment string2nlp(DString s) {
	for(int i=0;i<5;++i)
		if(g_NLPNames[i]==s)
			return static_cast<NodeLabelAlignment>(i);
	return DG_NODELABEL_CENTER;
}
