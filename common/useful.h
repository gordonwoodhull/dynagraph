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

#include "dgxep.h"
#include "time-o-matic.h"
#include <stdio.h>

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
	return f>=0.0?int(f + .5):int(f - .5);
}
// silly little
template<typename T>
inline bool assign(T &a,const T &b) {
	if(a!=b)
		return a = b,true;
	return false;
}
// debugging things.  even for internal errors, exceptions are more useful
// than things based on abort()
// assert compiles out in release builds, whereas check doesn't
struct Assertion : DGException {
	const char *expr,*file;
	int line;
	Assertion(const char *expr,const char *file,int line) 
		: DGException("assertion failure",true),expr(expr),file(file),line(line) {}
};
#undef assert
#undef check
#ifndef NDEBUG
#define assert(expr) do { if(!(expr)) throw Assertion(#expr,__FILE__,__LINE__); } while(0)
#define check(expr) do { if(!(expr)) throw Assertion(#expr,__FILE__,__LINE__); } while(0)
#else
#define assert(X)
#define check(X) (X)
#endif

#pragma warning (disable : 4800)

// cross-platform debug report mechanism
void report(int rt,char *msg,...);
void vreport(int rt, char *msg,va_list va);
enum reportTypes {r_dynadag,r_cmdline,r_crossopt,r_wander,r_stats,r_error,
	r_splineRoute,r_shortestPath,r_grChange,r_timing,r_exchange,r_nsdump,
	r_ranks,r_xsolver,r_modelDump,r_ranker,r_dumpQueue,r_stability,r_readability,r_progress,
	r_bug
};
void enableReport(int rt,FILE *f = stdout);
bool reportEnabled(int rt);
void shush(bool whether);

// writes fields to a file.  deals with combining reports that are going to the same file
struct LoopMinder {
	char sep;
	void Start(int rt);
	void Field(int rt,char *colname,double val);
	void Finish(int rt);
	void Cancel();
private:
	struct FieldSet { // all the data that's going into one file
		enum {start,first,loop,done} state;
		std::vector<std::string> names;
		std::vector<double> data;
		FieldSet() : state(start) {}
	};
	typedef std::map<FILE*,FieldSet> FieldSets;
	FieldSets m_fieldSets;
	void doField(FieldSet &f,char *colname,double val);
};
extern LoopMinder loops;

} // namespace Dynagraph

#endif
