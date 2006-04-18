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

#include "common/Transform.h"
extern Dynagraph::Transform *g_transform;

namespace Dynagraph {

template<typename Layout>
struct Stepper {
	virtual void Step(ChangeQueue<Layout> &Q) = 0;
	virtual bool Done() = 0;
	virtual ~Stepper() {} // to evade warnings for derived
};

template<typename Layout>
class Wandering : public Stepper<Layout> {
	typedef std::pair<typename Layout::Node*,typename Layout::Edge*> gob; // maybe both; neither=>done
	typename Layout::node_iter ni;
	Layout *l;
	typename Layout::Node *n;
	typename Layout::Edge *e;
	Layout hit;
	typename Layout::Edge *next_edge(typename Layout::Node *n) {
		for(typename Layout::outedge_iter ei = n->outs().begin(); ei!=n->outs().end();++ei)
			if(!hit.find(*ei))
				return *ei;
		return 0;
	}
	gob next_thing() {
		for(;ni!=l->nodes().end(); ++ni) {
			if(!hit.find(*ni))
				return gob(*ni,0);
			if(typename Layout::Edge *e = next_edge(*ni)) {
				if(!hit.find(e->head))
					return gob(e->head,e);
				else
					return gob(0,e);
			}
		}
		return gob(0,0);
	}
	void advance(typename Layout::Node *curr) {
		if((e = next_edge(curr))) {
			n = 0;
			if(!hit.find(e->head))
				n = e->head;
		}
		else {
			gob t = next_thing();
			n = t.first;
			e = t.second;
		}
	}
public:
	Wandering(Layout *l) : l(l),hit(l) {
		ni = l->nodes().begin();
		if(ni!=l->nodes().end())
			n = *ni;
		else
			n = 0;
		e = 0;
	}
	void Step(ChangeQueue<Layout> &Q) {
		if(n && !e) { // starting from a node
			assert(hit.insert(n).second);
			Q.InsNode(n);
			advance(n);
		}
		else if(n && e) { // travelling to a new node: first return node
			assert(hit.insert(n).second);
			Q.InsNode(n);
			n = 0;
		}
		else if(!n && e) { // then edge
			assert(hit.insert(e).second);
			Q.InsEdge(e);
			advance(e->head);
		}
		else {
			assert(!n && !e);
		}
	}
	bool Done() {
		return !n && !e;
	}
};
template<typename Layout>
class NodesFirst : public Stepper<Layout> {
	Layout *l;
	typename Layout::node_iter ni;
	typename Layout::graphedge_iter ei;
public:
	NodesFirst(Layout *l) : l(l),ni(l->nodes().begin()),ei(l->edges().begin()) {}
	void Step(ChangeQueue<Layout> &Q) {
		if(ni!=l->nodes().end())
			Q.InsNode(*ni++);
		else if(ei!=l->edges().end())
			Q.InsEdge(*ei++);
	}
	bool Done() {
		return ni==l->nodes().end() && ei==l->edges().end();
	}
};
template<typename Layout>
class Batch : public Stepper<Layout> {
	Layout *l;
	bool  done;
public:
	Batch(Layout *l) : l(l),done(false) {}
	void Step(ChangeQueue<Layout> &Q) {
		if(!done) {
			for(typename Layout::node_iter ni = l->nodes().begin(); ni!=l->nodes().end(); ++ni)
				Q.InsNode(*ni);
			for(typename Layout::graphedge_iter ei = l->edges().begin(); ei!=l->edges().end(); ++ei)
				Q.InsEdge(*ei);
			done = true;
		}
	}
	bool Done() {
		return done;
	}
};
template<typename Layout,typename Trav>
class TraversalStep : public Stepper<Layout> {
	Trav i;
public:
	TraversalStep(Layout *l) : i(l,true) {}
	bool Done() {
		return i.stopped();
	}
	void Step(ChangeQueue<Layout> &Q) {
		typename Trav::V ins = *i;
		if(ins.n)
			Q.InsNode(ins.n);
		if(ins.e)
			Q.InsEdge(ins.e);
		++i;
	}
};
template<typename Layout>
inline void init_node(typename Layout::Node *n) {
    // give it default attributes by assigning it.. nothing!
	StrAttrs attrs;
	// template parameter deduction doesn't work here?  must learn more
	stringsIn<Layout>(g_transform,n,attrs,false);  
}
template<typename Layout>
inline typename Layout::Node *randomNode(Layout *l) {
	int N = l->nodes().size();
	if(!N)
		return 0;
	int i = rand()%N;
	typename Layout::node_iter ni = l->nodes().begin();
	while(i--) ++ni;
	return *ni;
}
// this keeps an array of nodes which might have new edges... selects one randomly, looks for the new edge
// if there, it inserts it.  otherwise it removes the node from the array.
template<typename Layout>
class RandomTraversal : public Stepper<Layout> {
	typedef std::vector<typename Layout::Node*> Nodes;
	Nodes nodes;
	Layout *l;
	unsigned m_hitpos;
	bool started;
public:
	RandomTraversal(Layout *l) : l(l) {
		for(m_hitpos = 0;m_hitpos<MAX_TRAVERSAL;++m_hitpos)
			if(!gd<Hit>(l)[m_hitpos])
				break;
		assert(m_hitpos<MAX_TRAVERSAL);
		gd<Hit>(l)[m_hitpos] = true;
		started = false;
		if(l->nodes().begin()==l->nodes().end()) // empty
			started = true;
	}
	void Step(ChangeQueue<Layout> &Q) {
		if(!started) {
			typename Layout::Node *n = randomNode(l);
			if(n) {
				Q.InsNode(n);
				gd<Hit>(n)[m_hitpos] = true;
				nodes.push_back(n);
				started = true;
			}
		}
		else if(nodes.size()) {
			while(nodes.size()) {
				int i = rand()%nodes.size();
				typename Layout::Node *n = nodes[i];
				for(typename Layout::nodeedge_iter ei = n->alledges().begin(); ei!=n->alledges().end(); ++ei)
					if(!gd<Hit>(*ei)[m_hitpos]) {
						typename Layout::Node *o = (*ei)->other(n);
						if(!gd<Hit>(o)[m_hitpos]) {
							Q.InsNode(o);
							gd<Hit>(o)[m_hitpos] = true;
							nodes.push_back(o);
						}
						Q.InsEdge(*ei);
						gd<Hit>(*ei)[m_hitpos] = true;
						return;
					}
					nodes.erase(nodes.begin()+i); // no new edges found so
			}
		}
	}
	bool Done() {
		return started && nodes.empty();
	}
};
template<typename Layout>
class RandomDrawer : public Stepper<Layout> {
	Layout *l;
	int limit;
	double reconnectProbability;
public:
	RandomDrawer(Layout *l,int limit,double reconnectProbability) : l(l),limit(limit),reconnectProbability(reconnectProbability) { srand(17); }
	void Step(ChangeQueue<Layout> &Q) {
		if(!limit)
			return;
		int N = l->nodes().size();
		int E = l->edges().size();
		char buf[10];
		sprintf(buf,"e%d",E);
		Name ename = buf;
		double r = double(rand())/double(RAND_MAX);
		std::pair<typename Layout::Edge*,bool> newe;
		if(N>3 && r<reconnectProbability) {
			typename Layout::Node *t,*h;
			do h = randomNode(l),t = randomNode(l);
			while(h==t || l->find_edge(h,t)||l->find_edge(t,h));
			newe = l->create_edge(t,h,ename);
			assert(newe.second);
			Q.InsEdge(newe.first);
		}
		else {
			char buf[10];
			sprintf(buf,"n%d",N+1);
			typename Layout::Node *t = randomNode(l),
				*h = l->create_node(buf);
			if(!t) {
				sprintf(buf,"n%d",(int)l->nodes().size());
				t = l->create_node(buf);
				init_node<Layout>(t);
				Q.InsNode(t);
			}
			Q.InsNode(h);
			newe = l->create_edge(t,h,ename);
			assert(newe.second);
			Q.InsEdge(newe.first);
		}
		gd<Name>(newe.first) = buf;
		limit--;
	}
	bool Done() {
		return limit==0;
	}
};

} // namespace Dynagraph
