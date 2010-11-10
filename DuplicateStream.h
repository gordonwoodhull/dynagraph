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

#ifndef DuplicateStream_h
#define DuplicateStream_h

#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/bind.hpp>
#include <fcntl.h>
#ifdef WIN32
#include <io.h>
inline void pipe(int fd[2]) {
	const int pipe_buffer_size=20000;
	_pipe(fd,pipe_buffer_size,_O_TEXT);
}
#define fdopen _fdopen //?
#else
#include <unistd.h>
#endif


namespace Dynagraph {

extern int g_maxWait;
extern bool g_randomizeWait;

struct DuplicateIn {
	DuplicateIn(FILE *input, std::ostream &log);
	~DuplicateIn() {
		delete thread_;
	}
	FILE *getNewInput() {
		return fromPipe_;
	}
private:
	FILE *input_,*toPipe_,*fromPipe_;
	std::ostream &log_;
	boost::thread *thread_;
	const int RAND_MULT;
	void go();
};
/* (unused)
struct DuplicateOut {
	FILE *output_,*log_,*toPipe_,*fromPipe_;
	boost::thread *thread_;

	DuplicateIn(FILE *output, FILE *log) : output_(output), log_(log) {
		int fd[2];
		_pipe(fd,1000,_O_BINARY);
		toPipe_ = fdopen(fd[1],"w");
		setvbuf(toPipe_,0,_IONBF,0);
		fromPipe_ = fdopen(fd[0],"r");
		thread_ = new boost::thread(boost::bind(&DuplicateIn::go,this));
	}
	~DuplicateIn() {
		delete thread_;
	}
	FILE *getNewOutput() {
		return toPipe_;
	}
	void go() {
		while(!feof(input_)) {
			char buf[1000];
			if(fgets(buf,1000,fromPipe_)) {
				fputs(buf,log_);
				fputs(buf,output_);
			}
		}
		fclose(output_);
	}
};
*/
} // namespace Dynagraph

#endif // DuplicateStream_h
