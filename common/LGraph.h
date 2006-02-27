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


#ifndef LGRAPH_H
#define LGRAPH_H

#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <stack>
#include <vector>

// convert c-style int compare() to bool less()
template<typename Comp>
struct ComparesLess {
	bool operator()(typename Comp::value_type a,typename Comp::value_type b) const {
		return Comp()(a,b)<0;
	}
};
// cdt iterators (tags) are within the objects so getting the iterator is "free"
// not so for stl (more a point of elegance than performance)
template<typename ST>
struct set_order : ST {
	typename ST::iterator iter(typename ST::value_type x) {
		return find(x);
	}
};

struct ADTisSTL {
	template<typename Edge,typename Node,typename EdgeCompare,typename HeadCompare,typename NodeCompare>
	struct defs {
		typedef set_order<std::set<Edge*,ComparesLess<EdgeCompare> > > edge_order;
		typedef set_order<std::set<Edge*,ComparesLess<HeadCompare> > > edge_by_head_order;
		typedef edge_order inedge_order;
		typedef edge_order outedge_order;

		typedef set_order<std::set<Node*,ComparesLess<NodeCompare> > > node_order;

		struct graph_adtdata {
			node_order m_nodes;
			typename node_order::iterator get_iter(Node *n) {
				return m_nodes.find(n);
			}
		};

		struct edge_parent {};
		struct edge_data {};
		struct node_parent {};
		struct node_data {
			inedge_order m_ins;
			outedge_order m_outs;
			edge_by_head_order m_outFinder; // used exclusively by find_edge(t,h)
			node_data(graph_adtdata &gadt) {}
		};
	};
};

struct Nothing {};

/*
template<typename ADT,typename D,typename GD,typename ND, typename ED,typename GID,typename NID, typename EID>
inline D &gd(LGraph<ADT,GD,ND,ED,GID,NID,EID> *g) {
	return *static_cast<D*>(g->dat);
}
*/
template<typename D,typename GO>
inline D &gd(GO *go) {
	return static_cast<D&>(*go->dat);
}
// extracts instance-specific datum by type
template<typename D,typename GO>
inline D &igd(GO *go) {
	return static_cast<D&>(go->idat);
}

// LGraph graphs, nodes, and edges carry data that has been aggregated using
// multiple inheritance.  the gd function accesses part of a graph, node, or
// edge's data by specifying one of the aggregated classes.

/* because LGraph nodes' and edges' types depend on the data within,
    it's not possible to specify directly that the data will contain pointers to
    other nodes and edges.  a cast is necessary.
    a good way to do this is to use templates:
template<typename T>
struct NodeData {
    T *another_node;
};
typedef NodeData<void> StoredNodeData;
typedef LGraph<... StoredNodeData ...> Graph;
typedef NodeData<Graph::Node *> RealNodeData;

then either use gd2<RealNodeData,StoredNodeData>
or better, specialize gd for your type:
template<>
RealNodeData &gd<RealNodeData,Graph::Node>(Graph::Node *n) {
    return gd2<RealNodeData,StoredNodeData>(n);
}
*/

template<class DOut,class DIn,class GO>
DOut &gd2(GO *go) {
    return *reinterpret_cast<DOut*>(&gd<DIn>(go));
}
template<class DOut,class DIn,class GO>
DOut &igd2(GO *go) {
    return *reinterpret_cast<DOut*>(&igd<DIn>(go));
}

// exceptions
struct LGraphException {};
// when trying to use set operations with subgraphs
struct NoCommonParent : LGraphException {};
struct NullPointer : LGraphException {};
struct IteratorOutOfBounds : LGraphException {};
// attempted to use objects from another graph
struct WrongGraph : LGraphException {};
// attempted to use edge with node it's not connected to
struct WrongNode : LGraphException {};

template<class T>
class pseudo_seq {
	T m_begin,m_end;
public:
    typedef T iterator;
	pseudo_seq(T begin,T end) : m_begin(begin),m_end(end) {}
	T begin() { return m_begin; }
	T end() { return m_end; }
	int size() {
		int ret=0;
		for(T i = m_begin; i!=m_end; ++i) ++ret;
		return ret;
	}
};
template<class ADTPolicy,typename GraphDatum,typename NodeDatum, typename EdgeDatum, // same data in every image
	// different data in different images; gets copied on insert
	typename GraphIDat = Nothing, typename NodeIDat = Nothing, typename EdgeIDat = Nothing>
struct LGraph {
	typedef LGraph<ADTPolicy,GraphDatum,NodeDatum,EdgeDatum,GraphIDat,NodeIDat,EdgeIDat> Graph;
	struct Edge;
	class Node;

	struct Seq {
		int seq, // these numbers keep going up
            id; // these get recycled from erased nodes/edges to inserted (0 <= id < nodes/edges().size())
	};
    template<typename T>
	struct SeqComp {
		typedef T *value_type;
		int operator ()(T *one,T *two) const {
			int s1 = gd<Seq>(one).seq,
				s2 = gd<Seq>(two).seq;
			return s1 - s2;
		}
	};
	struct HeadSeqComp {
        typedef typename Graph::Edge *value_type;
		int operator ()(typename Graph::Edge *e1,typename Graph::Edge *e2) const {
			int s1 = gd<Seq>(e1->head).seq,
				s2 = gd<Seq>(e2->head).seq;
			return s1 - s2;
		}
	};
	struct Edge;
	struct Node;
	typedef typename ADTPolicy::template defs<Edge,Node,SeqComp<Edge>,HeadSeqComp,SeqComp<Node> > ADTDefs;
    struct ND2 : NodeDatum, Seq {
		ND2(const NodeDatum &d) : NodeDatum(d) {}
	};
	struct ED2 : EdgeDatum, Seq {
		ED2(const EdgeDatum &d) : EdgeDatum(d) {}
	};
    struct Edge : ADTDefs::edge_parent
	{
		LGraph * const g;
		ED2 * const dat;
		EdgeIDat idat;
		Node *const tail,*const head;
		Node *other(Node *n) {
			if(n==tail)
				return head;
			else if(n==head)
				return tail;
			else
				return 0;
		}
		bool amMain() { return g->amMain(); }
		template<typename D>
		D &gd() {
			return *static_cast<D*>(dat);
		}
		template<typename D>
		D &igd() {
			return static_cast<D&>(idat);
		}

	private:
		friend struct LGraph<ADTPolicy,GraphDatum,NodeDatum,EdgeDatum,GraphIDat,NodeIDat,EdgeIDat>;
		Edge(LGraph *g, Node *tail, Node *head, ED2 *dat) :
		  g(g),
		  dat(dat),
		  tail(tail),
		  head(head) {
			if(g) {
				if(!tail) throw NullPointer();
				if(!head) throw NullPointer();
				if(!dat) throw NullPointer();
			}
		}
		~Edge() {}
	};
    typedef typename ADTDefs::inedge_order inedge_order;
    typedef typename ADTDefs::outedge_order outedge_order;
    typedef typename ADTDefs::edge_by_head_order edge_by_head_order;
    typedef typename inedge_order::iterator inedge_iter;
	typedef typename ADTDefs::outedge_order::iterator outedge_iter;
	class nodeedge_iter {
		inedge_order *m_inSeq;
		outedge_order *m_outSeq;
		struct itre { // eh, inefficient, but whatever.  can't do union
			typename inedge_order::iterator in;
			typename outedge_order::iterator out;
		} i;
		void goOut() {
			m_inSeq = 0;
			if(m_outSeq->empty())
				m_outSeq = 0;
			else
				i.out = m_outSeq->begin();
		}
		friend struct LGraph;
		nodeedge_iter(inedge_order *ins, outedge_order *outs) : m_inSeq(ins), m_outSeq(outs) {
			if(m_inSeq) {
				if(m_inSeq->empty())
					goOut();
				else
					i.in = m_inSeq->begin();
			}
		}
	public:
		Edge *operator *() {
			if(!m_outSeq)
				return 0;
			else if(m_inSeq)
				return *i.in;
			else
				return *i.out;
		}
		bool headward() {
			return !m_inSeq;
		}
		Node *target() {
			Edge *e = **this;
			if(!e)
				return 0;
			else if(headward())
				return e->head;
			else
				return e->tail;
		}
		nodeedge_iter &operator ++() {
			if(!m_outSeq)
				throw IteratorOutOfBounds();
			if(!m_inSeq) {
				if(++i.out==m_outSeq->end())
					m_outSeq = 0;
			}
			else if(++i.in==m_inSeq->end())
				goOut();
			return *this;
		}
		nodeedge_iter operator ++(int) {
			nodeedge_iter ret = *this;
			operator++();
			return ret;
		}
		bool operator ==(nodeedge_iter other) {
			if(m_inSeq!=other.m_inSeq || m_outSeq!=other.m_outSeq)
				return false;
			if(m_inSeq==0 && m_outSeq==0)
				return true;
			if(m_inSeq==0)
				return i.out==other.i.out;
			else
				return i.in==other.i.in;
		}
		bool operator !=(nodeedge_iter other) {
			return !(*this==other);
		}
	};
	// workaround for circular typing problems w/ friend decl: do not call!
	static nodeedge_iter ne_iter(inedge_order *i,outedge_order *o) {
		return nodeedge_iter(i,o);
	}
    class Node : public ADTDefs::node_parent
	{
	  friend struct LGraph<ADTPolicy,GraphDatum,NodeDatum,EdgeDatum,GraphIDat,NodeIDat,EdgeIDat>;
	  Node(LGraph *g,ND2 *dat) : nodeData_(g->m_adtdata),
	  g(g),
	  dat(dat) {}
	  ~Node() {}

		typename ADTDefs::node_data nodeData_;
	public:
		LGraph * const g;
		ND2 * const dat;
		NodeIDat idat;

		inedge_order &ins() {
			return nodeData_.m_ins;
		}
		outedge_order &outs() {
			return nodeData_.m_outs;
		}
		edge_by_head_order &outFinder() {
			return nodeData_.m_outFinder;
		}
		pseudo_seq<nodeedge_iter> alledges() {
			return pseudo_seq<nodeedge_iter>(ne_iter(&nodeData_.m_ins,&nodeData_.m_outs),ne_iter(0,0));
		};

		int degree() {
			return nodeData_.m_ins.size() + nodeData_.m_outs.size();
		}
		bool amMain() { return g->amMain(); }
		inedge_iter inIter(Edge *e) {
			if(e->head!=this)
				throw WrongNode();
			return nodeData_.m_ins.make_iter(static_cast<typename ADTDefs::inseqlink*>(e));
		}
		inedge_iter outIter(Edge *e) {
			if(e->tail!=this)
				throw WrongNode();
			return nodeData_.m_outs.make_iter(static_cast<typename ADTDefs::outseqlink*>(e));
		}
		template<typename D>
		D &gd() {
			return *static_cast<D*>(dat);
		}
		template<typename D>
		D &igd() {
			return static_cast<D&>(idat);
		}
	};

	typedef std::list<LGraph *> subgraph_list;
	typedef typename subgraph_list::iterator subgraph_iter;

    typedef typename ADTDefs::node_order node_order;
	typedef typename node_order::iterator node_iter;

private:
    typename ADTDefs::graph_adtdata m_adtdata;
	subgraph_list m_subs;
	int m_nNumber, m_eNumber;
    typedef std::stack<int, std::vector<int> > id_stack;
    id_stack m_recycleNodeIds,m_recycleEdgeIds;

public:
	LGraph * const parent;
	GraphDatum * const dat;
	GraphIDat idat;
	LGraph(LGraph *parent=0) :
	  m_nNumber(0), m_eNumber(0),
	  parent(parent),
	  dat(parent?parent->dat:new GraphDatum)
	{
		if(parent)
			parent->m_subs.push_back(this);
	}
	// explicit copy constructor because it's a really bad idea to do memberwise copy!
    LGraph(const LGraph &other) :
	  m_nNumber(0), m_eNumber(0),
	  parent(other.parent),
	  dat(parent?parent->dat:new GraphDatum(*other.dat)) {
		if(parent)
			parent->m_subs.push_back(this);
	    *this = other;
	}
	~LGraph() {
		clear();
		if(parent)
			parent->m_subs.remove(this);
		if(!parent)
			delete dat;
	}
	node_iter iter(Node *n) {
        return m_adtdata.get_iter(n);
	}
	node_order &nodes() {
		return m_adtdata.m_nodes;
	}
	const node_order &nodes() const {
		return m_adtdata.m_nodes;
	}
	subgraph_list &subgraphs() {
		return m_subs;
	}
	template<typename GD>

	// to iterate on all edges in a graph
	struct graphedge_set;
	struct graphedge_iter {
		Edge *operator *() {
			if(!g)
				return 0;
			return *ei;
		}
		graphedge_iter &operator ++() {
			++ei;
			advance();
			return *this;
		}
		graphedge_iter operator ++(int) {
			graphedge_iter ret = *this;
			operator++();
			return ret;
		}
		bool operator ==(graphedge_iter other) {
			return g==0?other.g==0:(g==other.g && ni==other.ni && ei==other.ei);
		}
		bool operator !=(graphedge_iter other) {
			return !(*this==other);
		}
		graphedge_iter() : g(0) {}
	private:
		friend struct LGraph<ADTPolicy,GraphDatum,NodeDatum,EdgeDatum,GraphIDat,NodeIDat,EdgeIDat>;
		graphedge_iter(LGraph *g) : g(g) {
			if(g) {
				if((ni = g->nodes().begin())==g->nodes().end())
					this->g = 0;
				else {
					ei = (*ni)->outs().begin();
					advance();
				}
			}
		}
		void advance() {
			if(!g) throw IteratorOutOfBounds();
			while(ei==(*ni)->outs().end()) {
				if(++ni==g->nodes().end()) {
					g = 0;
					return;
				}
				ei = (*ni)->outs().begin();
			}
		}
		typename node_order::iterator ni;
		typename outedge_order::iterator ei;
		LGraph *g;
	};
	/*
	template<typename D>
	D &gd() {
		return *static_cast<D*>(dat);
	}
	template<typename D>
	D &igd() {
		return static_cast<D&>(idat);
	}
	*/
	pseudo_seq<graphedge_iter> edges() {
		return pseudo_seq<graphedge_iter>(graphedge_iter(this),graphedge_iter(0));
	}
	// methods available only on main graphs
	Node *create_node(const NodeDatum &d = NodeDatum()) {
		if(parent)
			return 0;
		Node *ret = new Node(this,new ND2(d));
		gd<Seq>(ret).seq = ++m_nNumber;
        if(m_recycleNodeIds.size()) {
            gd<Seq>(ret).id = m_recycleNodeIds.top();
            m_recycleNodeIds.pop();
        }
        else
            gd<Seq>(ret).id = m_nNumber-1;
		nodes().insert(ret);
		return ret;
	}
	std::pair<Edge*,bool> create_edge(Node *tail, Node *head,const EdgeDatum &d = EdgeDatum()) {
		if(parent)
			return std::make_pair((Edge*)0,false);
		if(!tail) throw NullPointer();
		if(tail->g!=this) throw WrongGraph();
		if(!head) throw NullPointer();
		if(head->g!=this) throw WrongGraph();
		if(Edge *found = find_edge(tail,head))
			return std::make_pair(found,false);
		Edge *ret = new Edge(this,tail,head,new ED2(d));
		gd<Seq>(ret).seq = ++m_eNumber;
        if(m_recycleEdgeIds.size()) {
            gd<Seq>(ret).id = m_recycleEdgeIds.top();
            m_recycleEdgeIds.pop();
        }
        else
            gd<Seq>(ret).id = m_eNumber-1;
		tail->outs().insert(ret);
		tail->outFinder().insert(ret);
		head->ins().insert(ret);
		return std::make_pair(ret,true);
	}
	// methods available only on subgraphs
	// the shorter, overloaded methods insert,erase,find are intended to
	// mimic std::set<> operations.  it's a little less explicit what you're doing though.
	std::pair<Node*,bool> insert_subnode(Node *n) {
		if(Node *found = find_nodeimage(n))
			return std::make_pair(found,false);
		if(!parent||!parent->insert_subnode(n).first)
			return std::make_pair((Node*)0,false);
		Node *ret = new Node(this,n->dat);
		ret->idat = n->idat;
		nodes().insert(ret);
		return std::make_pair(ret,true);
	}
	std::pair<Edge*,bool> insert_subedge(Edge *e) {
		if(Edge *found = find_edgeimage(e))
			return std::make_pair(found,false);
		if(!parent||!parent->insert_subedge(e).first)
			return std::make_pair((Edge*)0,false);
		Node *t = insert_subnode(e->tail).first,
			*h = insert_subnode(e->head).first;
		Edge *ret = new Edge(this,t,h,e->dat);
		ret->idat = e->idat;
		t->outs().insert(ret);
		t->outFinder().insert(ret);
		h->ins().insert(ret);
		return std::make_pair(ret,true);
	}
	std::pair<Node*,bool> insert(Node *n) {
		return insert_subnode(n);
	}
	std::pair<Edge*,bool> insert(Edge *e) {
		return insert_subedge(e);
	}
	// methods available on both graphs and subgraphs
	bool amMain() {
		return !parent;
	}
	bool empty() {
		return nodes().empty();
	}
	bool erase_node(Node *n) {
		if(n->g!=this)
			if(Node *n2 = find_nodeimage(n))
				n = n2;
			else
				return false;
		for(typename subgraph_list::iterator i = m_subs.begin(); i!=m_subs.end(); ++i)
			(*i)->erase_node(n);
		while(!n->outs().empty())
			erase_edge(*n->outs().begin());
		while(!n->ins().empty())
			erase_edge(*n->ins().begin());
		nodes().erase(n);
        if(!parent) {
            m_recycleNodeIds.push(gd<Seq>(n).id);
			delete n->dat;
        }
		delete n;
		return true;
	}
	bool erase_edge(Edge *e) {
		if(e->head->g!=this)
			if(Edge *e2 = find_edgeimage(e))
				e = e2;
			else
				return false;
		for(typename subgraph_list::iterator i = m_subs.begin(); i!=m_subs.end(); ++i)
			(*i)->erase_edge(e);
		e->tail->outs().erase(e);
		e->tail->outFinder().erase(e);
		e->head->ins().erase(e);
        if(!parent) {
            m_recycleEdgeIds.push(gd<Seq>(e).id);
			delete e->dat;
        }
		delete e;
		return true;
	}
	bool erase(Node *n) {
		return erase_node(n);
	}
	bool erase(Edge *e) {
		return erase_edge(e);
	}
	Edge *find_edge(Node *tail, Node *head) {
		if(tail->g!=this)
			if(!(tail = find_nodeimage(tail)))
				return 0;
		if(head->g!=this)
			if(!(head = find_nodeimage(head)))
				return 0;
		Edge key(0,0,head,0);
		typename edge_by_head_order::iterator i = tail->outFinder().find(&key);
		if(i==tail->outFinder().end())
			return 0;
		else
			return *i;
	}
	Node *find_nodeimage(Node *n) {
		node_iter i = nodes().find(n);
		if(i==nodes().end())
			return 0;
		else
			return *i;
	}
	Edge *find_edgeimage(Edge *e) {
		node_iter i = nodes().find(e->tail);
		if(i==nodes().end())
			return 0;
		outedge_iter j = (*i)->outs().find(e);
		if(j==(*i)->outs().end())
			return 0;
		else
			return *j;
	}
	Node *find(Node *n) {
		return find_nodeimage(n);
	}
	Edge *find(Edge *e) {
		return find_edgeimage(e);
	}
	// another std::set<> like method
	void clear() {
		while(!nodes().empty())
			erase_node(*nodes().begin());
	}
	void check_common_parent(const LGraph &g) {
		const LGraph *p,*p2;
		for(p = parent; p->parent; p = p->parent);
		for(p2 = &g; p2->parent; p2 = p2->parent);
		if(p!=p2)
			throw NoCommonParent();
	}
	// operators
	LGraph &operator|=(const LGraph &cg) { // union
		LGraph &g = const_cast<LGraph&>(cg); // uck
		if(parent) {
			// if a subgraph, we're inserting everything
			check_common_parent(g);
			for(node_iter ni = g.nodes().begin(); ni!=g.nodes().end(); ++ni)
				insert(*ni);
			for(graphedge_iter ei(&g); ei!=graphedge_iter(); ++ei)
				insert(*ei);
		}
		else {
			// if not, we're copying everything
			std::map<Node*,Node*> remember;
			node_iter ni;
			for(ni = g.nodes().begin(); ni!=g.nodes().end(); ++ni) {
				// don't copy Seq, just rest!
				Node *n = create_node(*(*ni)->dat);
				// copy instance data
				n->idat = (*ni)->idat;
				remember[*ni] = n;
			}
			for(ni = g.nodes().begin(); ni!=g.nodes().end(); ++ni)
				for(outedge_iter ei = (*ni)->outs().begin(); ei!=(*ni)->outs().end(); ++ei) {
					Node *t = remember[(*ei)->tail],
						*h = remember[(*ei)->head];
					Edge *e = create_edge(t,h,*(*ei)->dat).first;
					e->idat = (*ei)->idat;
				}
		}
		return *this;
	}
	LGraph &operator&=(LGraph &g) {
		check_common_parent(g);
		for(node_iter ni = nodes().begin(),next=ni; ni!=nodes().end(); ni = next) {
			++next;
			if(!g.find(*ni))
				erase(*ni);
		}
		for(graphedge_iter ei = edges().begin(),nex=ei; ei!=edges().end(); ei = nex) {
			++nex;
		  	if(!g.find(*ei))
				erase(*ei);
		}
		return *this;
	}
	LGraph &operator=(const LGraph &g) {
		clear();
		*this |= g;
		*dat = *g.dat;
		idat = g.idat;
		return *this;
	}
	LGraph operator |(LGraph &g) {
		LGraph ret = *this;
		ret |= g;
		return ret;
	}
	LGraph operator &(LGraph &g) {
		LGraph ret = *this;
		ret &= g;
		return ret;
	}
};

#endif
