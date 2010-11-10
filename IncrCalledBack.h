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

#ifndef IncrCalledBack_h
#define IncrCalledBack_h

#include "incrface/IncrLangEvents.h"

namespace Dynagraph {

struct IncrCalledBack : IncrCallbacks {
    IncrCalledBack() {
        g_incrCallback = this;
    }
    ~IncrCalledBack() {
		g_incrCallback = 0;
    }
    void incr_cb_create_handler(Name name,StrAttrs &attrs);
    void incr_cb_destroy_handler(IncrLangEvents *h);
    void incr_cb_fulfil_graph(DString name,StrGraph *sg);
    void incr_cb_fulfil_node(DString graphname,DString nodename,const StrAttrs &attrs);
    void incr_cb_fulfil_edge(DString graphname,DString edgename,const StrAttrs &attrs);
    void incr_cb_message(const char *msg);
};
extern IncrCalledBack g_incrPhone;
extern StrAttrs g_defaultGraphAttrs;

struct HandlerCreationFailed : DGException {
	HandlerCreationFailed() : DGException("failed to create layout or engines",true) {}
};

} // namespace Dynagraph

#endif // IncrCalledBack_h
