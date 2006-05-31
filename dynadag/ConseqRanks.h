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

#ifndef ConseqRanks_h
#define ConseqRanks_h

#include "Rank.h"

namespace Dynagraph {
namespace DynaDAG {

template<typename Layout>
struct ConseqRankXlator {
	typedef int index;
	bool Above(Layout *l,index a,index b) {
		return a<b;
	}
	bool Below(Layout *l,index a,index b) {
		return a>b;
	}
	index CoordToRank(Layout *l,double y) {
#ifndef DOWN_GREATER
		return -ROUND(y/gd<GraphGeom>(l).resolution.y);
#else
		return ROUND(y/gd<GraphGeom>(l).resolution.y);
#endif
	}
	double RankToCoord(Layout *l,index r) {
#ifndef DOWN_GREATER
		return -r*gd<GraphGeom>(l).resolution.y;
#else
		return r*gd<GraphGeom>(l).resolution.y;
#endif
	}
	index HeightToDRank(Layout *l,double dy) {
		return ROUND(dy/gd<GraphGeom>(l).resolution.y);
	}
};

struct ConseqRanks : std::vector<Rank*> {
	typedef int index;
	double sep;
	ConseqRanks(double div,double sep) : sep(sep),low(0),high(-1) {}
	ConseqRanks(ConseqRanks &o) {
		*this = o;
	}
	ConseqRanks &operator =(ConseqRanks &o) {
		clear();
		sep = o.sep;
		low = o.low;
		high = o.high;
		for(iterator ri = o.begin(); ri!=o.end(); ++ri)
			push_back(new Rank(**ri));
		return *this;
	}
	index Low() { return low; }
	index High() { return high; }
	bool Above(index a,index b) {
		return a<b;
	}
	bool Below(index a,index b) {
		return a>b;
	}
	index Up(index r) {
		return r-1;
	}
	index Down(index r) {
		return r+1;
	}
	iterator GetIter(index r) {
		if(low > high || r < low || r > high)
			return end();
		return begin() + r - low;
	}
	Rank *GetRank(index r)	{
		iterator ri = GetIter(r);
		if(ri==end())
			return 0;
		else
			return *ri;
	}
	iterator EnsureRank(index r,double sep) {
		if(r < low || r > high)
			if(low <= high)
				extendConfig(std::min(r,low),std::max(r,high),sep);
			else // first rank
				extendConfig(r,r,sep);
		return GetIter(r);
	}
	index IndexOfIter(iterator ri) {
		return index(ri-begin()+low);
	}
	index MapCoordToRank(double y) {
		if(empty()) {		/* no config exists yet */
			index ret = 0;
			Rank *rank = *EnsureRank(ret);
			rank->yBase = y;
			return ret;
		}
		Rank *rank = front();
	#ifndef DOWN_GREATER
		if(y > rank->yAbove(1.0)) /* above min rank */
			return ROUND(low - (y - rank->yBase) / rank->Height());
	#else
		if(y < rank->yAbove(1.0)) /* above min rank */
			return ROUND(low - (rank->yBase - y) / rank->Height());
	#endif
		rank = back();
	#ifndef DOWN_GREATER
		if(y < rank->yBelow(1.0)) /* below max rank */
			return ROUND(high + (rank->yBase - y) / rank->Height());
	#else
		if(y > rank->yBelow(1.0)) /* below max rank */
			return ROUND(high + (y - rank->yBase) / rank->Height());
	#endif

		/* somewhere in between */
		index bestrank = low;
		double bestdist = DBL_MAX;
		for(iterator ri = begin(); ri!=end(); ++ri) {
			double d = absol(y - (*ri)->yBase);
			if(d < bestdist) {
				bestdist = d;
				bestrank = index(ri-begin()+low);
			}
		}
		return bestrank;
	}
	void Check();
private:
	index low,high;
    Rank* &at(int i) { return operator[](i); } // for earlier gccs
	void extendConfig(int newLow, int newHigh,double sep) {
		int osize = high<low ? 0 : high - low + 1,
			nsize = newHigh - newLow + 1;
		int where = int(newLow < low ? 0 : size());
		// make some sorta vague initial Ys
		double dy,y;
		if(osize) {
		  dy = where?-at(high-low)->Height():at(0)->Height(),
		  y = where?at(high-low)->yBase:at(0)->yBase;
		}
		else {
		  assert(nsize==1);
		  y = dy = -17; // will get reset
		}
		insert(begin()+where,nsize-osize,(Rank*)0);
		for(int i = 0; i < nsize-osize; ++i) {
		  y += dy;
		  (at(where+i) = new Rank(sep))->yBase = y;
		}
		low = newLow;
		high = newHigh;
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // ConseqRanks_h
