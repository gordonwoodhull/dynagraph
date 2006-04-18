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
#ifndef REORIENT_H
#define REORIENT_H

namespace Dynagraph {

struct ReorientBadDir : DGException {
	ReorientBadDir() : DGException("bad direction argument to reorient()",true) {}
};
inline Coord reorient(Coord val,bool in,Orientation dir) {
	Coord ret;
	switch(dir) {
	case DG_ORIENT_UP:
		ret.x = -val.x;
		ret.y = -val.y;
		break;
	case DG_ORIENT_DOWN:
		ret.x = val.x;
		ret.y = val.y;
		break;
	case DG_ORIENT_LEFT:
		if(in)
			goto right;
	left:
		ret.x = val.y;
		ret.y = -val.x;
		break;
	case DG_ORIENT_RIGHT:
		if(in)
			goto left;
	right:
		ret.x = -val.y;
		ret.y = val.x;
		break;
	default:
		throw ReorientBadDir();
	}
	return ret;
}

} // namespace Dynagraph

#endif // REORIENT_H
