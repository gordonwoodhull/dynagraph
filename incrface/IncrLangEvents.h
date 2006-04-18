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

#ifndef IncrLangEvents_h
#define IncrLangEvents_h

namespace Dynagraph {

struct IncrLangEvents {
	virtual ~IncrLangEvents() {}
	virtual DString dinotype() = 0; // returns "layout" "abstract" etc so xlators etc can downcast
	virtual void incr_ev_open_graph(DString graph,const StrAttrs &attrs) = 0;
	virtual void incr_ev_close_graph() = 0;
	virtual void incr_ev_mod_graph(const StrAttrs &attrs) = 0;
	virtual void incr_ev_lock() = 0;
	virtual void incr_ev_unlock() = 0;
	virtual DString incr_ev_ins_node(DString name, const StrAttrs &attrs, bool merge) = 0;
	virtual DString incr_ev_ins_edge(DString name, DString tailname, DString headname, const StrAttrs &attrs) = 0;
	virtual void incr_ev_mod_node(DString name,const StrAttrs &attrs) = 0;
	virtual void incr_ev_mod_edge(DString name,const StrAttrs &attrs) = 0;
	virtual void incr_ev_del_node(DString name) = 0;
	virtual void incr_ev_del_edge(DString name) = 0;
	virtual void incr_ev_req_graph() = 0;
	virtual void incr_ev_req_node(DString name) = 0;
	virtual void incr_ev_req_edge(DString name) = 0;
    virtual void incr_ev_load_strgraph(StrGraph *sg,bool merge, bool del) = 0;
};
struct IncrCallbacks {
	virtual ~IncrCallbacks() {}
	virtual IncrLangEvents *incr_cb_create_handler(Name name,const StrAttrs &attrs) = 0;
	virtual void incr_cb_destroy_handler(IncrLangEvents *hand) = 0;
	virtual void incr_cb_fulfil_graph(DString name,StrGraph *sg) = 0;
	virtual void incr_cb_fulfil_node(DString graphname,DString nodename,const StrAttrs &attrs) = 0;
	virtual void incr_cb_fulfil_edge(DString graphname,DString edgename,const StrAttrs &attrs) = 0;
	virtual void incr_cb_message(const char *msg) = 0;
};
extern IncrCallbacks *g_incrCallback; // in incrcmds.cpp; fill this before calling incr_yyparse

extern void incr_set_handler(DString name,IncrLangEvents *handler);
extern IncrLangEvents *incr_get_handler(DString name);
struct IncrSetHandlerFirst : DGException { IncrSetHandlerFirst() : DGException("set handler before calling incr_set_allow_reopen()",true) {} };
extern void incr_set_allow_reopen(DString name,bool whether);

} // namespace Dynagraph

#endif // IncrLangEvents_h
