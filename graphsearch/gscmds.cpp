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


#include "common/LayoutAttrs.h"
#include "common/ChangeQueue.h"
#include "common/ChangeProcessor.h"
#include "common/ag2str.h"
#include "common/emitGraph.h"
#include "common/bufferGraphStream.h"
#include "common/randomName.h"
#include "common/diff_strgraph.h"

#include "gscmds.h"
#include "Search.h"

using namespace GSearch;
using namespace std;

static StrAttrs g_currAttrs;
extern StrGraph *g_sourceGraph;

Patterns g_patterns;

typedef map<DString,Search*> Searches;
Searches g_searches;

Inputs g_inputs;

struct Session {
    DString searchname;
    Search *search;
    int locks;
    bool changed;
    Session() : search(0),locks(0),changed(false) {}
};
typedef map<DString,Session> Sessions;
Sessions g_sessions;

bool assign_search(Session &s) {
    StrAttrs::iterator ai;
    ai = g_currAttrs.find("searchname");
    if(ai==g_currAttrs.end())
        return false;
    Searches::iterator si = g_searches.find(ai->second);
    if(si==g_searches.end())
        throw GSError();
    if(s.search)
        delete s.search;
    s.searchname = si->first;
    // copy the search because it has state
    s.search = new Search(*si->second);
    return true;
}

StrGraph &result(Session &s) {
    Search::Node *finish = s.search->dict["finish"];
    if(!finish)
        throw GSError();
    return gd<SearchStage>(finish).result;
}
struct emitReact {
    const char *view;
    void ins(StrGraph::Node *n) {
        reports[dgr::output] << "insert node " << mquote(view) << " " << mquote(gd<Name>(n).c_str()) << " "
            << gd<StrAttrs>(n) << endl;
    }
    void ins(StrGraph::Edge *e) {
        reports[dgr::output] << "insert edge " << mquote(view) << " " << mquote(gd<Name>(e).c_str()) <<
            " " << mquote(gd<Name>(e->tail)) << " " << mquote(gd<Name>(e->head)) << " "
            << gd<StrAttrs>(e) << endl;
    }
    void mod(StrGraph::Node*,StrGraph::Node *n,StrAttrs *attrs) {
        reports[dgr::output] << "modify node " << mquote(view) << " " << mquote(gd<Name>(n).c_str()) << " "
            << *attrs << endl;
    }
    void mod(StrGraph::Edge*,StrGraph::Edge *e,StrAttrs *attrs) {
        reports[dgr::output] << "modify edge " << mquote(view) << " " << mquote(gd<Name>(e).c_str()) << " "
            << *attrs << endl;
    }
    void del(StrGraph::Node *n) {
        reports[dgr::output] << "delete node " << mquote(view) << " " << mquote(gd<Name>(n).c_str()) << endl;
    }
    void del(StrGraph::Edge *e) {
        reports[dgr::output] << "delete edge " << mquote(view) << " " << mquote(gd<Name>(e).c_str()) << endl;
    }
};
void remove_2cycles(StrGraph &g) {
    for(StrGraph::graphedge_iter ei = g.edges().begin(); ei!=g.edges().end();) {
        StrGraph::Edge *e0 = *ei++;
        if(StrGraph::Edge *e = g.find_edge(e0->head,e0->tail)) {
            if(e==*ei)
                ++ei; // tricky iters!
            g.erase(e);
        }
    }
}
void output_result(const char *view,StrGraph &start,Session &s) {
    StrGraph &finish = result(s);
    remove_2cycles(finish);
    emitReact r;
    r.view = view;
    reports[dgr::output] << "lock graph " << mquote(view) << endl;
    diff_strgraph(&start,&finish,r);
    reports[dgr::output] << "unlock graph " << mquote(view) << endl;
    /*
    reports[dgr::output] << "segue graph " << mquote(view) << endl;
    reports[dgr::output].flush();
    emitGraph(reports[dgr::output],&finish);
    */
}
void run(Session &s,const char *view) {
    if(!s.locks) {
        try {
            StrGraph before = result(s);
            s.search->Run(g_inputs);
            output_result(view,before,s);
        }
        catch(...) {
        }
    }
    else
        s.changed = true;
}
void gs_open_graph(char *view)
{
    Session &s = g_sessions[view];
    if(s.search) {
        gs_error(IF_ERR_ALREADY_OPEN,view);
        gs_close_graph(view);
    }
    bool hasSearch = assign_search(s);
    reports[dgr::output] << "open graph " << mquote(view) << ' '
        << g_currAttrs << endl;
    if(hasSearch)
        run(s,view);
}

void gs_close_graph(char *view)
{
    Session &s = g_sessions[view];
    if(s.search) {
        delete s.search;
        g_sessions.erase(view);
    }
    else gs_error(IF_ERR_NOT_OPEN,view);
    reports[dgr::output] << "close graph " << mquote(view) << endl;
}

void gs_mod_graph(char *view) {
    Session &s = g_sessions[view];
    if(!s.locks) {
        reports[dgr::output].flush();
        reports[dgr::output] << "modify graph " << mquote(view) << ' '
            << g_currAttrs << endl;
    }
    if(assign_search(s))
        run(s,view);
}


void gs_lock(char *view) {
    Session &s = g_sessions[view];
    s.locks++;
}
void gs_unlock(char *view) {
    Session &s = g_sessions[view];
    if(!--s.locks && s.changed)
        run(s,view);
}

void gs_segue(char *view) {
    throw NYI();
}
void search_changed(DString sname,bool recopy) {
    Search &search = *g_searches[sname];
    for(Sessions::iterator si = g_sessions.begin(); si!=g_sessions.end(); ++si) {
        Session &session = si->second;
        if(session.searchname==sname) {
            if(recopy) {
                delete session.search;
                session.search = new Search(search);
            }
            else session.search->reset();
            run(session,si->first.c_str());
        }
    }
}
extern FILE *gs_yyin;
void gs_define_pattern() {
    bufferGraphStream fix(gs_yyin);
    StrGraph *sg = readStrGraph(fix.fin);
    if(!sg)
        throw ParseError();
    Pattern &pattern = g_patterns[gd<Name>(sg)];
    pattern.readStrGraph(*sg);
    for(Searches::iterator si = g_searches.begin(); si!=g_searches.end(); ++si)
        for(Search::node_iter sti = si->second->nodes().begin(); sti!=si->second->nodes().end(); ++sti) {
      //Search::Node *stage = *sti;
            if(gd<SearchStage>(*sti).pattern==&pattern)
                search_changed(si->first,false);
        }
    delete sg;
}
void gs_define_search() {
    bufferGraphStream fix(gs_yyin);
    StrGraph *sg = readStrGraph(fix.fin);
    if(!sg)
        throw ParseError();
    Name name = gd<Name>(sg);
    Search *&search = g_searches[name];
    if(search==0)
        search = new Search(*g_sourceGraph);
    try {
        search->readStrGraph(g_patterns,*sg);
        search_changed(name,true);
    }
    catch(UndefinedPattern up) {
        reports[dgr::error] << "undefined pattern " << up.name << endl;
    }
    delete sg;
}
void gs_define_input() {
    bufferGraphStream fix(gs_yyin);
    StrGraph *sg = readStrGraph(fix.fin);
    if(!sg)
        throw ParseError();
    StrGraph *input = new StrGraph(g_sourceGraph);
    try {
        input->readSubgraph(sg);
    }
    catch(NodeNotFound nnf) {
        reports[dgr::error] << "input node " << nnf.name << " not found " << endl;
    }
    catch(EdgeNotFound enf) {
        reports[dgr::error] << "input edge " << enf.tail << " -> " << enf.head << " not found" << endl;
    }
    g_inputs[gd<Name>(sg)] = input;
    for(Sessions::iterator si = g_sessions.begin(); si!=g_sessions.end(); ++si) {
        Session &s = si->second;
        if(!s.search)
            continue;
        StrGraph before = result(s);
        s.search->reset();
        s.search->Run(g_inputs);
        output_result(si->first.c_str(),before,s);
    }
    delete sg;
}
void gs_ins_node(char *view,char *id)
{
}

void gs_mod_node(char *view,char *id)
{
}

void gs_del_node(char *view,char *id)
{
}

void gs_ins_edge(char *view,char *id, char *tail, char *head)
{
}

void gs_mod_edge(char *view,char *id)
{
}

void gs_del_edge(char *view,char *id)
{
}

/* attribute scanning */

void gs_reset_attrs()
{
    g_currAttrs.clear();
}

void gs_append_attr(char *name, char *value)
{
    g_currAttrs[name] = value;
}


/* error handler */

static char *ErrMsg[] = {
    "unknown error",
    "graph/view already open",
    "graph/view not open",
    "name mismatch",
    "syntax error",
    "duplicated ID (node/edge)",
    "not implemented",
    "object doesn't exist",
    (char*)0
} ;
void gs_error(int code, char *str)
{
    if (!str) str = "";
    reports[dgr::error] << "graphsearch interface: " << ErrMsg[code] << ' ' << str << endl;
    throw GSError();
}

void gs_abort(int code)
{
    g_sessions.clear();
    gs_error(code,0);
}

