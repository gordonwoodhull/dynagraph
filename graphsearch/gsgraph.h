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


template<typename T>
struct DijNDataT {
	int weight,dist;
	T *prev;
};
struct DijEData {
	int weight;
	EData() : weight(0) {}
};

struct GSNode : DijNData<void>,NamedAttrs {
    GSNode(DString s) : NamedAttrs(s) {}
};
struct GSEdge : DijEData,NamedAttrs {
    GSEdge(DString s) : NamedAttrs(s) {}
};

typedef NamedGraph<ADTisCDT,Nothing,GSNode,GSEdge> GSGraph;

typedef DijNDataT<GSGraph::Node> DijNData;
template<>
DijNData &gd<DijNData,GSGraph::Node>(GSGraph::Node *n) {
    return gd2<DijNData,DijNDataT<void> >(n);
}
typedef boost::MemberAccessor<GSGraph::Node*,DijNData,GSGraph::Node*,&DijNData::prev> PrevAcc;
typedef boost::MemberAccessor<GSGraph::Node*,DijNData,int,&DijNData::dist> DistAcc;
typedef boost::MemberAccessor<GSGraph::Edge*,EData,int,&EData::weight> WeightAcc;
typedef boost::MemberAccessor<GSGraph::Node*,Seq,int,&Seq::id> IndexAcc;

