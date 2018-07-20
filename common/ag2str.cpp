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


#include <stdio.h>

#include "ag2str.h"
#include "StrAttr.h"
#include "useful.h"

using namespace std;

namespace Dynagraph {

// lackadaisical C goes head to head with obsessive C++ here
inline char *kc(const char *val) { // kill const
  return const_cast<char*>(val);
}
inline int SetAttr(void *o,int kind,const char *tag,const char *value) {
  agattr(agraphof(o),kind,kc(tag),kc(""));
  return agset(o,kc(tag),kc(value));
}
typedef map<Agnode_t*,StrGraph::Node*> Remem;

StrGraph *doSubgraph(Remem &remem,StrGraph *main,StrGraph *super,Agraph_t *g) {
    StrGraph *ret = new StrGraph(super);
    gd<Name>(ret) = agnameof(g);
    Agnode_t *n;
    for(n = agfstnode(g); n; n=agnxtnode(g, n)) {
        Agnode_t *n0 = agsubnode(g->root,n,false);
        StrGraph::Node *nn0 = remem[n0];
        ret->insert(nn0);
        Agedge_t *e;
        for(e = agfstout(g, n); e; e = agnxtout(g, e)) {
            Agnode_t *h0 = agsubnode(g->root,aghead(e),false);
            StrGraph::Node *hh0 = remem[h0];
            ret->insert(hh0);
        }
    }
    for(Agraph_t *s = agfstsubg(g); s; s = agnxtsubg(s))
        doSubgraph(remem,main,ret,s);
    return ret;
}
StrGraph *ag2str(Agraph_t *g) {
    Remem remem;
    StrGraph *gg = new StrGraph;
    gd<Name>(gg) = agnameof(g);
    /*
    // BAAAD: THIS CODE CRASHED VC++ DEBUGGER.  PLEASE REINSTATE****!!!!!!!
    for(Agsym_t *sym = agnxtattr(g,AGRAPH,0); sym; sym = agnxtattr(g,AGRAPH,sym))
        gd<StrAttrs>(gg)[sym->name] = agget(gg,sym->name);
        */

    int anonN(0);
    Agnode_t *n;
    for(n = agfstnode(g);n;n = agnxtnode(g, n)) {
        char *name = agnameof(n),
            buf[30];
        if(!name || !name[0]) {
            sprintf(buf,"n%d",anonN++);
            name = buf;
        }
        StrGraph::Node *nn = gg->create_node(name);
        remem[n] = nn;
        for(Agsym_t *sym = agnxtattr(g,AGNODE,0); sym; sym = agnxtattr(g,AGNODE,sym)) {
            char *val = agxget(n,sym);
            if(val&&*val)
                gd<StrAttrs>(nn)[sym->name] = val;
        }
    }
    for(n = agfstnode(g);n;n = agnxtnode(g, n)) {
        StrGraph::Node *tail = remem[n];
        for(Agedge_t *e = agfstout(g, n); e; e = agnxtout(g, e)) {
            StrGraph::Node *head = remem[aghead(e)];
            char *name = agnameof(e);
            NamedAttrs nattr(name);
            for(Agsym_t *sym = agnxtattr(g,AGEDGE,0); sym; sym = agnxtattr(g,AGEDGE,sym)) {
                char *val = agget(e,sym->name);
                if(val&&*val)
                    if(!strcmp(sym->name,"id"))
                        (Name&)nattr = val;
                    else
                        ((StrAttrs&)nattr)[sym->name] = val;
            }
            try {
                gg->create_edge(tail,head,nattr);
            }
            catch(DGParallelEdgesNotSupported) {
            }
        }
    }
    for(Agraph_t *s = agfstsubg(g); s; s = agnxtsubg(s))
        doSubgraph(remem,gg,gg,s);
    return gg;
}
Agraph_t *doSubgraph(Agraph_t *super,StrGraph *root,StrGraph *gg) {
    Agraph_t *ret = agsubg(super,kc(gd<Name>(gg).c_str()),true);
    for(StrGraph::node_iter ni = gg->nodes().begin(); ni!=gg->nodes().end(); ++ni) {
        Agnode_t *n0 = agnode(super->root,kc(gd<Name>(*ni).c_str()),false);
        for(StrGraph::outedge_iter ei = (*ni)->outs().begin(); ei!=(*ni)->outs().end(); ++ei) {
            Agnode_t *h0 = agnode(super->root,kc(gd<Name>((*ei)->head).c_str()),false);
            Agedge_t *e0 = agedge(super->root,n0,h0,kc(gd<Name>(*ei).c_str()),false);
            agsubedge(ret,e0,true);
        }
    }
    for(StrGraph::subgraph_iter si = gg->subgraphs().begin(); si!=gg->subgraphs().end(); ++si)
        doSubgraph(ret,root,static_cast<StrGraph*>(*si));
    return ret;
}

Agraph_t *str2ag(StrGraph *gg) {
  Agraph_t *g = agopen(kc(gd<Name>(gg).c_str()),Agstrictdirected,0);
  for(StrAttrs::iterator mi = gd<StrAttrs>(gg).begin(); mi!=gd<StrAttrs>(gg).end(); ++mi)
    SetAttr(g,AGRAPH,mi->first.c_str(),mi->second.c_str());
  StrGraph::node_iter ni;
  for(ni = gg->nodes().begin(); ni!=gg->nodes().end(); ++ni) {
    StrGraph::Node *nn = *ni;
    Agnode_t *n = agnode(g,kc(gd<Name>(nn).c_str()),true);
    reports[dgr::none] << gd<Name>(nn).c_str() << " has" << endl;
    for(StrAttrs::iterator mi = gd<StrAttrs>(nn).begin(); mi!=gd<StrAttrs>(nn).end(); ++mi) {
        reports[dgr::none] << "\t" << mi->first.c_str() << " = " << mi->second.c_str() << endl;
      SetAttr(n,AGNODE,mi->first.c_str(),mi->second.c_str());
    }
  }

  for(ni = gg->nodes().begin(); ni!=gg->nodes().end(); ++ni) {
    Agnode_t *tail = agnode(g,kc(gd<Name>(*ni).c_str()),false);
    for(StrGraph::outedge_iter ei = (*ni)->outs().begin(); ei!=(*ni)->outs().end(); ++ei) {
      Agnode_t *head = agnode(g,kc(gd<Name>((*ei)->head).c_str()),false);
      Agedge_t *e = agedge(g,tail,head,kc(gd<Name>(*ei).c_str()),true);
      if(e) // self edges lost
          for(StrAttrs::iterator mi = gd<StrAttrs>(*ei).begin(); mi!=gd<StrAttrs>(*ei).end(); ++mi)
        SetAttr(e,AGEDGE,mi->first.c_str(),mi->second.c_str());
    }
  }
    for(StrGraph::subgraph_iter si = gg->subgraphs().begin(); si!=gg->subgraphs().end(); ++si)
        doSubgraph(g,gg,static_cast<StrGraph*>(*si));
  return g;
}

} // namespace Dynagraph
