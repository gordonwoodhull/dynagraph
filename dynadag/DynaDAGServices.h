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

#ifndef DynaDAGServices_h
#define DynaDAGServices_h

#include "DynaDAGModel.h"
#include "Optimizer.h"

namespace Dynagraph {
namespace DynaDAG {

struct DynaDAGServices {
	virtual ~DynaDAGServices() {}
	virtual std::pair<DDMultiNode*,DDModel::Node*> OpenModelNode(DynaDAGLayout::Node *layoutN) = 0;
	virtual void CloseModelNode(DDModel::Node *n) = 0;
	virtual std::pair<DDPath*,DDModel::Edge*> OpenModelEdge(DDModel::Node *u, DDModel::Node *v, DynaDAGLayout::Edge *layoutE) = 0;
	virtual void CloseModelEdge(DDModel::Edge *e) = 0;
	virtual void CloseChain(DDChain *chain,bool killEndNodes) = 0;
	virtual Optimizer *GetOptimizer() = 0;
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // DynaDAGServices_h
