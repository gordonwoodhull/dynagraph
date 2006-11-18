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


#ifndef useful_h
#define useful_h

#include <string>
#include <vector>
#include <map>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include "dgxep.h"

namespace Dynagraph {

template<typename T>
inline bool sequence(T a,T b,T c) {
	return a <= b && b <= c;
}
template<typename T>
inline T absol(T n) {
	return n<T(0) ? -n : n;
}
/*  don't use "round(..)" as it collides with a system definition */
inline int ROUND(double f) {
	return int(f + ((f>=0.)?.5:-.5));
}
// silly little
template<typename T>
inline bool assign(T &a,const T &b) {
	if(a!=b)
		return a = b,true;
	return false;
}
// from http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
// which does not recommend it but converting to integers seems overkill
template<typename F,typename Maxes>
inline bool is_vclose_howclose(F A, F B) {
	if (fabs(A - B) < Maxes::Absolute())
		return true;
	F relativeError = fabs((A - B) / std::max(A,B));
	if (relativeError <= Maxes::Relative())
		return true;
	return false;
}
struct DoubleMaxes {
	static double Absolute() {
		return 0.00000000001;
	}
	static double Relative() {
		return 0.0000000000001;
	}
};
inline bool is_vclose(double A,double B) {
	return is_vclose_howclose<double,DoubleMaxes>(A,B);
}
struct FloatMaxes {
	static float Absolute() {
		return 0.00001f;
	}
	static float Relative() {
		return 0.000001f;
	}
};
inline bool is_vclose(float A,float B) {
	return is_vclose_howclose<float,FloatMaxes>(A,B);
}

template<typename T>
inline bool assign_unclose(T &a,const T &b) {
	if(!is_vclose(a,b))
		return a = b, true;
	return false;
}

// debugging things.  even for internal errors, exceptions are more useful
// than things based on abort()
// dgassert compiles to nothing in release builds, whereas dgcheck keeps the statement
struct Assertion : DGException {
	const char *expr,*file;
	int line;
	Assertion(const char *expr,const char *file,int line) 
		: DGException("assertion failure",true),expr(expr),file(file),line(line) {}
};
#undef dgassert
#undef dgcheck
#ifndef NDEBUG
#define dgassert(expr) do { if(!(expr)) throw Assertion(#expr,__FILE__,__LINE__); } while(0)
#define dgcheck(expr) do { if(!(expr)) throw Assertion(#expr,__FILE__,__LINE__); } while(0)
#else
#define dgassert(X)
#define dgcheck(X) (X)
#endif

#pragma warning (disable : 4800)

struct dgr {
	enum reportType {none,input_raw,input_cooked,output,inner_input,inner_output,dynadag,cmdline,crossopt,wander,stats,error,
		splineRoute,shortestPath,grChange,timing,exchange,nsdump,
		ranks,xsolver,modelDump,ranker,dumpQueue,stability,readability,progress,
		bug
	};
	std::ostream &operator [](reportType rt) {
		if(gotIt(rt))
			return *streams_[rt];
		else 
			return null_stream();
	}
	void enable(reportType rt,std::ostream *str = &std::cout) {
		if(unsigned(rt)>=streams_.size())
			streams_.resize(rt+1,0);
		streams_[rt] = str;
	}
	bool enabled(reportType rt) {
		return gotIt(rt);
	}
#ifndef DYNAGRAPH_NO_THREADS
	boost::mutex &mutex(reportType rt) {
		dgassert(rt>=0);
		if(unsigned(rt)>=mutices_.size())
			mutices_.resize(rt+1,0);
		boost::mutex *&pm = mutices_[rt];
		if(!pm)
			pm = new boost::mutex;
		return *pm;
	}
	typedef boost::mutex::scoped_lock lock;
#endif //DYNAGRAPH_NO_THREADS
private:
	std::vector<std::ostream*> streams_;
#ifndef DYNAGRAPH_NO_THREADS
	std::vector<boost::mutex*> mutices_;
#endif
	bool gotIt(reportType rt) {
		return ! (rt==none || unsigned(rt)>=streams_.size() || !streams_[rt]);
	}
	static std::ostream &null_stream();
};
extern dgr reports;
#ifndef DYNAGRAPH_NO_THREADS
#define LOCK_REPORT(rt) dgr::lock lock(reports.mutex(rt))
#else
#define LOCK_REPORT(rt)
#endif
// writes fields to a file.  deals with combining reports that are going to the same file
struct LoopMinder {
	char sep;
	void Start(dgr::reportType rt);
	void Field(dgr::reportType rt,char *colname,double val);
	void Finish(dgr::reportType rt);
	void Cancel();
private:
	struct FieldSet { // all the data that's going into one file
		enum {start,first,loop,done} state;
		std::vector<std::string> names;
		std::vector<double> data;
		FieldSet() : state(start) {}
	};
	typedef std::map<std::ostream*,FieldSet> FieldSets;
	FieldSets m_fieldSets;
	FieldSet &getFieldSet(dgr::reportType rt) {
		return m_fieldSets[&reports[rt]];
	}
	void doField(FieldSet &f,char *colname,double val);
};
extern LoopMinder loops;

} // namespace Dynagraph

#endif
