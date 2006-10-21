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

#ifndef IncrStrGraphHandler_h
#define IncrStrGraphHandler_h

#include "common/ChangeQueue.h"
#include "common/ChangeProcessor.h"
#include "common/diff_strgraph.h"
#include "common/randomName.h"
#include "common/ChangingGraph.h"
#include "IncrLangEvents.h"
#include "IncrViewWatcher.h"

#ifndef DYNAGRAPH_NO_THREADS
#define STRHANDLER_DO_THREADS
#endif
#ifdef STRHANDLER_DO_THREADS
#include "common/DynagraphThread.h"
#endif

#include <memory>

namespace Dynagraph {

// NGraph is LGraph containing at least StrAttrs2
template<typename NGraph>
struct IncrStrGraphHandler : IncrLangEvents {
	std::auto_ptr<ChangingGraph<NGraph> > world_;
	IncrViewWatcher<NGraph> *watcher_;
	ChangeProcessor<NGraph> *next_;
#ifdef STRHANDLER_DO_THREADS
	DynagraphThread<NGraph> *layoutThread_;
#endif
    int locks_;

    IncrStrGraphHandler(ChangingGraph<NGraph> *world) : world_(world),watcher_(0),next_(0),
#ifdef STRHANDLER_DO_THREADS
		layoutThread_(0),
#endif
		locks_(0) {}
	~IncrStrGraphHandler() {
		// don't delete watcher because it's probably in engine chain
		if(next_)
			delete next_;
	}

	bool maybe_go(typename ChangeProcessor<NGraph>::Function purpose = &ChangeProcessor<NGraph>::Process);
	void wait_thread();

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
	void incr_ev_shutdown();
	void interrupt_thread();
	void incr_ev_open_graph(DString graph,const StrAttrs &attrs);
	void incr_ev_close_graph();
	void incr_ev_mod_graph(const StrAttrs &attrs);
	void incr_ev_pulse(const StrAttrs &attrs);
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
template<typename NGraph>
bool IncrStrGraphHandler<NGraph>::maybe_go(typename ChangeProcessor<NGraph>::Function purpose) {
	if(locks_>0)
		return false;
	if(next_) {
#ifdef STRHANDLER_DO_THREADS
		dgassert(!layoutThread_); // must either interrupt or wait_thread between runs
		layoutThread_ = new DynagraphThread<NGraph>(*world_,next_,purpose);
#else
		(next_->*purpose)();
#endif
	}
	return true;
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::wait_thread() {
#ifdef STRHANDLER_DO_THREADS
	if(layoutThread_) {
		layoutThread_->wait();
		delete layoutThread_;
		layoutThread_ = 0;
	}
#endif
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::interrupt_thread() {
#ifdef STRHANDLER_DO_THREADS
	if(layoutThread_) {
		layoutThread_->interrupt();
		delete layoutThread_;
		layoutThread_ = 0;
	}
#endif
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_shutdown() {
	wait_thread();
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_open_graph(DString graph,const StrAttrs &attrs) {
	interrupt_thread(); // there shouldn't be a thread but hey
	gd<Name>(&world_->whole_) = graph;
	SetNoMark(world_->Q_.ModGraph(),attrs);
	if(reports.enabled(dgr::input_cooked)) {
		LOCK_REPORT(dgr::input_cooked);
		reports[dgr::input_cooked] << "open graph " << gd<Name>(&world_->current_);
		if(!attrs.empty())
			reports[dgr::input_cooked] << ' ' << attrs;
		reports[dgr::input_cooked] << std::endl;
	}
	maybe_go(&ChangeProcessor<NGraph>::Open);
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_close_graph() {
	interrupt_thread();
	if(reports.enabled(dgr::input_cooked)) {
		LOCK_REPORT(dgr::input_cooked);
		reports[dgr::input_cooked] << "close graph " << gd<Name>(&world_->current_) << std::endl;
	}
	maybe_go(&ChangeProcessor<NGraph>::Close);
	wait_thread(); // don't allow parent to delete me until i'm finished! (why thread this cmd then? foolish consistency?)
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_mod_graph(const StrAttrs &attrs) {
	interrupt_thread();
	SetAndMark(world_->Q_.ModGraph(),attrs);
	if(reports.enabled(dgr::input_cooked)) {
		LOCK_REPORT(dgr::input_cooked);
		if(!attrs.empty())
			reports[dgr::input_cooked] << "modify graph " << gd<Name>(&world_->current_) << ' ' << attrs << std::endl;
	}
    maybe_go();
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_pulse(const StrAttrs &attrs) {
	// WEIRD: received pulse becomes an interrupt which then should generate almost the same pulse as output!
	gd<Interruptible>(&world_->whole_).attrs = attrs; // qualify/restrict the kind of interrupt
    interrupt_thread();
	gd<Interruptible>(&world_->whole_).attrs.clear(); // unqualify 
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_lock() {
    interrupt_thread();
	if(reports.enabled(dgr::input_cooked)) {
		LOCK_REPORT(dgr::input_cooked);
		reports[dgr::input_cooked] << "lock graph " << gd<Name>(&world_->current_) << std::endl;
	}
    locks_++;
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_unlock() {
    interrupt_thread();
    --locks_;
	if(reports.enabled(dgr::input_cooked)) {
		LOCK_REPORT(dgr::input_cooked);
		reports[dgr::input_cooked] << "unlock graph " << gd<Name>(&world_->current_) << std::endl;
	}
    maybe_go();
}
template<typename NGraph>
DString IncrStrGraphHandler<NGraph>::incr_ev_ins_node(DString name, const StrAttrs &attrs, bool merge) {
    interrupt_thread();
    if(name.empty())
        name = randomName('n');
    typename NGraph::Node *n = fetch_node(name,true);
	typename ChangeQueue<NGraph>::NodeResult result = world_->Q_.InsNode(n);
	SetNoMark(result.object,attrs);
	if(reports.enabled(dgr::input_cooked)) {
		LOCK_REPORT(dgr::input_cooked);
		reports[dgr::input_cooked] << "insert node " << gd<Name>(&world_->current_) << ' ' << gd<Name>(result.object);
		if(!attrs.empty())
			reports[dgr::input_cooked] << ' ' << attrs;
		reports[dgr::input_cooked] << std::endl;
	}
    maybe_go();
    return name;
}
template<typename NGraph>
DString IncrStrGraphHandler<NGraph>::incr_ev_ins_edge(DString name, DString tailname, DString headname, const StrAttrs &attrs) {
    interrupt_thread();
    if(name.empty())
        name = randomName('e');
    typename NGraph::Edge *e = fetch_edge(tailname,headname,name,true);
	typename ChangeQueue<NGraph>::EdgeResult result = world_->Q_.InsEdge(e);
	SetNoMark(result.object,attrs);
	if(reports.enabled(dgr::input_cooked)) {
		LOCK_REPORT(dgr::input_cooked);
		reports[dgr::input_cooked] << "insert edge " << gd<Name>(&world_->current_) << ' ' << gd<Name>(result.object) << ' ' << tailname << ' ' << headname;
		if(!attrs.empty())
			reports[dgr::input_cooked] << ' ' << attrs;
		reports[dgr::input_cooked] << std::endl;
	}
    maybe_go();
    return name;
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_mod_node(DString name,const StrAttrs &attrs) {
    interrupt_thread();
    typename NGraph::Node *n = fetch_node(name,false);
	typename ChangeQueue<NGraph>::NodeResult result = world_->Q_.ModNode(n);
	SetAndMark(result.object,attrs);
	if(reports.enabled(dgr::input_cooked) && !attrs.empty()) {
		LOCK_REPORT(dgr::input_cooked);
		reports[dgr::input_cooked] << "modify node " << gd<Name>(&world_->current_) << ' ' << gd<Name>(result.object) << ' ' << attrs << std::endl;
	}
    maybe_go();
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_mod_edge(DString name,const StrAttrs &attrs) {
    interrupt_thread();
    typename NGraph::Edge *e = fetch_edge(name);
	typename ChangeQueue<NGraph>::EdgeResult result = world_->Q_.ModEdge(e);
	SetAndMark(result.object,attrs);
	if(reports.enabled(dgr::input_cooked) && !attrs.empty()) {
		LOCK_REPORT(dgr::input_cooked);
		reports[dgr::input_cooked] << "modify edge " << gd<Name>(&world_->current_) << ' ' << gd<Name>(result.object) << ' ' << attrs << std::endl;
	}
    maybe_go();
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_del_node(DString name) {
    interrupt_thread();
    typename NGraph::Node *n = fetch_node(name,false);
	world_->Q_.DelNode(n);
	if(reports.enabled(dgr::input_cooked)) {
		LOCK_REPORT(dgr::input_cooked);
		reports[dgr::input_cooked] << "delete node " << gd<Name>(&world_->current_) << ' ' << gd<Name>(n)  << std::endl;
	}
    maybe_go();
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_del_edge(DString name) {
    interrupt_thread();
    typename NGraph::Edge *e = fetch_edge(name);
	world_->Q_.DelEdge(e);
	if(reports.enabled(dgr::input_cooked)) {
		LOCK_REPORT(dgr::input_cooked);
		reports[dgr::input_cooked] << "delete edge " << gd<Name>(&world_->current_) << ' ' << gd<Name>(e)  << std::endl;
	}
    maybe_go();
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_req_graph() {
    interrupt_thread();
	if(watcher_)
		watcher_->FulfilGraph(&world_->whole_);
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_req_node(DString name) {
    interrupt_thread();
    typename NGraph::Node *n = fetch_node(name,false);
	if(watcher_)
		watcher_->FulfilNode(n);
}
template<typename NGraph>
void IncrStrGraphHandler<NGraph>::incr_ev_req_edge(DString name) {
    interrupt_thread();
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
    interrupt_thread();
    dgassert(merge && del);
    we_do_what_we_are_told<NGraph> react(this);
    incr_ev_lock();
    diff_strgraph(&world_->current_,sg,react);
    incr_ev_unlock();
}

} // namespace Dynagraph

#endif //IncrStrGraphHandler_h
