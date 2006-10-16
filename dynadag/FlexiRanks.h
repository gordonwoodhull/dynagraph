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
#include "FlexiRankXlator.h"

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
	typedef FlexiRankXlator<DynaDAGLayout> Xlator;
	typedef Xlator::index index;
	typedef std::vector<index> IndexV;
	IndexV newRanks,oldRanks;
	DynaDAGLayout *layout_;

	FlexiRanks(DynaDAGLayout *layout) : layout_(layout) {}
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
		layout_ = o.layout_;
		oldRanks = o.oldRanks;
		newRanks = o.newRanks;
		for(iterator ri = o.begin(); ri!=o.end(); ++ri) {
			Rank *nr = new Rank(**ri);
			insert(nr);
		}
		return *this;
	}
	void backup_x() {
		for(iterator ri = begin(); ri!=end(); ++ri)
			(*ri)->backup_x(); // ugh
	}
	Rank *front() { return *begin(); }
	Rank *back() { return *rbegin(); }
	index Low() {
		if(empty())
			return 0;
		else return Xlator::CoordToRank(layout_,front()->yBase);
	}
	index High() {
		if(empty()) return 0;
		else return Xlator::CoordToRank(layout_,back()->yBase);
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
		Rank q(8.); // stupid set lookup deserves rewrite
		q.yBase = Xlator::RankToCoord(layout_,r);
		return find(&q);
	}
	Rank *GetRank(index r)	{
		iterator ri = GetIter(r);
		if(ri==end())
			return 0;
		else
			return *ri;
	}
	iterator EnsureRank(index r,double sep) {
		dgassert(r!=INT_MAX && r!=INT_MIN); // off bottom or top
		iterator ri = GetIter(r);
		if(ri==end()) {
			Rank *rank = new Rank(sep);
			rank->yBase = Xlator::RankToCoord(layout_,r);
			ri = insert(rank).first;
		}
		return ri;
	}
	void RemoveRank(iterator ri) {
		Rank *del = *ri;
		erase(ri);
		delete del;
	}
	index IndexOfIter(iterator ri) {
		return Xlator::CoordToRank(layout_,(*ri)->yBase);
	}
	void Check();
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // FlexiRanks_h
