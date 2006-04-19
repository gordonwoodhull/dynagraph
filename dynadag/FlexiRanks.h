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

#ifndef FlexiRanks_h
#define FlexiRanks_h

#include "Rank.h"

namespace Dynagraph {
namespace DynaDAG {

struct CompRank {
	bool operator()(Rank *r1,Rank *r2) const {
#ifndef DOWN_GREATER
		return r1->yBase>r2->yBase;
#else
		return r1->yBase<r2->yBase;
#endif
	}
};
struct FlexiRanks : std::set<Rank*,CompRank> {
	typedef int index;
	double div,sep;
	FlexiRanks(double div,double sep) : div(div),sep(sep) {}
	FlexiRanks(FlexiRanks &o) {
		*this = o;
	}
	~FlexiRanks() {
		reset();
	}
	void reset() {
		iterator ri = begin();
		while(ri!=end()) {
			Rank *r = *ri;
			iterator er = ri++;
			erase(er);
			delete r;
		}
	}
	FlexiRanks &operator =(FlexiRanks &o) {
		reset();
		oldRanks = o.oldRanks;
		newRanks = o.newRanks;
		div = o.div;
		sep = o.sep;
		for(iterator ri = o.begin(); ri!=o.end(); ++ri) {
			Rank *nr = new Rank(**ri);
			insert(nr);
		}
		return *this;
	}
	Rank *front() { return *begin(); }
	Rank *back() { return *rbegin(); }
	index Low() { if(empty()) return 0; else return y2r(front()->yBase); }
	index High() { if(empty()) return 0; else return y2r(back()->yBase); }
	bool Above(index a,index b) {
		return a<b;
	}
	bool Below(index a,index b) {
		return a>b;
	}
	index Up(index r) {
		if(r==INT_MIN)
			return r;
		iterator ri = GetIter(r);
		if(ri==begin())
			return INT_MIN;
		return IndexOfIter(--ri);
	}
	index Down(index r) {
		if(r==INT_MAX)
			return r;
		iterator ri = GetIter(r);
		if(++ri==end())
			return INT_MAX;
		return IndexOfIter(ri);
	}
	iterator GetIter(index r) {
		Rank q(sep);
		q.yBase = r2y(r);
		return find(&q);
	}
	Rank *GetRank(index r)	{
		iterator ri = GetIter(r);
		if(ri==end())
			return 0;
		else
			return *ri;
	}
	iterator EnsureRank(index r) {
		assert(r!=INT_MAX && r!=INT_MIN); // off bottom or top
		iterator ri = GetIter(r);
		if(ri==end()) {
			Rank *rank = new Rank(sep);
			rank->yBase = r2y(r);
			ri = insert(rank).first;
		}
		return ri;
	}
	index IndexOfIter(iterator ri) {
		return y2r((*ri)->yBase);
	}
	index MapCoordToRank(double y) {
		return y2r(y);
	}
	// Flexi-specific
	void RemoveRank(iterator ri) {
		Rank *del = *ri;
		erase(ri);
		delete del;
	}
	index y2r(double y) {
#ifndef DOWN_GREATER
		return -ROUND(y/div);
#else
		return ROUND(y/div);
#endif
	}
	double r2y(index r) {
#ifndef DOWN_GREATER
		return -r*div;
#else
		return r*div;
#endif
	}
	void Check();
	typedef std::vector<index> IndexV;
	IndexV newRanks,oldRanks;
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // FlexiRanks_h
