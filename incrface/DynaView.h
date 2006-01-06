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


#include "createLayoutServer.h"
#include "common/randomName.h"
#include "common/Transform.h"
#include "DinoMachine.h"
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

struct DynaView : IncrLangEvents {
	typedef std::map<DString,Layout::Node*> nodeDict;
	typedef std::map<DString,Layout::Edge*> edgeDict;
	nodeDict nodes;
	edgeDict edges;
	Layout layout, // everything the client has created
        current, // what's currently being controlled by engine
        old; // when switching engines, the stuff that was already here
	ChangeQueue Q;
	Server *dgserver;
	int locks;
    Transform *m_transform;
    bool m_useDotDefaults;
	bool m_replacementFlag, // whether we're in the middle of engine replacement
        m_allowOneReopen;


	// implement these to respond to incrface events
	virtual void IncrHappened() = 0;
	virtual void IncrNewNode(Layout::Node *n) = 0;
	virtual void IncrNewEdge(Layout::Edge *e) = 0;

	DynaView(Name name = Name(),Transform *transform=0, bool useDotDefaults=false);
	virtual ~DynaView();
    // create or replace engine based on "engines" attribute of layout
    void createServer();
    // complete an engine replacement by changing the insertions into new engine into modifies
    // (call after dgserver->Process but before dealing with Q
    void completeReplacement();
	std::pair<Layout::Node*,bool> getNode(DString id,bool create=false);
	std::pair<Layout::Edge*,bool> getEdge(DString id,Layout::Node *t,Layout::Node *h,bool create);
	std::pair<Layout::Edge*,bool> getEdge(DString id,DString tail,DString head,bool create);
	Layout::Edge *getEdge(DString id,DString tail,DString head);
	Layout::Edge *getEdge(DString id);
	void rename(DString newName);
	void rename(Layout::Node *n,DString newName);
	void rename(Layout::Edge *e,DString newName);
	void forget(Layout::Node *n);
	void forget(Layout::Edge *e);
	// important to destroy edges because graph representation won't allow
	// you to draw two edges t->h
	void destroy(Layout::Node *n);
	void destroy(Layout::Edge *e);

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
