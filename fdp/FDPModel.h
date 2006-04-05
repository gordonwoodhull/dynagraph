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


#ifndef	FDPMODEL_H
#define FDPMODEL_H

namespace Dynagraph {
namespace FDP {

#define NDIM 2

struct FDPEdge {
	FDPLayout::Edge *layoutE;

	FDPEdge() : layoutE(0) {}
};

struct FDPNode {
	FDPLayout::Node *layoutN;
    bool fixed; // true if node should not move
    double pos[NDIM], // new position
		disp[NDIM]; // incremental displacement

	FDPNode() : layoutN(0),fixed(false) {
		for(int i = 0; i<NDIM; ++i)
			pos[i] = disp[i] = 0.0;
	}
};

typedef LGraph<ADTisCDT,Nothing,FDPNode,FDPEdge> FDPModel;

inline FDPModel::Node *&modelP(FDPLayout::Node *n) {
	return reinterpret_cast<FDPModel::Node*&>(gd<ModelPointer>(n).model);
}
inline FDPModel::Edge *&modelP(FDPLayout::Edge *e) {
	return reinterpret_cast<FDPModel::Edge*&>(gd<ModelPointer>(e).model);
}

} // namespace FDP
} // namespace Dynagraph

#endif // FDPMODEL_H
