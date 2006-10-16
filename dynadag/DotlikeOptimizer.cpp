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

//#define DOUBLE_MEDIANS
#ifdef DOUBLE_MEDIANS
#define med_type double
#define cvt_med
#define med_eq(m1,m2) (fabs(m1-m2)<0.25)
#else
#define med_type int
#define cvt_med ROUND
#define med_eq(m1,m2) (m1==m2)
#endif

struct OrderConstraintSwitchable {
	bool canSwitch(DDModel::Node *l,DDModel::Node *r) {
		return gd<DDNode>(l).orderConstraint<0 || gd<DDNode>(r).orderConstraint<0 ||
			gd<DDNode>(l).orderConstraint > gd<DDNode>(r).orderConstraint;
	}
};
struct MedianCompare {
	UpDown m_dir;
	bool m_allowEqual;
	MedianCompare(UpDown dir,bool allowEqual) : m_dir(dir),m_allowEqual(allowEqual) {}
	bool comparable(DDModel::Node *n) {
		return MValExists(n,m_dir);
	}
	bool shouldSwitch(DDModel::Node *l,DDModel::Node *r) {
		med_type lm = cvt_med(MVal(l,m_dir)),
			rm = cvt_med(MVal(r,m_dir));
		return lm > rm || lm==rm && m_allowEqual;
	}
};
struct CrossingCompare {
	Config &config;
	SiftMatrix &matrix;
	bool m_allowEqual;
	CrossingCompare(Config &config,SiftMatrix &matrix,bool allowEqual) : config(config),matrix(matrix),m_allowEqual(allowEqual) {}
	bool comparable(DDModel::Node *n) {
		return true;
	}
	bool shouldSwitch(DDModel::Node *l,DDModel::Node *r) {
		dgassert(gd<DDNode>(l).rank==gd<DDNode>(r).rank);
		dgassert(gd<DDNode>(l).order<gd<DDNode>(r).order);
		Rank *rank = config.ranking.GetRank(gd<DDNode>(l).rank);
		int numcross=0;
		for(int o = gd<DDNode>(l).order; o<=gd<DDNode>(r).order; ++o)
			numcross += matrix.allCrossings(rank->order[o],l) - matrix.allCrossings(l,rank->order[o]);
		return numcross<0 || numcross==0&&m_allowEqual;
	}
};
void moveBefore(Config &config,SiftMatrix &matrix,DDModel::Node *n,DDModel::Node *before) {
	matrix.move(n,before);
	int rank = gd<DDNode>(n).rank;
	config.RemoveNode(n);
	if(before)
		config.InstallAtOrder(n,rank,gd<DDNode>(before).order);
	else
		config.InstallAtRight(n,rank);
}
template<class Switchable, class Compare>
void bubblePassR(Config &config,SiftMatrix &matrix,Rank *r,Switchable &switchable,Compare &compare) {
	NodeV::iterator li;
	for(li = r->order.begin(); li<r->order.end()-1; ++li) {
		if(!compare.comparable(*li))
			continue;
		// search to right: if you find something you can't switch, go on to next li
		// if you find something to switch, jump to place it's been put
		for(NodeV::iterator ri = li+1;ri!=r->order.end(); ++ri)
			if(!switchable.canSwitch(*li,*ri))
				break;
			else if(compare.comparable(*ri)) {
				if(compare.shouldSwitch(*li,*ri)) {
					moveBefore(config,matrix,*li,(ri==r->order.end()-1)?0:*(ri+1));
					li = ri;
				}
				break;
			}
	}
}
template<class Switchable, class Compare>
void bubblePassL(Config &config,SiftMatrix &matrix,Rank *r,Switchable &switchable,Compare &compare) {
	NodeV::reverse_iterator ri;
	for(ri = r->order.rbegin(); ri<r->order.rend()-1; ++ri) {
		if(!compare.comparable(*ri))
			continue;
		for(NodeV::reverse_iterator li = ri+1;li!=r->order.rend(); ++li)
			if(!switchable.canSwitch(*li,*ri))
				break;
			else if(compare.comparable(*li)) {
				if(compare.shouldSwitch(*li,*ri)) {
					moveBefore(config,matrix,*ri,*li);
					ri = li-1;
				}
				break;
			}
	}
}
template<class Switchable, class Compare>
void bubblePass(Config &config,SiftMatrix &matrix,const vector<int> &ranks,UpDown dir,LeftRight way,Switchable &switchable,Compare &compare) {
	if(dir==DOWN)
		for(vector<int>::const_iterator ri = ranks.begin(); ri!=ranks.end(); ++ri) {
			Rank *r = config.ranking.GetRank(*ri);
			if(way==RIGHT)
				bubblePassR(config,matrix,r,switchable,compare);
			else
				bubblePassL(config,matrix,r,switchable,compare);
		}
	else
		for(vector<int>::const_reverse_iterator ri = ranks.rbegin(); ri!=ranks.rend(); ++ri) {
			Rank *r = config.ranking.GetRank(*ri);
			if(way==RIGHT)
				bubblePassR(config,matrix,r,switchable,compare);
			else
				bubblePassL(config,matrix,r,switchable,compare);
		}
	//matrix.check();
}
struct RankLess {
	bool operator()(DDModel::Node *u,DDModel::Node *v) {
		if(gd<DDNode>(u).rank == gd<DDNode>(v).rank)
			return gd<DDNode>(u).order < gd<DDNode>(v).order;
		return gd<DDNode>(u).rank < gd<DDNode>(v).rank;
	}
};
#define TIRE 6
void DotlikeOptimizer::Reorder(DynaDAGLayout &nodes,DynaDAGLayout &edges) {
	vector<int> affectedRanks;
	{
		NodeV optimVec;
		getCrossoptModelNodes(nodes,edges,optimVec);
		if(optimVec.empty())
			return;
		sort(optimVec.begin(),optimVec.end(),RankLess());
		NodeV::iterator wot = optimVec.begin();
		for(Config::Ranks::iterator ri = config.ranking.begin(); ri!=config.ranking.end(); ++ri)
			for(NodeV::iterator ni = (*ri)->order.begin(); ni!=(*ri)->order.end(); ++ni)
				if(wot!=optimVec.end() && *ni==*wot) {
					gd<DDNode>(*ni).orderConstraint = -1;
					++wot;
				}
				else
					gd<DDNode>(*ni).orderConstraint = gd<DDNode>(*ni).order;
		dgassert(wot==optimVec.end());
		loops.Field(dgr::crossopt,"model nodes for crossopt",optimVec.size());
		loops.Field(dgr::crossopt,"total model nodes",optimVec.front()->g->nodes().size());

		for(NodeV::iterator ni = optimVec.begin(); ni!=optimVec.end(); ++ni)
			if(affectedRanks.empty() || affectedRanks.back()!=gd<DDNode>(*ni).rank)
				affectedRanks.push_back(gd<DDNode>(*ni).rank);
		loops.Field(dgr::crossopt,"ranks for crossopt",affectedRanks.size());
		loops.Field(dgr::crossopt,"total ranks",config.ranking.size());
	}
	SiftMatrix matrix(config),backupM(config);
	MedianCompare median(DOWN,false);
	CrossingCompare crossing(config,matrix,false);
	OrderConstraintSwitchable switchable;

	config.checkX();
	config.ranking.backup_x();
	Config::Ranks backupC = config.ranking;
	// optimize once ignoring node crossings (they can scare the sifting alg)
	// in a sec we'll sift using the node penalty to clean up
	matrix.m_light = true;
	matrix.recompute();
	backupM = matrix;
	unsigned best = matrix.sumCrossings(),bestPass=0;
	loops.Field(dgr::crossopt,"crossings before crossopt",best);
	unsigned passes = 32,pass=0,score=0;
	while(pass<passes && best) {
		int tired = 0;
		while(pass<passes && tired<TIRE) {
				dgassert(matrix.m_light);
			median.m_allowEqual = pass%8>4;
			crossing.m_allowEqual = !median.m_allowEqual;
			LeftRight way = (pass%2) ? RIGHT : LEFT;
			UpDown dir;
				dgassert(matrix.m_light);
			if(pass%4<2) {
				median.m_dir = UP;
				dir = DOWN;
			}
			else {
				median.m_dir = DOWN;
				dir = UP;
			}
			bubblePass(config,matrix,affectedRanks,dir,way,switchable,median);
				dgassert(matrix.m_light);

			// something in the next dozen lines crashes MS Visual C++ .NET
			// if run-time checks are enabled so i've disabled them for this file - ?
			unsigned score2 = matrix.sumCrossings();
			do {
				score = score2;
				dgassert(matrix.m_light);
				bubblePass(config,matrix,affectedRanks,dir,way,switchable,crossing);
				score2 = matrix.sumCrossings();
				dgassert(score2<=score);
			}
			while(score2<score);
			score = score2;

			if(reports.enabled(dgr::crossopt)) {
				char buf[10];
				sprintf(buf,"crossings pass %d",pass);
				loops.Field(dgr::crossopt,buf,score);
			}

			if(score<best) {
				config.checkX();
				config.ranking.backup_x();
				backupC = config.ranking;
				backupM = matrix;
				best = score;
				bestPass = pass;
				tired = 0;
			}
			else
				tired++;
			pass++;
		}
		if(score>best || tired==TIRE) {
			config.Restore(backupC);
			//matrix.recompute();
			matrix = backupM;
			tired = 0;
		}
	}
	if(reports.enabled(dgr::crossopt))
		for(;pass<passes;++pass) {
			char buf[10];
			sprintf(buf,"crossings pass %d",pass);
			loops.Field(dgr::crossopt,buf,0);
		}

	if(score>=best) {
		config.Restore(backupC);
	}
	loops.Field(dgr::crossopt,"model edge crossings",best);

	// sift out node crossings
	matrix.m_light = false;
	matrix.recompute();
	score = matrix.sumCrossings();
	loops.Field(dgr::crossopt,"weighted crossings before heavy pass",score);
	/*
	reports[dgr::crossopt] << "%d node-node crossings; %d node-edge crossings; %d edge-edge crossings\n",score/(NODECROSS_PENALTY*NODECROSS_PENALTY),
		(score/NODECROSS_PENALTY)%NODECROSS_PENALTY,score%NODECROSS_PENALTY);
	*/
	if(score<NODECROSS_PENALTY) {
		loops.Field(dgr::crossopt,"weighted crossings after heavy pass",-1);
		return;
	}
	pass = 0;
	passes = 4;
	crossing.m_allowEqual = true;
	// sifting out upward or downward may be better - try both.
	bool improved;
	do {
		improved = false;
		backupC = config.ranking;
		bubblePass(config,matrix,affectedRanks,DOWN,RIGHT,switchable,crossing);
		unsigned down = matrix.sumCrossings();
		Config::Ranks backup2 = config.ranking;
		config.Restore(backupC);
		matrix.recompute();
		bubblePass(config,matrix,affectedRanks,UP,RIGHT,switchable,crossing);
		unsigned up = matrix.sumCrossings();
		if(down<score && down<up) {
			config.Restore(backup2);
			matrix.recompute();
			score = down;
			improved = true;
		}
		else if(up<score) {
			score = up;
			improved = true;
		}
	}
	while(improved);
	loops.Field(dgr::crossopt,"weighted crossings after heavy pass",score);
	// absolutely must not leave here with nodes crossing nodes!!
	dgassert(score<NODECROSS_PENALTY*NODECROSS_PENALTY);
}
double DotlikeOptimizer::Reopt(DDModel::Node *n,UpDown dir) {
	return gd<DDNode>(n).cur.x;
}

} // namespace DynaDAG
} // namespace Dynagraph
