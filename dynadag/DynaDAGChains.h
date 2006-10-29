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

#ifndef DynaDAGChains_h
#define DynaDAGChains_h

namespace Dynagraph {
namespace DynaDAG {

// these templates are a workaround for the circular typing problem
// DDNode and DDEdge cannot refer to the graph made with them.
template<typename N,typename E>
struct Chain {
	E *first,*last;
	Chain() : first(0),last(0) {}
	struct edge_iter {
		E *operator *() {
			return i;
		}
		edge_iter &operator ++() {
			if(i==chain->last)
				i = 0;
			else {
				dgassert(i);
				i = *i->head->outs().begin();
			}
			return *this;
		}
		edge_iter operator ++(int) {
			edge_iter ret = *this;
			operator++();
			return ret;
		}
		bool operator ==(const edge_iter &it) const {
			return it.chain==chain && it.i==i;
		}
		bool operator !=(const edge_iter &it) const {
			return !(*this==it);
		}
		edge_iter &operator=(const edge_iter &it) {
			i = it.i;
			chain = it.chain;
			return *this;
		}
		friend struct Chain<N,E>;
		Chain *chain; // public cuz I can't do friend struct Chain<N,E>::node_iter; (in msvc++?)
		edge_iter() {}
	private:
		edge_iter(Chain *chain,E *i) : chain(chain), i(i) {
			dgassert(!i || i->head->g); // make sure valid pointer
		}
		E *i;
	};
	struct node_iter {
		N *operator *() {
			return *ei?(*ei)->head:0;
		}
		node_iter &operator ++() {
			++ei;
			skipLast();
			return *this;
		}
		node_iter operator ++(int) {
			node_iter ret = *this;
			operator++();
			return ret;
		}
		bool operator ==(const node_iter &it) const {
			return ei==it.ei;
		}
		bool operator !=(const node_iter &it) const {
			return !(*this==it);
		}
		friend struct Chain<N,E>;
	private:
		node_iter(edge_iter ei) : ei(ei) {
			skipLast();
		}
		edge_iter ei;
		void skipLast() {
			if(*ei==ei.chain->last)
				++ei;
		}
	};
	edge_iter eBegin() {
		return edge_iter(this,first);
	}
	edge_iter eEnd() {
		return edge_iter(this,0);
	}
	node_iter nBegin() {
		return node_iter(eBegin());
	}
	node_iter nEnd() {
		return node_iter(eEnd());
	}
};
template<typename N,typename E>
struct MultiNode : Chain<N,E> {
	// one node: first==last==0, node!=0
	// multi-node: first,last!=0, node==0
	N *node;
	DynaDAGLayout::Node *layoutN;
	NodeConstraints xcon;
	//	coordFixed; // whether a good place in X has been found
	MultiNode() : node(0),layoutN(0) /*,coordFixed(false)*/ {}
	struct node_iter {
		N *operator *() {
			switch(state) {
			case Mine:
				return MN()->node;
			case Tail:
				return MN()->first->tail;
			case Heads:
				return *ei?(*ei)->head:0;
			default:
				return 0;
			}
		}
		node_iter &operator ++() {
			switch(state) {
			case Mine:
			case Tail:
				state = Heads;
				return *this;
			case Heads:
				++ei;
				return *this;
			default:
				dgassert(0);
				return *this;
			}
		}
		node_iter operator ++(int) {
			node_iter ret = *this;
			operator++();
			return ret;
		}
		bool operator ==(const node_iter &it) const {
			return state==it.state && ei==it.ei;
		}
		bool operator !=(const node_iter &it) const {
			return !(*this==it);
		}
		node_iter &operator=(const node_iter it) {
			ei = it.ei;
			state = it.state;
			return *this;
		}
		friend struct MultiNode<N,E>;
		node_iter() {}
	private:
		node_iter(typename Chain<N,E>::edge_iter ei,bool end) : ei(ei) {
			if(end)
				state = Heads;
			else if(MN()->node)
				state = Mine;
			else if(MN()->first)
				state = Tail;
			else
				state = Heads;
		}
		MultiNode *MN() { return static_cast<MultiNode*>(ei.chain); }
		enum State {Mine,Tail,Heads} state;
		typename Chain<N,E>::edge_iter ei;
	};
	node_iter nBegin() {
		return node_iter(Chain<N,E>::eBegin(),false);
	}
	node_iter nEnd() {
		return node_iter(Chain<N,E>::eEnd(),true);
	}
	Position pos() {
		if(!top() || !gd<DDNode>(top()).cur.valid)
			return Position();
		return Position(gd<DDNode>(top()).cur.x,(gd<DDNode>(top()).cur.y+gd<DDNode>(bottom()).cur.y)/2.0);
	}
	N *top() {
		return node?node:Chain<N,E>::first->tail;
	}
	N *bottom() {
		return node?node:Chain<N,E>::last->head;
	}
	int len() {
		int n=0;
		for(node_iter ni = Chain<N,E>::nBegin(); ni!=Chain<N,E>::nEnd(); ++ni,++n);
		return n;
	}
};
// if a single edge, first=last=the edge
template<typename N,typename E>
struct Path : Chain<N,E> {
	// self or flat: first==last==0; use layoutE to figure out ends
	DynaDAGLayout::Edge *layoutE;
	Line unclippedPath;
	Path() : layoutE(0) {}
};

inline DDModel::Node *cutNode(DDPath *path) {
	Position ret;
	int suppressRank = gd<Suppression>(path->layoutE).suppressRank;
	for(DDPath::node_iter ni = path->nBegin(); ni!=path->nEnd(); ++ni) 
		if(gd<DDNode>(*ni).rank==suppressRank) 
			return *ni;
	/*
	std::cerr << "no suppressRank " << suppressRank << " in ";
	for(DDPath::node_iter ni = path->nBegin(); ni!=path->nEnd(); ++ni) 
		std::cerr << gd<DDNode>(*ni).rank << ' ';
	std::cerr << std::endl;
	*/
	return 0;
}
inline Position cutPos(DDPath *path) {
	Position ret;
	if(DDModel::Node *n = cutNode(path))
		ret = gd<DDNode>(n).cur;
	return ret;
}

} // namespace Dynagraph
} // namespace DynaDAG

#endif // DynaDAGChains_h
