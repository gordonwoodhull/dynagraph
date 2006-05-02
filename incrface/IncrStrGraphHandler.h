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


#include "common/ChangeQueue.h"
#include "common/ChangeProcessor.h"
#include "common/diff_strgraph.h"
#include "common/randomName.h"
#include "common/DynagraphWorld.h"
#include "IncrLangEvents.h"
#include "IncrViewWatcher.h"

#ifndef STRGRAPHHANDLER_NO_THREADS
#include "common/DynagraphThread.h"
#endif

#include <memory>

namespace Dynagraph {

// NGraph is LGraph containing at least StrAttrs2
template<typename NGraph> 
struct IncrStrGraphHandler : IncrLangEvents {
	std::auto_ptr<DynagraphWorld<NGraph> > world_;
	ChangeQueue<NGraph> Q_;
	IncrViewWatcher<NGraph> *watcher_;
	ChangeProcessor<NGraph> *next_;
	DynagraphThread<NGraph> *layoutThread_;
    int locks_;

    IncrStrGraphHandler(DynagraphWorld<NGraph> *world) : world_(world),Q_(&world_->whole_,&world_->current_),watcher_(0),next_(0),layoutThread_(0),locks_(0) {}
	~IncrStrGraphHandler() {
		// don't delete watcher because it's probably in engine chain
		if(next_)
			delete next_;
	}

	void interrupt_layout() {
#ifndef STRGRAPHHANDLER_NO_THREADS
		if(layoutThread_) {
			layoutThread_->interrupt();
			delete layoutThread_;
			layoutThread_ = 0;
		}
#endif
	}
    bool maybe_go() {
		if(locks_>0)
			return false;
		if(next_) {
#ifndef STRGRAPHHANDLER_NO_THREADS
			layoutThread_ = new DynagraphThread<NGraph>(*world_,next_,Q_);
#else
			next_->Process(Q_);
#endif
		}
		return true;
	}

	typename NGraph::Node *fetch_node(DString name,bool create) {
		typename NGraph::Node *n = world_->whole_.fetch_node(name,create).first;
		if(!n)
			throw DGNodeDoesNotExist(name);
		return n;
	}
	typename NGraph::Edge *fetch_edge(DString tailname,DString headname,DString edgename,bool create) {
		typename NGraph::Edge *e = world_->whole_.fetch_edge(tailname,headname,edgename,create).first;
		if(!e)
			throw DGEdgeDoesNotExist(edgename);
		return e;
	}
	typename NGraph::Edge *fetch_edge(DString edgename) {
		typename NGraph::Edge *e = world_->whole_.fetch_edge(edgename);
		if(!e)
			throw DGEdgeDoesNotExist(edgename);
		return e;
	}

    // IncrLangEvents
	DString dinotype() { return "stringraph"; }
	void incr_ev_open_graph(DString graph,const StrAttrs &attrs);
	void incr_ev_close_graph();
	void incr_ev_mod_graph(const StrAttrs &attrs);
	void incr_ev_lock();
	void incr_ev_unlock();
	DString incr_ev_ins_node(DString name, const StrAttrs &attrs, bool merge);
	DString incr_ev_ins_edge(DString name, DString tailname, DString headname, const StrAttrs &attrs);
	void incr_ev_mod_node(DString name,const StrAttrs &attrs);
	void incr_ev_mod_edge(DString name,const StrAttrs &attrs);
	void incr_ev_del_node(DString name);
	void incr_ev_del_edge(DString name);
	void incr_ev_req_graph();
	void incr_ev_req_node(DString name);
	void incr_ev_req_edge(DString name);
    void incr_ev_load_strgraph(StrGraph *sg,bool merge, bool del);
};

// make changes immediately but only Process them when unlocked
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_open_graph(DString graph,const StrAttrs &attrs) {
	interrupt_layout();
    gd<Name>(&world_->whole_) = graph;
    SetAndMark(Q_.ModGraph(),attrs);
    if(watcher_)
		watcher_->IncrOpen(Q_);
	Q_.Execute(true);
	maybe_go();
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_close_graph() {
	interrupt_layout();
    if(watcher_)
		watcher_->IncrClose(Q_);
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_mod_graph(const StrAttrs &attrs) {
	interrupt_layout();
	SetAndMark(Q_.ModGraph(),attrs);
    maybe_go();
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_lock() {
	interrupt_layout();
    locks_++;
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_unlock() {
	interrupt_layout();
    --locks_;
    maybe_go();
}
template<typename NGraph>
DString IncrStrGraphHandler<NGraph>::incr_ev_ins_node(DString name, const StrAttrs &attrs, bool merge) {
	interrupt_layout();
    if(name.empty())
        name = randomName('n');
    typename NGraph::Node *n = fetch_node(name,true);
	typename ChangeQueue<NGraph>::NodeResult result = Q_.InsNode(n);
	SetAndMark(result.object,attrs);
    maybe_go();
    return name;
}
template<typename NGraph>
DString IncrStrGraphHandler<NGraph>::incr_ev_ins_edge(DString name, DString tailname, DString headname, const StrAttrs &attrs) {
	interrupt_layout();
    if(name.empty())
        name = randomName('e');
    typename NGraph::Edge *e = fetch_edge(tailname,headname,name,true);
	typename ChangeQueue<NGraph>::EdgeResult result = Q_.InsEdge(e);
	SetAndMark(result.object,attrs);
    maybe_go();
    return name;
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_mod_node(DString name,const StrAttrs &attrs) {
	interrupt_layout();
    typename NGraph::Node *n = fetch_node(name,false);
	typename ChangeQueue<NGraph>::NodeResult result = Q_.ModNode(n);
	SetAndMark(result.object,attrs);
    maybe_go();
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_mod_edge(DString name,const StrAttrs &attrs) {
	interrupt_layout();
    typename NGraph::Edge *e = fetch_edge(name);
	typename ChangeQueue<NGraph>::EdgeResult result = Q_.ModEdge(e);
	SetAndMark(result.object,attrs);
    maybe_go();
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_del_node(DString name) {
	interrupt_layout();
    typename NGraph::Node *n = fetch_node(name,false);
	Q_.DelNode(n);
    maybe_go();
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_del_edge(DString name) {
	interrupt_layout();
    typename NGraph::Edge *e = fetch_edge(name);
	Q_.DelEdge(e);
    maybe_go();
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_req_graph() {
	interrupt_layout();
	if(watcher_)
		watcher_->FulfilGraph(&world_->whole_);
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_req_node(DString name) {
	interrupt_layout();
    typename NGraph::Node *n = fetch_node(name,false);
	if(watcher_)
		watcher_->FulfilNode(n);
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_req_edge(DString name) {
	interrupt_layout();
    typename NGraph::Edge *e = fetch_edge(name);
	if(watcher_)
		watcher_->FulfilEdge(e);
}
template<typename NGraph>
struct we_do_what_we_are_told {
    IncrStrGraphHandler<NGraph> *hand;
    we_do_what_we_are_told(IncrStrGraphHandler<NGraph> *hand) : hand(hand) {}
    void ins(StrGraph::Node *n) {
        hand->incr_ev_ins_node(gd<Name>(n),gd<StrAttrs>(n),true);
    }
    void ins(StrGraph::Edge *e) {
        hand->incr_ev_ins_edge(gd<Name>(e),gd<Name>(e->tail),gd<Name>(e->head),gd<StrAttrs>(e));
    }
    void mod(typename NGraph::Node *nn,StrGraph::Node *sn,StrAttrs *attrs) {
        hand->incr_ev_mod_node(gd<Name>(nn),*attrs);
    }
    void mod(typename NGraph::Edge *ne,StrGraph::Edge *se,StrAttrs *attrs) {
        hand->incr_ev_mod_edge(gd<Name>(ne),*attrs);
    }
    void del(typename NGraph::Node *n) {
        hand->incr_ev_del_node(gd<Name>(n));
    }
    void del(typename NGraph::Edge *e) {
        hand->incr_ev_del_edge(gd<Name>(e));
    }
};
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_load_strgraph(StrGraph *sg,bool merge, bool del) {
    assert(merge && del);
    we_do_what_we_are_told<NGraph> react(this);
    incr_ev_lock();
    diff_strgraph(&world_->current_,sg,react);
    incr_ev_unlock();
}

} // namespace Dynagraph
