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

void SiftMatrix::recompute() {
    for(Config::Ranks::iterator ri = config.ranking.begin(); ri!=config.ranking.end(); ++ri) {
        Rank *rank = *ri;
        unsigned r = config.ranking.IndexOfIter(ri);
        unsigned wid = rank->order.size();
        Layer &l = data[r];
        l.resize(wid);
        for(unsigned o = 0; o<wid; ++o) {
            l[o].resize(wid);
            for(unsigned o2 = 0; o2<wid; ++o2) {
                CrossCount cr = make_pair(weigh(uvcross(rank->order[o],rank->order[o2],true,false)),
                    weigh(uvcross(rank->order[o],rank->order[o2],false,true)));
                dgassert(cr.first>=0);
                dgassert(cr.second>=0);
                l[o][o2] = cr;
            }
        }
    }
}
/*
unsigned SiftMatrix::total() {
    unsigned ret = 0;
    for(Config::Ranks::iterator ri = config.ranking.begin(); ri!=config.ranking.end(); ++ri) {
        Rank *r = *ri;
        if(r->order.size()<2)
            continue;
        NodeV::iterator end = r->order.end()-1;
        for(NodeV::iterator ni = r->order.begin(); ni<end; ++ni)
            for(NodeV::iterator ni2 = ni+1;ni2!=r->order.end(); ++ni2) {
                unsigned c = getCrossings(*ni,*ni2,false);
                dgassert(c>=0);
                ret += c;
            }
    }
    return ret;
}
*/
void SiftMatrix::rowCopy(unsigned rank,unsigned dst,unsigned src) {
    Layer &l = data[rank];
    for(unsigned i = 0;i<l.size(); ++i)
        l[dst][i] = l[src][i];
}
void SiftMatrix::colCopy(unsigned rank,unsigned dst,unsigned src) {
    Layer &l = data[rank];
    for(unsigned i = 0;i<l.size(); ++i)
        l[i][dst] = l[i][src];
}
void SiftMatrix::rowPaste(unsigned rank,unsigned dst,const vector<CrossCount> &src) {
    Layer &l = data[rank];
    for(unsigned i = 0;i<l.size(); ++i)
        l[dst][i] = src[i];
}
void SiftMatrix::colPaste(unsigned rank,unsigned dst,const vector<CrossCount> &src) {
    Layer &l = data[rank];
    for(unsigned i = 0;i<l.size(); ++i)
        l[i][dst] = src[i];
}
// v is going to switch sides with x
void SiftMatrix::updateOuts(DDModel::Node *v,DDModel::Node *x) {
    bool toLeft = gd<DDNode>(x).order<gd<DDNode>(v).order;
    for(DDModel::outedge_iter uxi = x->outs().begin(); uxi!=x->outs().end(); ++uxi) {
        DDModel::Node *ux = (*uxi)->other(x);
        for(DDModel::outedge_iter uvi = v->outs().begin(); uvi!=v->outs().end(); ++uvi) {
            DDModel::Node *uv = (*uvi)->other(v);
            if(gd<DDNode>(ux).order==gd<DDNode>(uv).order)
                continue;
            Crossings c(*uxi,*uvi);
            unsigned cost = weigh(c);
            dgassert(cost>=0);
            if(toLeft) {
              setCrossings(uv,ux,false,getCrossings(uv,ux,false) -cost);
              setCrossings(ux,uv,false,getCrossings(ux,uv,false)+cost);
            }
            else {
              setCrossings(ux,uv,false,getCrossings(ux,uv,false)-cost);
              setCrossings(uv,ux,false,getCrossings(uv,ux,false)+cost);
            }
        }
    }
}
void SiftMatrix::updateIns(DDModel::Node *v,DDModel::Node *x) {
    bool toLeft = gd<DDNode>(x).order<gd<DDNode>(v).order;
    for(DDModel::inedge_iter uxi = x->ins().begin(); uxi!=x->ins().end(); ++uxi) {
        DDModel::Node *ux = (*uxi)->other(x);
        for(DDModel::inedge_iter uvi = v->ins().begin(); uvi!=v->ins().end(); ++uvi) {
            DDModel::Node *uv = (*uvi)->other(v);
            if(gd<DDNode>(ux).order==gd<DDNode>(uv).order)
                continue;
            unsigned cost = weigh(Crossings(*uxi,*uvi));
            dgassert(cost>=0);
            if(toLeft) {
              setCrossings(uv,ux,true,getCrossings(uv,ux,true) -cost);
              setCrossings(ux,uv,true,getCrossings(ux,uv,true)+cost);
            }
            else {
              setCrossings(ux,uv,true,getCrossings(ux,uv,true)-cost);
              setCrossings(uv,ux,true,getCrossings(uv,ux,true)+cost);
            }
        }
    }
}
void SiftMatrix::update(DDModel::Node *v,DDModel::Node *x) {
    updateOuts(v,x);
    updateIns(v,x);
}
// node v will be moved before node w
void SiftMatrix::move(DDModel::Node *v, DDModel::Node *w) {
    int r = gd<DDNode>(v).rank;
    dgassert(!w||r==gd<DDNode>(w).rank);
    Rank *rank = config.ranking.GetRank(r);
    unsigned size = rank->order.size(),
        vo = gd<DDNode>(v).order,
        wo = w?gd<DDNode>(w).order:size,
        dest = wo>vo?wo-1:wo;
    // rearrange rows + columns
    // 1. save row + column v
    vector<CrossCount> cutRow,cutCol;
    cutRow.reserve(size);
    cutCol.reserve(size);
    for(unsigned j = 0; j<size; ++j) {
        cutRow.push_back(data[r][vo][j]);
        cutCol.push_back(data[r][j][vo]);
    }
    // 2. rearrange these
    CrossCount save = cutRow[vo];
    cutRow.erase(cutRow.begin()+vo);
    cutRow.insert(cutRow.begin()+dest,save);
    cutCol.erase(cutCol.begin()+vo);
    cutCol.insert(cutCol.begin()+dest,save);
    // 3. move columns + rows
    if(vo<wo) {
      unsigned i;
        for(i = vo; i<dest; ++i)
            rowCopy(r,i,i+1);
        for(i = vo; i<dest; ++i)
            colCopy(r,i,i+1);
    }
    else {
      unsigned i;
        for(i = vo; i>wo; --i)
            rowCopy(r,i,i-1);
        for(i = vo; i>wo; --i)
            colCopy(r,i,i-1);
    }
    // 4. paste row + column v
    rowPaste(r,dest,cutRow);
    colPaste(r,dest,cutCol);
    // 5. adjust adjacent ranks
    if(vo<wo)
        for(unsigned i = vo+1; i<wo; ++i) {
            DDModel::Node *x = rank->order[i];
            update(v,x);
        }
    else
        for(int i = vo-1; i>=int(wo); --i) {
            DDModel::Node *x = rank->order[i];
            update(v,x);
        }
}
void SiftMatrix::checkWithConfig() {
    for(Data::iterator li = data.begin(); li!=data.end(); ++li) {
        Layer &l = li->second;
        for(unsigned o = 0; o<l.size(); o++)
            for(unsigned o2 = 0; o2<l.size(); ++o2) {
                DDModel::Node *u = config.ranking.GetRank(li->first)->order[o],
                    *v = config.ranking.GetRank(li->first)->order[o2];
                dgassert(getCrossings(u,v,false)==weigh(uvcross(u,v,true,false)));
                dgassert(getCrossings(u,v,true)==weigh(uvcross(u,v,false,true)));
            }
    }
}

} // namespace DynaDAG
} // namespace Dynagraph

