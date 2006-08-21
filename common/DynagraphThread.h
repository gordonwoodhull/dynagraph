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

extern bool g_xeptOut;

namespace Dynagraph {

template<typename Graph>
struct DynagraphThread {
	ChangingGraph<Graph> &world_;
	ChangeProcessor<Graph> *engine_;
	boost::thread *thread_;
	//DGException2 xep_;
	//bool xepted_;

	DynagraphThread(ChangingGraph<Graph> &world,ChangeProcessor<Graph> *engine) :
	  world_(world),engine_(engine) /*,xep_("","",false),xepted_(false)*/ {
		thread_ = new boost::thread(boost::bind(&DynagraphThread::go,this));
	}
	~DynagraphThread() {
		delete thread_;
	}
	void go() {
		try {
			engine_->Process();
		}
		catch(Assertion sert) {
			LOCK_REPORT(dgr::output);
			reports[dgr::output] << "message \"(exception) Assertion: " << sert.expr << "; " << sert.file << ", " << sert.line << '"' << endl;
			if(g_xeptOut)
				throw;
			exit(23);
		}
		catch(DGException2 dgx) {
			LOCK_REPORT(dgr::output);
			reports[dgr::output] << "message \"(exception) " << dgx.exceptype << ": " << dgx.param << '"' << endl;
			if(g_xeptOut)	
				throw;
			exit(23);
		}
		catch(DGException dgx) {
			LOCK_REPORT(dgr::output);
			reports[dgr::output] << "message \"(exception) " << dgx.exceptype << '"' << endl;
			if(g_xeptOut)
				throw;
			exit(23);
		}
		catch(...) {
			LOCK_REPORT(dgr::output);
			reports[dgr::output] << "message \"(exception) unknown exception\"" << endl;
			exit(23);
		}
	}
	void interrupt() {
		dgassert(boost::thread()!=*thread_);
		gd<Interruptible>(&world_.whole_).interrupt = true;
		thread_->join();
		gd<Interruptible>(&world_.whole_).interrupt = false;
	}
};

} // namespace Dynagraph

#endif // DynagraphThread_h
