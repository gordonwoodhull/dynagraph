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



#include "common/LayoutAttrs.h"
#include "common/ChangeQueue.h"
#include "common/DynagraphServer.h"
#include "common/stringsIn.h"
#include "common/stringsOut.h"
#include "common/emitGraph.h"
//#include "common/engine_replacement.h"
#include "createLayoutServer.h"
#include "common/randomName.h"
#include "common/Transform.h"
#include "DinoMachine.h"

namespace Dynagraph {

// basic stuff to handle an engine/layout pair with named nodes & edges.
// applications (e.g. the dynagraph executable & dynagraph COM component)
// derive from this and override the Incr...() methods to share a basic
// understanding of how to deal with names, how to create/replace the layout server, etc.

// all these exceptions are recoverable: report error to client and ignore
struct DVException : DGException2 {
	DVException(const char *xep,const char *parm) : DGException2(xep,parm,false) {}
};
struct DVReopenXep : DVException {
    DVReopenXep(const char *name) : DVException("Tried to reopen graph",name) {}
};
struct DVEdgeReopen : DVException {
    DVEdgeReopen(const char *name) : DVException("The edge name has already been used",name) {}
};
struct DVNodeReopen : DVException {
    DVNodeReopen(const char *name) : DVException("The node name has already been used",name) {}
};
struct DVEdgeTailDoesNotExist : DVException {
	DVEdgeTailDoesNotExist(const char *name) : DVException("Tried to open edge but tail does not exist",name) {}
};
struct DVEdgeHeadDoesNotExist : DVException {
	DVEdgeHeadDoesNotExist(const char *name) : DVException("Tried to open edge but head does not exist",name) {}
};
struct DVEdgeNameMismatch : DVException {
    DVEdgeNameMismatch(const char *name) : DVException("Tried to re-open edge under new name",name) {}
};
struct DVEdgeDoesNotExist : DVException {
    DVEdgeDoesNotExist(const char *name) : DVException("Tried to modify or delete unknown edge",name) {}
};
struct DVNodeDoesNotExist : DVException {
    DVNodeDoesNotExist(const char *name) : DVException("Tried to modify or delete unknown node",name) {}
};

template<typename Layout>
struct DynaView : IncrLangEvents {
	typedef std::map<DString,typename Layout::Node*> nodeDict;
	typedef std::map<DString,typename Layout::Edge*> edgeDict;
	nodeDict nodes;
	edgeDict edges;
	Layout layout, // everything the client has created
        current, // what's currently being controlled by engine
        old; // when switching engines, the stuff that was already here
	ChangeQueue<Layout> Q;
	Server<Layout> *dgserver;
	int locks;
    Transform *m_transform;
    bool m_useDotDefaults;
	bool m_replacementFlag, // whether we're in the middle of engine replacement
        m_allowOneReopen;


	// implement these to respond to incrface events
	virtual void IncrHappened() = 0;
	virtual void IncrNewNode(typename Layout::Node *n) = 0;
	virtual void IncrNewEdge(typename Layout::Edge *e) = 0;

	DynaView(Name name = Name(),Transform *transform=0, bool useDotDefaults=false);
	virtual ~DynaView();
    // create or replace engine based on "engines" attribute of layout
    void createServer();
    // complete an engine replacement by changing the insertions into new engine into modifies
    // (call after dgserver->Process but before dealing with Q
    void completeReplacement();
	std::pair<typename Layout::Node*,bool> getNode(DString name,bool create=false) { // vc++ gave me erroneous errors when i outalined
		if(name.empty())
			name = randomName('n');
		typename Layout::Node *n = nodes[name];
		if(n)
			return std::make_pair(n,false);
		if(!create)
			return std::make_pair<typename Layout::Node*>(0,false);
		n = layout.create_node();
		gd<Name>(n) = name;
		nodes[name] = n;
		return std::make_pair(n,true);
	}
	std::pair<typename Layout::Edge*,bool> getEdge(DString id,typename Layout::Node *t,typename Layout::Node *h,bool create);
	std::pair<typename Layout::Edge*,bool> getEdge(DString id,DString tail,DString head,bool create);
	typename Layout::Edge *getEdge(DString id,DString tail,DString head);
	typename Layout::Edge *getEdge(DString id);
	void rename(DString newName);
	void rename(typename Layout::Node *n,DString newName);
	void rename(typename Layout::Edge *e,DString newName);
	void forget(typename Layout::Node *n);
	void forget(typename Layout::Edge *e);
	// important to destroy edges because graph representation won't allow
	// you to draw two edges t->h
	void destroy(typename Layout::Node *n);
	void destroy(typename Layout::Edge *e);

    bool maybe_go();
    Update open_layout(const StrAttrs &attrs);
	
    // IncrLangEvents
	DString dinotype() { return "layout"; }
	bool incr_ev_open_graph(DString graph,const StrAttrs &attrs);
	bool incr_ev_close_graph();
	bool incr_ev_mod_graph(const StrAttrs &attrs);
	bool incr_ev_lock();
	bool incr_ev_unlock();
	DString incr_ev_ins_node(DString name, const StrAttrs &attrs, bool merge);
	DString incr_ev_ins_edge(DString name, DString tailname, DString headname, const StrAttrs &attrs);
	bool incr_ev_mod_node(DString name,const StrAttrs &attrs);
	bool incr_ev_mod_edge(DString name,const StrAttrs &attrs);
	bool incr_ev_del_node(DString name);
	bool incr_ev_del_edge(DString name);
	bool incr_ev_req_graph();
	bool incr_ev_req_node(DString name);
	bool incr_ev_req_edge(DString name);
    void incr_ev_load_strgraph(StrGraph *sg,bool merge, bool del);
};

template<typename Layout>
DynaView<Layout>::DynaView(Name name,Transform *transform, bool useDotDefaults) :
	current(&layout),
    old(&layout),
	Q(&layout,&current),
	dgserver(0),
	locks(0),
    m_transform(transform),
    m_useDotDefaults(useDotDefaults),
    m_replacementFlag(false),
    m_allowOneReopen(false)
{
	if(name.empty())
		name = randomName('v');
	gd<Name>(&layout) = name;
}
template<typename Layout>
DynaView<Layout>::~DynaView() {
	if(dgserver)
		delete dgserver;
}
template<typename Layout>
void DynaView<Layout>::createServer() {
	// the underlying structure is the same here: just swap engine
	if(dgserver) {
		delete dgserver;
		dgserver = 0;
	}
	dgserver = createLayoutServer<Layout>(&layout,&current);
}
template<typename Layout>
void DynaView<Layout>::completeReplacement() {
	//post_engine_replacement(Q,old);
}
/*
template<typename Layout>
std::pair<typename Layout::Node*,bool> DynaView<Layout>::getNode(DString name,bool create)
*/
template<typename Layout>
std::pair<typename Layout::Edge*,bool> DynaView<Layout>::getEdge(DString name,typename Layout::Node *t,typename Layout::Node *h,bool create) {
	typename Layout::Edge *e=0;
	if(name.empty())
		name = randomName('e');
	else if((e = edges[name])) {
		if(t && t!=e->tail || h && h!=e->head)
			throw DVEdgeReopen(name.c_str());
		return std::make_pair(e,false);
	}
	if(!(t && h))
		return std::make_pair<typename Layout::Edge*>(0,false);
	e = layout.find_edge(t,h);
	if(e)
		return std::make_pair(e,false);
	if(!create)
		return std::make_pair<typename Layout::Edge*>(0,false);
	e = layout.create_edge(t,h).first;
	gd<Name>(e) = name;
	edges[name] = e;
	return std::make_pair(e,true);
}
template<typename Layout>
std::pair<typename Layout::Edge*,bool> DynaView<Layout>::getEdge(DString name,DString tail,DString head,bool create) {
	assert(tail.size()&&head.size());
	typename Layout::Node *t = getNode(tail,false).first,
		*h = getNode(head,false).first;
	if(!t)
		throw DVEdgeTailDoesNotExist(tail.c_str());
	if(!h)
		throw DVEdgeHeadDoesNotExist(head.c_str());
	return getEdge(name,t,h,create);
}
template<typename Layout>
typename Layout::Edge *DynaView<Layout>::getEdge(DString name,DString tail,DString head) {
	return getEdge(name,tail,head,false).first;
}
template<typename Layout>
typename Layout::Edge *DynaView<Layout>::getEdge(DString name) {
	return getEdge(name,(typename Layout::Node*)0,(typename Layout::Node*)0,false).first;
}
template<typename Layout>
void DynaView<Layout>::rename(DString newName) {
    DinoMachine::Node *n = g_dinoMachine.ndict[gd<Name>(&layout)];
    assert(n);
    g_dinoMachine.rename(n,newName);
    gd<Name>(&layout) = newName;
}
template<typename Layout>
void DynaView<Layout>::rename(typename Layout::Node *n,DString newName) {
	forget(n);
	nodes[gd<Name>(n) = newName] = n;
}
template<typename Layout>
void DynaView<Layout>::rename(typename Layout::Edge *e,DString newName) {
	forget(e);
	edges[gd<Name>(e) = newName] = e;
}
template<typename Layout>
void DynaView<Layout>::forget(typename Layout::Node *n) {
	nodes.erase(gd<Name>(n));
}
template<typename Layout>
void DynaView<Layout>::forget(typename Layout::Edge *e) {
	edges.erase(gd<Name>(e));
}
template<typename Layout>
void DynaView<Layout>::destroy(typename Layout::Node *n) {
	forget(n);
	layout.erase(n);
}
template<typename Layout>
void DynaView<Layout>::destroy(typename Layout::Edge *e) {
	forget(e);
	layout.erase(e);
}

// incr_ev
template<typename Layout>
bool DynaView<Layout>::maybe_go() {
    if(locks>0)
        return false;
	bool ch = false;
    while(!Q.Empty()) {
        dgserver->Process(Q);
        if(m_replacementFlag) {
            completeReplacement();
            m_replacementFlag = false;
        }
        stringsOut(m_transform,Q);
        for(typename Layout::graphedge_iter ei = Q.delE.edges().begin(); ei!=Q.delE.edges().end(); ++ei)
            forget(*ei);
        for(typename Layout::node_iter ni = Q.delN.nodes().begin(); ni!=Q.delN.nodes().end(); ++ni)
            forget(*ni);
        IncrHappened(); // must clear Q but the events might cause more changes
		ch = true;
    }
	if(ch)
		g_dinoMachine.changed(gd<Name>(&layout));
    return true;
}
template<typename Layout>
Update DynaView<Layout>::open_layout(const StrAttrs &attrs) {
    bool create = false;
    if(!dgserver)
        create = true;
    else {
        StrAttrs::const_iterator ai = attrs.find("engines");
        if(ai!=attrs.end())
            if(ai->second!=gd<StrAttrs>(&current)["engines"])
                create = true;
    }
    Update ret = stringsIn(m_transform,m_useDotDefaults,&layout,attrs,false).flags;
    if(create)
        createServer();
    return ret;
}
template<typename Layout>
bool DynaView<Layout>::incr_ev_open_graph(DString graph,const StrAttrs &attrs) {
	bool isModify = false;
    if(dgserver) {
        if(!m_allowOneReopen)
            throw DVReopenXep(graph.c_str());
        m_allowOneReopen = false;
		isModify = true;
    }

    open_layout(attrs);
    if(!dgserver)
        return false;
    incr_set_handler(gd<Name>(&layout) = graph,this);
	std::cout << (isModify?"modify":"open") << " graph " << graph << " " << gd<StrAttrs2>(&layout) << std::endl;
	gd<StrAttrChanges>(&layout).clear();
    return true;
}
template<typename Layout>
bool DynaView<Layout>::incr_ev_close_graph() {
    std::cout << "close graph " << gd<Name>(&layout) << std::endl;
    return true;
}
template<typename Layout>
bool DynaView<Layout>::incr_ev_mod_graph(const StrAttrs &attrs) {
    Q.GraphUpdateFlags() |= open_layout(attrs).flags;
    maybe_go();
    return true;
}
template<typename Layout>
bool DynaView<Layout>::incr_ev_lock() {
    locks++;
    return true;
}
template<typename Layout>
bool DynaView<Layout>::incr_ev_unlock() {
    --locks;
    maybe_go();
    return true;
}
template<typename Layout>
DString DynaView<Layout>::incr_ev_ins_node(DString name, const StrAttrs &attrs, bool merge) {
    DString rename = name;
    std::pair<typename Layout::Node *,bool> nb = getNode(name,true);
    if(!nb.second&&!merge) {
        // name already was used, so make an anónimo and label with the name
        nb.second = true;
        nb.first = getNode(0,true).first;
        gd<StrAttrs>(nb.first)["label"] = name;
        rename = gd<Name>(nb.first);
    }
    Update upd = stringsIn<Layout>(m_transform,nb.first,attrs,true);
    if(nb.second) {
        Q.InsNode(nb.first);
        IncrNewNode(nb.first);
    }
    else
        Q.ModNode(nb.first,upd);
    maybe_go();
    return rename;
        /*
        pair<typename Layout::Node*,bool> nb = getNode(name,true);
        Update upd = stringsIn(m_transform,nb.first,attrs,true);
        if(!current.find(nb.first)) {
		    if(nb.second)
			    IncrNewNode(nb.first);
            Q.InsNode(nb.first);
        }
        else // treat re-insert as modify
            Q.ModNode(nb.first,upd);
        maybe_go();
        */
}
template<typename Layout>
DString DynaView<Layout>::incr_ev_ins_edge(DString name, DString tailname, DString headname, const StrAttrs &attrs) {
    std::pair<typename Layout::Edge*,bool> eb = getEdge(name,tailname,headname,true);
	if(!eb.second&&gd<Name>(eb.first)!=name)
		throw DVEdgeNameMismatch(name.c_str());
    Update upd = stringsIn<Layout>(m_transform,eb.first,attrs,true);
    if(eb.second) {
        Q.InsEdge(eb.first);
        IncrNewEdge(eb.first);
    }
    else
        Q.ModEdge(eb.first,upd);
    maybe_go();
    return gd<Name>(eb.first);
}
template<typename Layout>
bool DynaView<Layout>::incr_ev_mod_node(DString name,const StrAttrs &attrs) {
    typename Layout::Node *n = getNode(name).first;
    if(!n)
        throw DVNodeDoesNotExist(name.c_str());
    Q.ModNode(n,stringsIn<Layout>(m_transform,n,attrs,false));
    maybe_go();
    return true;
}
template<typename Layout>
bool DynaView<Layout>::incr_ev_mod_edge(DString name,const StrAttrs &attrs) {
    typename Layout::Edge *e = getEdge(name);
    if(!e)
        throw DVEdgeDoesNotExist(name.c_str());
    Q.ModEdge(e,stringsIn<Layout>(m_transform,e,attrs,false));
    maybe_go();
    return true;
}
template<typename Layout>
bool DynaView<Layout>::incr_ev_del_node(DString name) {
	typename Layout::Node *n = getNode(name).first;
    if(!n)
        throw DVNodeDoesNotExist(name.c_str());
    Q.DelNode(n);
    maybe_go();
    return true;
}
template<typename Layout>
bool DynaView<Layout>::incr_ev_del_edge(DString name) {
    typename Layout::Edge *e = getEdge(name);
    if(!e)
        throw DVEdgeDoesNotExist(name.c_str());
    Q.DelEdge(e);
    maybe_go();
    return true;
}
template<typename Layout>
bool DynaView<Layout>::incr_ev_req_graph() {
	std::cout << "fulfil graph " << gd<Name>(&layout) << std::endl;
	emitGraph(std::cout,&layout);
    return true;
}
template<typename Layout>
bool DynaView<Layout>::incr_ev_req_node(DString name) {
    typename Layout::Node *n = getNode(name).first;
    if(!n)
        throw DVNodeDoesNotExist(name.c_str());
	std::cout << "fulfil node " << gd<Name>(&layout) << " " << name << " " << gd<StrAttrs>(n);
    return true;
}
template<typename Layout>
bool DynaView<Layout>::incr_ev_req_edge(DString name) {
    typename Layout::Edge *e = getEdge(name);
    if(!e)
        throw DVEdgeDoesNotExist(name.c_str());
    std::cout << "fulfil edge " << gd<Name>(&layout) << " " << name << " " << gd<StrAttrs>(e);
    return true;
}
template<typename Layout>
void DynaView<Layout>::incr_ev_load_strgraph(StrGraph *sg,bool merge, bool del) {
    typedef std::map<DString,StrGraph::Node*> strnode_dict;
    incr_ev_lock();
    if(merge && del) { // find deletions
        strnode_dict nd;
        for(StrGraph::node_iter sni = sg->nodes().begin(); sni!=sg->nodes().end(); ++sni)
            nd[gd<Name>(*sni)] = *sni;
        for(typename Layout::node_iter ni = current.nodes().begin(); ni!=current.nodes().end(); ++ni)
            if(nd.find(gd<Name>(*ni))==nd.end()) {
                for(typename Layout::nodeedge_iter ei = (*ni)->alledges().begin(); ei!=(*ni)->alledges().end(); ++ei)
                    incr_ev_del_edge(gd<Name>(*ei));
                incr_ev_del_node(gd<Name>(*ni));
            }
        for(typename Layout::graphedge_iter ei = current.edges().begin(); ei!=current.edges().end(); ++ei) {
            // find edges by head and tail because prob has no name.
            StrGraph::Node *t=0,*h=0;
            strnode_dict::iterator di = nd.find(gd<Name>((*ei)->tail));
            if(di!=nd.end()) {
                t = di->second;
                di = nd.find(gd<Name>((*ei)->head));
                if(di!=nd.end()) {
                    h = di->second;
                    if(!sg->find_edge(t,h))
                        incr_ev_del_edge(gd<Name>(*ei));
                }
            }
            // if tail or head missing we've already called DelNode thus don't need to call DelEdge
        }
    }
    { // find insertions & modifications
        std::map<DString,DString> nd; // an override dictionary, only if !merge
        for(StrGraph::node_iter ni = sg->nodes().begin(); ni!=sg->nodes().end(); ++ni) {
            DString rename = incr_ev_ins_node(gd<Name>(*ni),gd<StrAttrs>(*ni),merge);
            if(!merge)
                nd[gd<Name>(*ni)] = rename;
        }
        for(StrGraph::graphedge_iter ei = sg->edges().begin(); ei!=sg->edges().end(); ++ei) {
            DString id = gd<Name>(*ei),
                t = gd<Name>((*ei)->tail),
                h = gd<Name>((*ei)->head);
            if(poorEdgeName(id.c_str()))
                id = 0;
            if(!merge) {
                t = nd[t];
                h = nd[h];
            }
			if(typename Layout::Edge *oldE = getEdge(id,t,h))
				incr_ev_mod_edge(gd<Name>(oldE),gd<StrAttrs>(*ei));
			else
				incr_ev_ins_edge(id,t,h,gd<StrAttrs>(*ei));
        }
    }
    incr_ev_unlock();
}

} // namespace Dynagraph
