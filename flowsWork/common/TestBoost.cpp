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


#include <boost/graph/graph_concepts.hpp>
#include <common/LGraph-cdt.h>
#include <common/BoostLGraph.h>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <functional>

template<typename T>
struct NData {
	int id,dist;
	T *pred;
};
struct EData {
	int weight;
	EData() : weight(2) {}
};
typedef LGraph<ADTisCDT,Nothing,NData<void>,EData> DijGraph;

typedef NData<DijGraph::Node> DijData;
template<>
DijData &gd<DijData,DijGraph::Node>(DijGraph::Node *n) {
    return gd2<DijData,NData<void> >(n);
}
typedef boost::MemberAccessor<DijGraph::Node*,DijData,DijGraph::Node*,&DijData::pred> PredAcc;
typedef boost::MemberAccessor<DijGraph::Node*,DijData,int,&DijData::dist> DistAcc;
typedef boost::MemberAccessor<DijGraph::Edge*,EData,int,&EData::weight> WeightAcc;
typedef boost::MemberAccessor<DijGraph::Node*,DijData,int,&DijData::id> IndexAcc;

using namespace boost;
int main() {
	function_requires< VertexListGraphConcept<DijGraph> >();
	function_requires< BidirectionalGraphConcept<DijGraph> >();
	function_requires< MutableGraphConcept<DijGraph> >();

	DijGraph g;

	DijGraph::Node *n[30];
	for(int i=0; i<30; ++i) {
		n[i] = g.create_node();
		gd<DijData>(n[i]).id = i;
	}
	for(int i=0; i<30; ++i)
		g.create_edge(n[i],n[(i+5)%30]);

	dijkstra_shortest_paths(g,n[0],PredAcc(),DistAcc(),WeightAcc(),IndexAcc(),
		std::less<int>(),std::plus<int>(),std::numeric_limits<int>::max(),0,dijkstra_visitor<null_visitor>());
	return 0;
}
