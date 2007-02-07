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

#ifndef SiftMatrix_h
#define SiftMatrix_h

#include "Config.h"
#include "Crossings.h"

namespace Dynagraph {
namespace DynaDAG {

template<typename WeighFun>
struct SiftMatrix {
	SiftMatrix(Config &config) : config(config) {
	}
	std::pair<unsigned,unsigned> &crossings(DDModel::Node *u,DDModel::Node *v) {
		dgassert(gd<DDNode>(u).rank==gd<DDNode>(v).rank);
		return data[gd<DDNode>(u).rank][gd<DDNode>(u).order][gd<DDNode>(v).order];
	}
	typedef enum {ins,outs,all} ways_t;
	unsigned getCrossings(DDModel::Node *u, DDModel::Node *v,ways_t ways) {
		std::pair<unsigned,unsigned> &p = crossings(u,v);
		dgassert(p.first>=0 && p.second>=0);
		unsigned ret=0;
		if(ways==ins || ways==all)
			ret += p.first;
		if(ways==outs || ways==all)
			ret += p.second;
		return ret;
	}
	void setCrossings(DDModel::Node *u, DDModel::Node *v,bool outs,unsigned val) {
		dgassert(val>=0);
		std::pair<unsigned,unsigned> &p = crossings(u,v);
		dgassert(p.first>=0 && p.second>=0);
		if(outs)
			p.second = val;
		else
			p.first = val;
	}
	SiftMatrix &operator =(SiftMatrix &other) {
		data = other.data;
		return *this;
	}
	void recompute();
	unsigned sumCrossings() {
		unsigned ret = 0;
		for(Config::Ranks::iterator ri = config.ranking.begin(); ri!=config.ranking.end(); ++ri) {
			Rank *r = *ri;
			if(r->order.size()<2)
				continue;
			NodeV::iterator end = r->order.end()-1;
			for(NodeV::iterator ni = r->order.begin(); ni<end; ++ni)
				for(NodeV::iterator ni2 = ni+1;ni2!=r->order.end(); ++ni2) {
					unsigned c = getCrossings(*ni,*ni2,ins);
					dgassert(c>=0);
					ret += c;
				}
		}
		return ret;
	}
	void move(DDModel::Node *v, DDModel::Node *before);
	void checkWithConfig();
private:
	typedef std::pair<unsigned,unsigned> CrossCount;
	typedef std::vector<std::vector<CrossCount> > Layer;
	typedef std::map<unsigned,Layer> Data;
	Data data;
	Config &config;
	WeighFun weigh_;
	void updateOuts(DDModel::Node *v,DDModel::Node *x);
	void updateIns(DDModel::Node *v,DDModel::Node *x);
	void update(DDModel::Node *v,DDModel::Node *x);
	void rowCopy(unsigned rank,unsigned dst,unsigned src);
	void colCopy(unsigned rank,unsigned dst,unsigned src);
	void rowPaste(unsigned rank,unsigned dst,const std::vector<CrossCount> &src);
	void colPaste(unsigned rank,unsigned dst,const std::vector<CrossCount> &src);
};
template<typename WeighFun>
void SiftMatrix<WeighFun>::recompute() {
	for(Config::Ranks::iterator ri = config.ranking.begin(); ri!=config.ranking.end(); ++ri) {
		Rank *rank = *ri;
		unsigned r = config.ranking.IndexOfIter(ri);
		unsigned wid = rank->order.size();
		Layer &l = data[r];
		l.resize(wid);
		for(unsigned o = 0; o<wid; ++o) {
			l[o].resize(wid);
			for(unsigned o2 = 0; o2<wid; ++o2) {
				CrossCount cr = make_pair(weigh_(uvcross(rank->order[o],rank->order[o2],true,false)),
					weigh_(uvcross(rank->order[o],rank->order[o2],false,true)));
				dgassert(cr.first>=0);
				dgassert(cr.second>=0);
				l[o][o2] = cr;
			}
		}
	}
}
template<typename WeighFun>
void SiftMatrix<WeighFun>::rowCopy(unsigned rank,unsigned dst,unsigned src) {
	Layer &l = data[rank];
	for(unsigned i = 0;i<l.size(); ++i)
		l[dst][i] = l[src][i];
}
template<typename WeighFun>
void SiftMatrix<WeighFun>::colCopy(unsigned rank,unsigned dst,unsigned src) {
	Layer &l = data[rank];
	for(unsigned i = 0;i<l.size(); ++i)
		l[i][dst] = l[i][src];
}
template<typename WeighFun>
void SiftMatrix<WeighFun>::rowPaste(unsigned rank,unsigned dst,const std::vector<CrossCount> &src) {
	Layer &l = data[rank];
	for(unsigned i = 0;i<l.size(); ++i)
		l[dst][i] = src[i];
}
template<typename WeighFun>
void SiftMatrix<WeighFun>::colPaste(unsigned rank,unsigned dst,const std::vector<CrossCount> &src) {
	Layer &l = data[rank];
	for(unsigned i = 0;i<l.size(); ++i)
		l[i][dst] = src[i];
}
// v is going to switch sides with x
template<typename WeighFun>
void SiftMatrix<WeighFun>::updateOuts(DDModel::Node *v,DDModel::Node *x) {
	bool toLeft = gd<DDNode>(x).order<gd<DDNode>(v).order;
	for(DDModel::outedge_iter uxi = x->outs().begin(); uxi!=x->outs().end(); ++uxi) {
		DDModel::Node *ux = (*uxi)->other(x);
		for(DDModel::outedge_iter uvi = v->outs().begin(); uvi!=v->outs().end(); ++uvi) {
			DDModel::Node *uv = (*uvi)->other(v);
			if(gd<DDNode>(ux).order==gd<DDNode>(uv).order)
				continue;
			Crossings c(*uxi,*uvi);
			unsigned cost = weigh_(c);
			dgassert(cost>=0);
			if(toLeft) {
			  setCrossings(uv,ux,false,getCrossings(uv,ux,ins)-cost);
			  setCrossings(ux,uv,false,getCrossings(ux,uv,ins)+cost);
			}
			else {
			  setCrossings(ux,uv,false,getCrossings(ux,uv,ins)-cost);
			  setCrossings(uv,ux,false,getCrossings(uv,ux,ins)+cost);
			}
		}
	}
}
template<typename WeighFun>
void SiftMatrix<WeighFun>::updateIns(DDModel::Node *v,DDModel::Node *x) {
	bool toLeft = gd<DDNode>(x).order<gd<DDNode>(v).order;
	for(DDModel::inedge_iter uxi = x->ins().begin(); uxi!=x->ins().end(); ++uxi) {
		DDModel::Node *ux = (*uxi)->other(x);
		for(DDModel::inedge_iter uvi = v->ins().begin(); uvi!=v->ins().end(); ++uvi) {
			DDModel::Node *uv = (*uvi)->other(v);
			if(gd<DDNode>(ux).order==gd<DDNode>(uv).order)
				continue;
			unsigned cost = weigh_(Crossings(*uxi,*uvi));
			dgassert(cost>=0);
			if(toLeft) {
			  setCrossings(uv,ux,true,getCrossings(uv,ux,outs) -cost);
			  setCrossings(ux,uv,true,getCrossings(ux,uv,outs)+cost);
			}
			else {
			  setCrossings(ux,uv,true,getCrossings(ux,uv,outs)-cost);
			  setCrossings(uv,ux,true,getCrossings(uv,ux,outs)+cost);
			}
		}
	}
}
template<typename WeighFun>
void SiftMatrix<WeighFun>::update(DDModel::Node *v,DDModel::Node *x) {
	updateOuts(v,x);
	updateIns(v,x);
}
// node v will be moved before node w
template<typename WeighFun>
void SiftMatrix<WeighFun>::move(DDModel::Node *v, DDModel::Node *w) {
	int r = gd<DDNode>(v).rank;
	dgassert(!w||r==gd<DDNode>(w).rank);
	Rank *rank = config.ranking.GetRank(r);
	unsigned size = rank->order.size(),
		vo = gd<DDNode>(v).order,
		wo = w?gd<DDNode>(w).order:size,
		dest = wo>vo?wo-1:wo;
	// rearrange rows + columns
	// 1. save row + column v
	std::vector<CrossCount> cutRow,cutCol;
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
template<typename WeighFun>
void SiftMatrix<WeighFun>::checkWithConfig() {
	for(Data::iterator li = data.begin(); li!=data.end(); ++li) {
		Layer &l = li->second;
		for(unsigned o = 0; o<l.size(); o++)
			for(unsigned o2 = 0; o2<l.size(); ++o2) {
				DDModel::Node *u = config.ranking.GetRank(li->first)->order[o],
					*v = config.ranking.GetRank(li->first)->order[o2];
				dgassert(getCrossings(u,v,ins)==weigh_(uvcross(u,v,true,false)));
				dgassert(getCrossings(u,v,outs)==weigh_(uvcross(u,v,false,true)));
			}
	}
}

} // namespace DynaDAG
} // namespace Dynagraph

#endif // SiftMatrix_h
