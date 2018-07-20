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

using namespace std;

namespace Dynagraph {
namespace DynaDAG {

const double EPSILON = 1.0e-5; // DBL_EPSILON*10; // 1.0e-36;

Region &getRegion(DDModel::Node *n) {
    DynaDAGLayout::Node *vn = gd<DDNode>(n).multi->layoutN;
    return gd<NodeGeom>(vn).region;
}
bool Config::IsSuppressed(DDModel::Node *n) {
    if(gd<DDNode>(n).amNodePart())
        return gd<NodeGeom>(gd<DDNode>(n).multi->layoutN).suppressed;
    else {
        DynaDAGLayout::Edge *e = gd<DDEdge>(*n->outs().begin()).path->layoutE;
        switch(gd<Suppression>(e).suppression) {
        case Suppression::suppressed:
            return true;
        case Suppression::tailSuppressed:
        case Suppression::headSuppressed:
            return gd<DDNode>(n).rank!=gd<Suppression>(e).suppressRank // the suppressRank is itself never suppressed!
                && (gd<Suppression>(e).suppression==Suppression::headSuppressed)
                    ^ (getEdgeDirection(e)==reversed)
                    ^ (gd<DDNode>(n).rank<gd<Suppression>(e).suppressRank);
        default:
            return false;
        }
    }
}
bool Config::IsSuppressed(DDModel::Edge *e) {
    return IsSuppressed(e->tail) || IsSuppressed(e->head);
}
double Config::LeftExtent(DDModel::Node *n) {
    if(!n || IsSuppressed(n))
        return 0.0;
    if(gd<DDNode>(n).amEdgePart())
        return 0.;
    if(getRegion(n).boundary.valid)
        return -getRegion(n).boundary.l;
    return EPSILON;
}
double Config::RightExtent(DDModel::Node *n) {
    if(!n || IsSuppressed(n))
        return 0.0;
    if(gd<DDNode>(n).amEdgePart())
        return 0.;
    double r = 0.0;
    if(getRegion(n).boundary.valid)
        r = getRegion(n).boundary.r;
    DynaDAGLayout::Node *ln = gd<DDNode>(n).multi->layoutN;
    if(current->find_edge(ln,ln))
        r += gd<GraphGeom>(whole).separation.x;
    return max(r,EPSILON);
}
double Config::TopExtent(DDModel::Node *n) {
    if(!n || IsSuppressed(n))
        return 0.0;
    if(gd<DDNode>(n).amEdgePart())
        return EPSILON;
    if(getRegion(n).boundary.valid)
        return getRegion(n).boundary.t;
    return EPSILON;
}
double Config::BottomExtent(DDModel::Node *n) {
    if(!n || IsSuppressed(n))
        return 0.0;
    if(gd<DDNode>(n).amEdgePart())
        return EPSILON;
    if(getRegion(n).boundary.valid)
        return -getRegion(n).boundary.b;
    return EPSILON;
}
double Config::LeftSep(DDModel::Node *n) {
    if(!n || IsSuppressed(n))
        return .0;
    if(gd<DDNode>(n).amEdgePart())
        return gd<GraphGeom>(whole).edgeSeparation>=0.
            ? gd<GraphGeom>(whole).edgeSeparation
            : gd<GraphGeom>(whole).separation.x/4.0;
    else return gd<GraphGeom>(whole).separation.x/2.0;
}
double Config::RightSep(DDModel::Node *n) {
    if(!n || IsSuppressed(n))
        return .0;
    if(gd<DDNode>(n).amEdgePart())
        return gd<GraphGeom>(whole).edgeSeparation>=0.
            ? gd<GraphGeom>(whole).edgeSeparation
            : gd<GraphGeom>(whole).separation.x/4.0;
    else return gd<GraphGeom>(whole).separation.x/2.0;
}
double Config::UVSep(DDModel::Node *left,DDModel::Node *right) {
    double lext = RightExtent(left),
        lsep = RightSep(left),
        rsep = LeftSep(right),
        rext = LeftExtent(right);
    return lext + lsep + rsep + rext;
}
double Config::CoordBetween(DDModel::Node *L, DDModel::Node *R) {
    double x;
    if(L || R) {
        double lx = L ? gd<DDNode>(L).cur.x : gd<DDNode>(R).cur.x - 2.0 * UVSep(0,R),
            rx = R ? gd<DDNode>(R).cur.x : gd<DDNode>(L).cur.x + 2.0 * UVSep(L,0);
        x = (lx + rx) / 2.0;
    }
    else x = 0.0;
    return x;
}
DDModel::Node *Config::RelNode(DDModel::Node *n, int offset) {
    unsigned pos = gd<DDNode>(n).order + offset;
    if(pos < 0)
        return 0;
    Rank *rank = ranking.GetRank(gd<DDNode>(n).rank);
    dgassert(rank);
    if(pos >= rank->order.size())
        return 0;
    return rank->order[pos];
}
DDModel::Node *Config::Right(DDModel::Node *n) {
    return RelNode(n,1);
}
DDModel::Node *Config::Left(DDModel::Node *n) {
    return RelNode(n,-1);
}
// "monster atoms"
// contains: one vector append, one graph insertion
void Config::InstallAtRight(DDModel::Node *n, int r) {
    Rank *rank = *ranking.EnsureRank(r,gd<GraphGeom>(current).separation.y);
    DDModel::Node *right = rank->order.size()?rank->order.back():0;
    double x = right?gd<DDNode>(right).cur.x + UVSep(right,n):0.0;
    dgassert(!right||x>=gd<DDNode>(right).cur.x);
    rank->order.push_back(n);
    DDNode &ddn = gd<DDNode>(n);
    ddn.rank = r;
    ddn.order = rank->order.size()-1;
    ddn.inConfig = true;
    ddn.cur.valid = true;
    ddn.cur.x = x;
    ddn.cur.y = rank->yBase; // estimate
#ifndef REDO_ALL_XCONSTRAINTS
    model.dirty().insert(n);
#endif
}
// contains: one vector insert, one node-edge iteration, one vector iterate-and-set, one graph insertion
void Config::InstallAtOrder(DDModel::Node *n, int r, unsigned o, double x) {
    Rank *rank = *ranking.EnsureRank(r,gd<GraphGeom>(current).separation.y);
    dgassert(o<=rank->order.size() && o>=0);
    NodeV::iterator i = rank->order.begin()+o;
    i = rank->order.insert(i,n);
    DDNode &ddn = gd<DDNode>(n);
    ddn.rank = r;
    ddn.order = i - rank->order.begin();
    ddn.inConfig = true;
    InvalidateAdjMVals(n);
    ddn.cur.valid = true;
    ddn.cur.x = x;
    ddn.cur.y = rank->yBase; // estimate

    for(++i; i!=rank->order.end(); ++i) {
        gd<DDNode>(*i).order++;
        InvalidateAdjMVals(*i);
    }
#ifndef REDO_ALL_XCONSTRAINTS
    model.dirty().insert(n);
#endif
}
void Config::InstallAtOrder(DDModel::Node *n, int r, unsigned o) {
    Rank *rank = *ranking.EnsureRank(r,gd<GraphGeom>(current).separation.y);
    dgassert(o<=rank->order.size() && o>=0);
    DDModel::Node *L = o>0?rank->order[o-1]:0,
        *R = o<rank->order.size()?rank->order[o]:0;
    InstallAtOrder(n,r,o,CoordBetween(L,R));
}
struct XLess {
    bool operator()(DDModel::Node *u,DDModel::Node *v) {
        return gd<DDNode>(u).cur.x < gd<DDNode>(v).cur.x;
    }
};
void Config::InstallAtPos(DDModel::Node *n, int r, double x) {
    Rank *rank = *ranking.EnsureRank(r,gd<GraphGeom>(current).separation.y);
    gd<DDNode>(n).cur.x = x;
    NodeV::iterator i = lower_bound(rank->order.begin(),rank->order.end(),n,XLess());
    DDModel::Node *L = i==rank->order.begin()?0:*(i-1),
        *R = i==rank->order.end()?0:*i;
    InstallAtOrder(n,r,R?gd<DDNode>(R).order:L?gd<DDNode>(L).order+1:0,x);
}
// cheap atom; unused: two node-edge iterations, two graph insertions
void Config::Exchange(DDModel::Node *u, DDModel::Node *v) {
    DDNode &ddu = gd<DDNode>(u),
        &ddv = gd<DDNode>(v);
    dgassert(ddu.inConfig);
    dgassert(ddv.inConfig);
    dgassert(ddu.rank == ddv.rank);
    //reports[dgr::exchange] << "exchange " <<
    //  u << " of " << type(u) << ' ' << thing(u) << ": x " << ddu.cur.x << " r " << ddu.rank << " o " << ddu.order << endl <<
    //  v << " of " << type(v) << ' ' << thing(v) << ": x " << ddv.cur.x << " r " << ddv.rank << " o " << ddv.order << endl;

    Rank *rank = ranking.GetRank(ddu.rank);
    //int upos = ddu.order,
    //  vpos = ddv.order;
    //dgassert(vpos==upos+1);

    /*xconOwner->RemoveNodeConstraints(u);
    xconOwner->RemoveNodeConstraints(v);*/

    //rank->order[vpos] = u; gd<DDNode>(u).order = vpos;
    //rank->order[upos] = v; gd<DDNode>(v).order = upos;
    swap(rank->order[ddu.order],rank->order[ddv.order]);
    swap(ddu.order,ddv.order);
    swap(gd<DDNode>(u).cur.x,gd<DDNode>(v).cur.x); // keep x order consistent
    InvalidateAdjMVals(u);
    InvalidateAdjMVals(v);
#ifndef REDO_ALL_XCONSTRAINTS
    /* delete any LR constraint between these nodes */
    xconOwner->DeleteLRConstraint(u,v);
    model.dirty().insert(u);
    model.dirty().insert(v);
#endif
}
void Config::MoveNodeBefore(DDModel::Node *u,DDModel::Node *v) {
    DDNode &ddu = gd<DDNode>(u);
    dgassert(ddu.inConfig);
    unsigned upos = ddu.order, vpos;
    Rank *rank = ranking.GetRank(ddu.rank);
    NodeV &order = rank->order;
    if(v) {
        dgassert(gd<DDNode>(v).inConfig);
        dgassert(gd<DDNode>(u).rank==gd<DDNode>(v).rank);
        vpos = gd<DDNode>(v).order;
    }
    else 
        vpos = order.size();
    if(upos+1==vpos)
        return; 
    if(upos+2==vpos) {
        Exchange(u,Right(u));
        return;
    }
    NodeV::iterator ui = order.begin()+upos,
        vi = order.begin()+vpos;
    if(upos<vpos) {
        for(NodeV::iterator xi = ui; xi<vi-1; ++xi) {
            *xi = *(xi+1);
            DDModel::Node *x = *xi;
            --gd<DDNode>(x).order;
            InvalidateAdjMVals(x);
        }
        *(vi-1) = u;
        ddu.order = vpos-1;
    }
    else {
        for(NodeV::iterator xi = ui; xi>vi; --xi) {
            *xi = *(xi-1);
            DDModel::Node *x = *xi;
            ++gd<DDNode>(x).order;
            InvalidateAdjMVals(x);
        }
        *vi = u;
        ddu.order = vpos;
    }
    ddu.cur.x = CoordBetween(Left(u),Right(u));
    InvalidateAdjMVals(u);
}
// contains: one edge delete, one node-edge iteration, one vector iterate-and-set, one vector erase, one graph insertion
void Config::RemoveNode(DDModel::Node *n) {
    xconOwner->RemoveNodeConstraints(n);
    InvalidateAdjMVals(n);
    DDNode &ddn = gd<DDNode>(n);
    if(!ddn.inConfig)
        return;
    Rank *rank = ranking.GetRank(gd<DDNode>(n).rank);
    int pos = gd<DDNode>(n).order;
    dgassert(rank->order[pos] == n);
    NodeV::iterator i = rank->order.begin()+pos;
#ifndef REDO_ALL_XCONSTRAINTS
    if(i!=rank->order.end())
        model.dirty().insert(*i); // re-constrain right node
#endif
    for(NodeV::iterator j=i;j!=rank->order.end(); ++j)
        gd<DDNode>(*j).order--;
    rank->order.erase(i);
    ddn.inConfig = false;
    ddn.rank = INT_MIN;
    /*
    if(ddn.multi)
        ddn.multi->coordFixed = false;
    */
    // ddn.orderFixed = false;
}
void Config::Restore(Ranks &backup) {
    ranking = backup;
    for(Ranks::iterator ri = ranking.begin(); ri!=ranking.end(); ++ri) {
        Rank *r = *ri;
        r->check_backdup_x();
        for(NodeV::iterator ni = r->order.begin(); ni!=r->order.end(); ++ni) {
            int o = int(ni - r->order.begin());
            if(gd<DDNode>(*ni).order != o) {
                InvalidateAdjMVals(*ni);
                gd<DDNode>(*ni).order = o;
            }
            gd<DDNode>(*ni).cur.x = r->x_backup[o];
        }
    }
    checkX();
}

} // namespace DynaDAG
} // namespace Dynagraph
