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


#include "DynaDAG.h"

namespace DynaDAG {

Crossings uvcross(DDModel::Node *v, DDModel::Node *w, bool use_in, bool use_out) {
	Crossings ret;
	if(use_in)
		for(DDModel::inedge_iter ei = w->ins().begin(); ei!=w->ins().end(); ++ei)
			for(DDModel::inedge_iter ej = v->ins().begin(); ej!=v->ins().end(); ++ej)
				if(DDd((*ej)->tail).order > DDd((*ei)->tail).order)
					ret.add(*ei,*ej);
	if (use_out)
		for(DDModel::outedge_iter ei = w->outs().begin(); ei!=w->outs().end(); ++ei)
			for(DDModel::outedge_iter ej = v->outs().begin(); ej!=v->outs().end(); ++ej)
				if(DDd((*ej)->head).order > DDd((*ei)->head).order)
					ret.add(*ei,*ej);
	return ret;
}

} // namespace DynaDAG
