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


#ifndef GRID_H
#define GRID_H

#include "cdt.h"
#include "common/derivable_dt.h"
#include "common/freelist.h"
#include <list>

struct gridpt {
	int i, j;
};
typedef std::list<FDPModel::Node*> node_list;
// this extra struct is to evade gcc3 warnings, which
// do not like offsetof(complicated struct)
struct PODCell {
  gridpt p;
  Dtlink_t link;
};
struct Cell : PODCell {
  node_list nodes;
};
struct Grid : derivable_dt { // cells indexed by (i,j)
	Freelist<Cell> cells; // this is wasteful; isn't deque supposed to not realloc if you just push?
	Grid(int size);
	~Grid() {
		close();
	}
	Cell *getCell();
	void clear() {
		dtclear(this);
		cells.clear();
	}
	void add(int i, int j, FDPModel::Node*);
	Cell* find(int i, int j);
	void walk(int(*walkf)(Dt_t *dt,void *cell,void *grid));
	struct Visitor {
		virtual ~Visitor() {}
		virtual int VisitCell(Cell *cell,Grid *grid) = 0;
	};
	void walk(Visitor *visitor);
};

extern int gLength (Cell* p);

#endif
