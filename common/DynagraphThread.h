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

extern bool g_xeptFatal;

namespace Dynagraph {

template<typename Graph>
struct DynagraphThread {
	ChangingGraph<Graph> &world_;
	ChangeProcessor<Graph> *engine_;
	boost::thread *thread_;
	typename ChangeProcessor<Graph>::Function purpose_;

	DynagraphThread(ChangingGraph<Graph> &world,ChangeProcessor<Graph> *engine,typename ChangeProcessor<Graph>::Function purpose) :
	  world_(world),engine_(engine),purpose_(purpose) {
		thread_ = new boost::thread(boost::bind(&DynagraphThread::go,this));
	}
	~DynagraphThread() {
		delete thread_;
	}
	void go() {
		try {
			(engine_->*purpose_)();
		}
		catch(Assertion sert) {
			LOCK_REPORT(dgr::output);
			reports[dgr::output] << "message \"(exception) Assertion: " << sert.expr << "; " << sert.file << ", " << sert.line << '"' << std::endl;
			exit(23);
		}
		catch(DGException2 dgx) {
			LOCK_REPORT(dgr::output);
			reports[dgr::output] << "message \"(exception) " << dgx.exceptype << ": " << dgx.param << '"' << std::endl;
			exit(23);
		}
		catch(DGException dgx) {
			LOCK_REPORT(dgr::output);
			reports[dgr::output] << "message \"(exception) " << dgx.exceptype << '"' << std::endl;
			exit(23);
		}
		catch(...) {
			LOCK_REPORT(dgr::output);
			reports[dgr::output] << "message \"(exception) unknown exception\"" << std::endl;
			exit(23);
		}
	}
	void interrupt() {
		dgassert(boost::thread()!=*thread_);
		dgassert(!gd<Interruptible>(&world_.whole_).interrupt);
		gd<Interruptible>(&world_.whole_).interrupt = true;
		wait();
		gd<Interruptible>(&world_.whole_).interrupt = false;
	}
	void wait() {
		thread_->join();
	}
};

} // namespace Dynagraph

#endif // DynagraphThread_h
