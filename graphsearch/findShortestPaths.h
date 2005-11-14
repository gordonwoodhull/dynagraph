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


#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "common/BoostLGraph.h"

template<class G>
void findShortestPaths(G &top,G &a,G &b,DString weightattr,int limit,G &ret) {
	if(a.empty() || b.empty())
		return;
	std::vector<typename G::Node *> pred;
	std::vector<int> dist,weight;
	pred.resize(top.nodes().size());
	dist.resize(top.nodes().size());
	weight.resize(top.edges().size());
	for(typename G::graphedge_iter ei = top.edges().begin(); ei!=top.edges().end(); ++ei) {
		DString wt = gd<StrAttrs>(*ei)[weightattr];
		weight[gd<typename G::Seq>(*ei).id] = wt.size()?atoi(wt.c_str()):0;
	}
	typedef boost::PointerAccessor<G,typename G::Node*,typename G::Node*> PredAcc;
	typedef boost::PointerAccessor<G,typename G::Node*,int> DistAcc;
	typedef boost::PointerAccessor<G,typename G::Edge*,int> WeightAcc;
	typedef boost::MemberAccessor<typename G::Node*,typename G::Seq,int,&G::Seq::id> IndexAcc;
	boost::dijkstra_shortest_paths(top,top.find(*a.nodes().begin()),PredAcc(&pred[0]),DistAcc(&dist[0]),WeightAcc(&weight[0]),IndexAcc(),
		std::less<int>(),std::plus<int>(),std::numeric_limits<int>::max(),0,boost::dijkstra_visitor<boost::null_visitor>());
	for(typename G::node_iter ni = b.nodes().begin(); ni!=b.nodes().end(); ++ni) {
		typename G::Node *n = top.find(*ni),*m;
		while((m = pred[gd<typename G::Seq>(n).id])!=n) {
			ret.insert(top.find_edge(m,n));
			n = m;
		}
		if(limit>0 && ret.nodes().size()>size_t(limit))
			break;
	}
}
