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
#include "common/weightedMedian.h"

namespace Dynagraph {
namespace DynaDAG {

void autoMVal(DDModel::Node *n, UpDown dir) {
    doubV vec;
    DDModel::Node *n2;
    /*if(gd<DDNode>(n).amNodePart()) {
        if(dir==UP)
            n2 = gd<DDNode>(n).multi->top();
        else
            n2 = gd<DDNode>(n).multi->bottom();
    }
    else*/
        n2 = n;
    // ick
    if(dir==UP)
        for(DDModel::inedge_iter ei = n->ins().begin(); ei!=n->ins().end(); ++ei)
            vec.push_back(gd<DDNode>((*ei)->tail).order);
    else
        for(DDModel::outedge_iter ei = n->outs().begin(); ei!=n->outs().end(); ++ei)
            vec.push_back(gd<DDNode>((*ei)->head).order);
    if(!vec.size())
        gd<DDNode>(n).med[dir].exists = false;
    else {
        gd<DDNode>(n).med[dir].exists = true;
        gd<DDNode>(n).med[dir].val = weightedMedian(vec);
    }
    gd<DDNode>(n).med[dir].cached = true;
}
void InvalidateMVal(DDModel::Node *n, UpDown dir) {
    gd<DDNode>(n).med[dir].cached = false;
}
bool MValExists(DDModel::Node *n, UpDown dir) {
    DDNode &ddn = gd<DDNode>(n);
    if(!ddn.med[dir].cached)
        autoMVal(n,dir);
    return ddn.med[dir].exists;
}
double MVal(DDModel::Node *n, UpDown dir) {
    DDNode &ddn = gd<DDNode>(n);
    if(!ddn.med[dir].cached)
        autoMVal(n,dir);
    return ddn.med[dir].val;
}
void SetMVal(DDModel::Node *n, double val, UpDown dir) {
    DDNode &ddn = gd<DDNode>(n);

    ddn.med[dir].exists = true;
    ddn.med[dir].cached = true;
    ddn.med[dir].val = val;
}

/* this could be a lot better.  forces a lot of needless
 * recomputation.  could cache node pointers from which median
 * was computed, etc.  just live with this for now.
 */
void InvalidateAdjMVals(DDModel::Node *n) {
    for(DDModel::inedge_iter ini = n->ins().begin(); ini!=n->ins().end(); ++ini)
        InvalidateMVal((*ini)->tail,DOWN);
    for(DDModel::outedge_iter outi = n->outs().begin(); outi!=n->outs().end(); ++outi)
        InvalidateMVal((*outi)->head,UP);
}

} // namespace DynaDAG
} // namespace Dynagraph
