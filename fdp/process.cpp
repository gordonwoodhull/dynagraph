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


#include "fdp.h"
#include "common/weightedMedian.h"

using namespace std;

namespace Dynagraph {
namespace FDP {

Position FDPServer::findMedianSize() {
    int N = this->world_->current_.nodes().size(),i=0;
    if(N==0)
        return Position();
    vector<double> Xs(N),Ys(N);
    for(FDPLayout::node_iter ni = this->world_->current_.nodes().begin(); ni!=this->world_->current_.nodes().end(); ++ni,++i)
        if(gd<NodeGeom>(*ni).region.boundary.valid)
            Xs[i] = gd<NodeGeom>(*ni).region.boundary.Width(),
            Ys[i] = gd<NodeGeom>(*ni).region.boundary.Height();
        else
            Xs[i] = Ys[i] = 0.0;
    return Coord(weightedMedian(Xs),weightedMedian(Ys));
}
void FDPServer::setParams(Coord avgsize) {
    double k = max(avgsize.x,avgsize.y);//avgsize.Len();
    if(k)
        K = k;
    else
        K = 1.0;

    if(T0 <= 0.0)
        T0 = K*sqrt((double)model.nodes().size())/5;

    K2 = K*K;

    if(useGrid) {
        if(CellSize <= 0.0)
            CellSize = 3*K;
        Radius2 = CellSize *CellSize;
    }
}

void FDPServer::createModelNode(FDPLayout::Node *n) {
    if(modelP(n))
        throw Inconsistency();
    n = this->world_->whole_.find(n);
    FDPModel::Node *fn = model.create_node();
    gd<FDPNode>(fn).layoutN = n;
    modelP(n) = fn;
}
void FDPServer::createModelEdge(FDPLayout::Edge *e) {
    if(modelP(e))
        throw Inconsistency();
    e = this->world_->whole_.find(e);
    FDPModel::Node *ft = modelP(e->tail),
        *fh = modelP(e->head);
    if(!ft || !fh)
        throw Inconsistency();
    FDPModel::Edge *fe = model.create_edge(ft,fh).first;
    gd<FDPEdge>(fe).layoutE = e;
    modelP(e) = fe;
}
void FDPServer::deleteModelNode(FDPLayout::Node *n) {
    if(!modelP(n))
        throw Inconsistency();
    FDPModel::Node *fn = modelP(n);
    if(fn->degree())
        throw StillHasEdges();
    model.erase(fn);
    modelP(n) = 0;
}
void FDPServer::deleteModelEdge(FDPLayout::Edge *e) {
    if(!modelP(e))
        throw Inconsistency();
    FDPModel::Edge *fe = modelP(e);
    model.erase(fe);
    modelP(e) = 0;
}
inline void readPos(FDPLayout::Node *n) {
    FDPNode &fdpn = gd<FDPNode>(modelP(n));
    Position pos = gd<NodeGeom>(n).pos;
    if(!pos.valid)
        pos = Coord(0,0);
    fdpn.pos[0] = pos.x;
    fdpn.pos[1] = pos.y;
}
void FDPServer::Process() {
    ChangeQueue<FDPLayout> &Q = this->world_->Q_;
    // this is not incremental, really: just respond to events, run,
    // and then say "everything changed"!
    FDPLayout::node_iter ni;
    FDPLayout::graphedge_iter ei;
    for(ni = Q.insN.nodes().begin(); ni!=Q.insN.nodes().end(); ++ni) {
        createModelNode(*ni);
        readPos(*ni);
    }
    for(ei = Q.insE.edges().begin(); ei!=Q.insE.edges().end(); ++ei)
        createModelEdge(*ei);

    for(ni = Q.modN.nodes().begin(); ni!=Q.modN.nodes().end(); ++ni)
        if(igd<Update>(*ni).flags & DG_UPD_MOVE)
            readPos(*ni);

    for(ei = Q.delE.edges().begin(); ei!=Q.delE.edges().end(); ++ei)
        deleteModelEdge(*ei);
    for(ni = Q.delN.nodes().begin(); ni!=Q.delN.nodes().end(); ++ni)
        deleteModelNode(*ni);

    Position medianSize = findMedianSize();
    if(!medianSize.valid)
        return;
    setParams(medianSize);
    layout();
    //splineEdges();

    // anything that's not ins or del is modified
    for(ni = this->world_->current_.nodes().begin(); ni!=this->world_->current_.nodes().end(); ++ni) {
        FDPModel::Node *fn = modelP(*ni);
        gd<NodeGeom>(*ni).pos = Coord(gd<FDPNode>(fn).pos[0],gd<FDPNode>(fn).pos[1]);
        ModifyNode(Q,*ni,DG_UPD_MOVE);
    }
    /*
    this algorithm doesn't deal with edges
    for(ei = Q.this->world_->current_.edges().begin(); ei!=Q.this->world_->current_.edges().end(); ++ei)
        ModifyEdge(Q,*ei,DG_UPD_MOVE);
    */
    NextProcess();
}

} // namespace FDP
} // namespace Dynagraph
