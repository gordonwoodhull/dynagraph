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

#ifndef DynagraphThread_h
#define DynagraphThread_h

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

namespace Dynagraph {

template<typename Graph>
struct DynagraphThread {
	ChangingGraph<Graph> &world_;
	ChangeProcessor<Graph> *engine_;
	boost::thread *thread_;

	DynagraphThread(ChangingGraph<Graph> &world,ChangeProcessor<Graph> *engine) :
	  world_(world),engine_(engine) {
		thread_ = new boost::thread(boost::bind(&DynagraphThread::go,this));
	}
	~DynagraphThread() {
		delete thread_;
	}
	void go() {
		engine_->Process();
	}
	void interrupt() {
		assert(boost::thread()!=*thread_);
		gd<Interruptable>(&world_.whole_).interrupt = true;
		thread_->join();
		gd<Interruptable>(&world_.whole_).interrupt = false;
	}
};

} // namespace Dynagraph

#endif // DynagraphThread_h
