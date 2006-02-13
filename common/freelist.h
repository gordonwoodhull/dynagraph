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

#include <stdlib.h>

#ifndef freelist_h
#define freelist_h

namespace Dynagraph {

#define COUNT_ALLOCATED
template<typename T>
struct Freelist {
	T *alloc() {
		if(!head) {
			int amount = sizeof(Blockheader) + blocksize * sizeof(T);
#ifdef COUNT_ALLOCATED
			allocated+=amount;
#endif
			Blockheader *mem = reinterpret_cast<Blockheader*>(malloc(amount));
			char *cp = reinterpret_cast<char*>(mem) + sizeof(Blockheader);
			for(int i=0; i<blocksize; i++)
				this->free(reinterpret_cast<T*>(cp) + i);
			mem->next = blocks;
			blocks = mem;
		}
		Freenode *t = head;
		head = t->nextfree;
		new(t) T;
		return reinterpret_cast<T*>(t);
	}
	void free(T *x) {
		Freenode *f = reinterpret_cast<Freenode*>(x);
		f->nextfree = head;
		head = f;
	}
	void clear() {
		head = 0;
		if(blocks)
			for(Blockheader *bp = blocks,*np; bp; bp = np) {
				np = bp->next;
				::free(bp);
			}
		blocks = 0;
#ifdef COUNT_ALLOCATED
		allocated=0;
#endif
	}
	Freelist(int blocksize) : head(0),blocksize(blocksize),blocks(0) {
#ifdef COUNT_ALLOCATED
		allocated=0;
#endif
	}
	~Freelist() { clear(); }
protected:
	struct Freenode {
		Freenode *nextfree;
	};

	struct Blockheader {
		Blockheader *next;
	};

	Freenode *head; // list of free nodes
	int blocksize; // number of nodes per block
	Blockheader *blocks; // list of blocks
#ifdef COUNT_ALLOCATED
public:
	int allocated;
#endif
};

} // namespace Dynagraph

#endif // freelist_h
