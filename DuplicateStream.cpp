/**********************************************************
*            This software is part of Dynagraph           *
*                http://www.dynagraph.org/                *
*                                                         *
*            Copyright (c) 2006 Gordon Woodhull           *
*        Portions Copyright (c) 1994-2005 AT&T Corp.      *
*  Licensed under the Common Public License, Version 1.0  *
*                                                         *
*      Dynagraph originates in the Graphviz toolset       *
*                   http://graphviz.org                   *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
*                       Many thanks.                      *
**********************************************************/

#include "DuplicateStream.h"
#include <cstring>

namespace Dynagraph {

int g_maxWait=-1;
bool g_randomizeWait = false;

DuplicateIn::DuplicateIn(FILE *input, std::ostream &log) : input_(input), log_(log),RAND_MULT(2*g_maxWait/RAND_MAX) {
	int fd[2];
	pipe(fd);
	toPipe_ = fdopen(fd[1],"w");
	setvbuf(toPipe_,0,_IONBF,0);
	fromPipe_ = fdopen(fd[0],"r");
	thread_ = new boost::thread(boost::bind(&DuplicateIn::go,this));
}
void DuplicateIn::go() {
	while(!feof(input_)) {
		char buf[1000];
		if(fgets(buf,1000,input_)) {
			if(g_maxWait>=0) {
				int wait = g_maxWait?(g_randomizeWait?rand()*RAND_MULT%g_maxWait:g_maxWait):0;
				if(wait) {
					boost::xtime cur;
					int res = boost::xtime_get(&cur,boost::TIME_UTC_);
					if(res==boost::TIME_UTC_) {
						boost::xtime fin = cur;
						fin.nsec += wait;
						boost::thread().sleep(fin);
					}
				}
				//else boost::thread().yield();
			}
			log_.write(buf,std::strlen(buf));
			fputs(buf,toPipe_);
		}
	}
	fclose(toPipe_);
}

} // namespace Dynagraph
