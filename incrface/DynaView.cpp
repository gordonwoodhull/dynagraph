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


#include "common/Dynagraph.h"
#include "DynaView.h"
#include "common/stringsIn.h"
#include "common/stringsOut.h"
#include "common/emitGraph.h"
#include "common/engine_replacement.h"

using namespace std;

DynaView::DynaView(Name name,Transform *transform, bool useDotDefaults) :
	current(&layout),
    old(&layout),
	Q(&layout,&current),
	dgserver(0),
	locks(0),
    m_transform(transform),
    m_useDotDefaults(useDotDefaults),
    m_replacementFlag(false),
    m_allowOneReopen(false)
{
	if(name.empty())
		name = randomName('v');
	gd<Name>(&layout) = name;
}
DynaView::~DynaView() {
	if(dgserver)
		delete dgserver;
}
void DynaView::createServer() {
	if(dgserver) {
		pre_engine_replacement(dgserver,Q,old);
		delete dgserver;
		m_replacementFlag = true;
	}
	dgserver = createLayoutServer(&layout,&current);
}
void DynaView::completeReplacement() {
	post_engine_replacement(Q,old);
}
pair<Layout::Node*,bool> DynaView::getNode(DString name,bool create) {
	if(name.empty())
		name = randomName('n');
	Layout::Node *n = nodes[name];
	if(n)
		return make_pair(n,false);
	if(!create)
		return make_pair<Layout::Node*>(0,false);
    // NodeAttrs *NA = new NodeAttrs();
	n = layout.create_node();//*NA);
    //delete NA;
	gd<Name>(n) = name;
	nodes[name] = n;
	return make_pair(n,true);
}
pair<Layout::Edge*,bool> DynaView::getEdge(DString name,Layout::Node *t,Layout::Node *h,bool create) {
	Layout::Edge *e=0;
	if(name.empty())
		name = randomName('e');
	else if((e = edges[name])) {
		if(t && t!=e->tail || h && h!=e->head)
			throw DVEdgeReopen(name.c_str());
		return make_pair(e,false);
	}
	if(!(t && h))
		return make_pair<Layout::Edge*>(0,false);
	e = layout.find_edge(t,h);
	if(e)
		return make_pair(e,false);
	if(!create)
		return make_pair<Layout::Edge*>(0,false);
	e = layout.create_edge(t,h).first;
	gd<Name>(e) = name;
	edges[name] = e;
	return make_pair(e,true);
}
pair<Layout::Edge*,bool> DynaView::getEdge(DString name,DString tail,DString head,bool create) {
	assert(tail.size()&&head.size());
	Layout::Node *t = getNode(tail,false).first,
		*h = getNode(head,false).first;
	if(!t)
		throw DVEdgeTailDoesNotExist(tail.c_str());
	if(!h)
		throw DVEdgeHeadDoesNotExist(head.c_str());
	return getEdge(name,t,h,create);
}
Layout::Edge *DynaView::getEdge(DString name,DString tail,DString head) {
	return getEdge(name,tail,head,false).first;
}
Layout::Edge *DynaView::getEdge(DString name) {
	return getEdge(name,(Layout::Node*)0,(Layout::Node*)0,false).first;
}
void DynaView::rename(DString newName) {
    DinoMachine::Node *n = g_dinoMachine.ndict[gd<Name>(&layout)];
    assert(n);
    g_dinoMachine.rename(n,newName);
    gd<Name>(&layout) = newName;
}
void DynaView::rename(Layout::Node *n,DString newName) {
	forget(n);
	nodes[gd<Name>(n) = newName] = n;
}
void DynaView::rename(Layout::Edge *e,DString newName) {
	forget(e);
	edges[gd<Name>(e) = newName] = e;
}
void DynaView::forget(Layout::Node *n) {
	nodes.erase(gd<Name>(n));
}
void DynaView::forget(Layout::Edge *e) {
	edges.erase(gd<Name>(e));
}
void DynaView::destroy(Layout::Node *n) {
	forget(n);
	layout.erase(n);
}
void DynaView::destroy(Layout::Edge *e) {
	forget(e);
	layout.erase(e);
}

// incr_ev

bool DynaView::maybe_go() {
    if(locks>0)
        return false;
	bool ch = false;
    while(!Q.Empty()) {
        dgserver->Process(Q);
        if(m_replacementFlag) {
            completeReplacement();
            m_replacementFlag = false;
        }
        stringsOut(m_transform,Q);
        for(Layout::graphedge_iter ei = Q.delE.edges().begin(); ei!=Q.delE.edges().end(); ++ei)
            forget(*ei);
        for(Layout::node_iter ni = Q.delN.nodes().begin(); ni!=Q.delN.nodes().end(); ++ni)
            forget(*ni);
        IncrHappened(); // must clear Q but the events might cause more changes
		ch = true;
    }
	if(ch)
		g_dinoMachine.changed(gd<Name>(&layout));
    return true;
}
Update DynaView::open_layout(const StrAttrs &attrs) {
    bool create = false;
    if(!dgserver)
        create = true;
    else {
        StrAttrs::const_iterator ai = attrs.find("engines");
        if(ai!=attrs.end())
            if(ai->second!=gd<StrAttrs>(&current)["engines"])
                create = true;
    }
    Update ret = stringsIn(m_transform,m_useDotDefaults,&layout,attrs,false).flags;
    if(create)
        createServer();
    return ret;
}
bool DynaView::incr_ev_open_graph(DString graph,const StrAttrs &attrs) {
	bool isModify = false;
    if(dgserver) {
        if(!m_allowOneReopen)
            throw DVReopenXep(graph.c_str());
        m_allowOneReopen = false;
		isModify = true;
    }

    open_layout(attrs);
    if(!dgserver)
        return false;
    incr_set_handler(gd<Name>(&layout) = graph,this);
	cout << (isModify?"modify":"open") << " graph " << graph << " " << gd<StrAttrs2>(&layout) << endl;
	gd<StrAttrChanges>(&layout).clear();
    return true;
}
bool DynaView::incr_ev_close_graph() {
    cout << "close graph " << gd<Name>(&layout) << endl;
    return true;
}
bool DynaView::incr_ev_mod_graph(const StrAttrs &attrs) {
    Q.GraphUpdateFlags() |= open_layout(attrs).flags;
    maybe_go();
    return true;
}
bool DynaView::incr_ev_lock() {
    locks++;
    return true;
}
bool DynaView::incr_ev_unlock() {
    --locks;
    maybe_go();
    return true;
}
DString DynaView::incr_ev_ins_node(DString name, const StrAttrs &attrs, bool merge) {
    DString rename = name;
    pair<Layout::Node *,bool> nb = getNode(name,true);
    if(!nb.second&&!merge) {
        // name already was used, so make an anónimo and label with the name
        nb.second = true;
        nb.first = getNode(0,true).first;
        gd<StrAttrs>(nb.first)["label"] = name;
        rename = gd<Name>(nb.first);
    }
    Update upd = stringsIn(m_transform,nb.first,attrs,true);
    if(nb.second) {
        Q.InsNode(nb.first);
        IncrNewNode(nb.first);
    }
    else
        Q.ModNode(nb.first,upd);
    maybe_go();
    return rename;
        /*
        pair<Layout::Node*,bool> nb = getNode(name,true);
        Update upd = stringsIn(m_transform,nb.first,attrs,true);
        if(!current.find(nb.first)) {
		    if(nb.second)
			    IncrNewNode(nb.first);
            Q.InsNode(nb.first);
        }
        else // treat re-insert as modify
            Q.ModNode(nb.first,upd);
        maybe_go();
        */
}
DString DynaView::incr_ev_ins_edge(DString name, DString tailname, DString headname, const StrAttrs &attrs) {
    pair<Layout::Edge*,bool> eb = getEdge(name,tailname,headname,true);
	if(!eb.second&&gd<Name>(eb.first)!=name)
		throw DVEdgeNameMismatch(name.c_str());
    Update upd = stringsIn(m_transform,eb.first,attrs,true);
    if(eb.second) {
        Q.InsEdge(eb.first);
        IncrNewEdge(eb.first);
    }
    else
        Q.ModEdge(eb.first,upd);
    maybe_go();
    return gd<Name>(eb.first);
        /*
        pair<Layout::Edge*,bool> eb = getEdge(name,tail,head,true);
        if(!eb.second && gd<Name>(eb.first)!=name) {
            incr_error(IF_ERR_NAME_MISMATCH,graph);
            return;
        }
        Update upd = stringsIn(m_transform,eb.first,attrs,true);
        if(!current.find(eb.first)) {
		    if(eb.second)
			    IncrNewEdge(eb.first);
            Q.InsEdge(eb.first);
        }
        else
            Q.ModEdge(eb.first,upd);
        maybe_go();
        */
}
bool DynaView::incr_ev_mod_node(DString name,const StrAttrs &attrs) {
    Layout::Node *n = getNode(name).first;
    if(!n)
        throw DVNodeDoesNotExist(name.c_str());
    Q.ModNode(n,stringsIn(m_transform,n,attrs,false));
    maybe_go();
    return true;
}
bool DynaView::incr_ev_mod_edge(DString name,const StrAttrs &attrs) {
    Layout::Edge *e = getEdge(name);
    if(!e)
        throw DVEdgeDoesNotExist(name.c_str());
    Q.ModEdge(e,stringsIn(m_transform,e,attrs,false));
    maybe_go();
    return true;
}
bool DynaView::incr_ev_del_node(DString name) {
	Layout::Node *n = getNode(name).first;
    if(!n)
        throw DVNodeDoesNotExist(name.c_str());
    Q.DelNode(n);
    maybe_go();
    return true;
}
bool DynaView::incr_ev_del_edge(DString name) {
    Layout::Edge *e = getEdge(name);
    if(!e)
        throw DVEdgeDoesNotExist(name.c_str());
    Q.DelEdge(e);
    maybe_go();
    return true;
}
bool DynaView::incr_ev_req_graph() {
	cout << "fulfil graph " << gd<Name>(&layout) << endl;
	emitGraph(cout,&layout);
    return true;
}
bool DynaView::incr_ev_req_node(DString name) {
    Layout::Node *n = getNode(name).first;
    if(!n)
        throw DVNodeDoesNotExist(name.c_str());
	cout << "fulfil node " << gd<Name>(&layout) << " " << name << " " << gd<StrAttrs>(n);
    return true;
}
bool DynaView::incr_ev_req_edge(DString name) {
    Layout::Edge *e = getEdge(name);
    if(!e)
        throw DVEdgeDoesNotExist(name.c_str());
    cout << "fulfil edge " << gd<Name>(&layout) << " " << name << " " << gd<StrAttrs>(e);
    return true;
}
void DynaView::incr_ev_load_strgraph(StrGraph *sg,bool merge, bool del) {
    typedef map<DString,StrGraph::Node*> strnode_dict;
    incr_ev_lock();
    if(merge && del) { // find deletions
        strnode_dict nd;
        for(StrGraph::node_iter sni = sg->nodes().begin(); sni!=sg->nodes().end(); ++sni)
            nd[gd<Name>(*sni)] = *sni;
        for(Layout::node_iter ni = current.nodes().begin(); ni!=current.nodes().end(); ++ni)
            if(nd.find(gd<Name>(*ni))==nd.end()) {
                for(Layout::nodeedge_iter ei = (*ni)->alledges().begin(); ei!=(*ni)->alledges().end(); ++ei)
                    incr_ev_del_edge(gd<Name>(*ei));
                incr_ev_del_node(gd<Name>(*ni));
            }
        for(Layout::graphedge_iter ei = current.edges().begin(); ei!=current.edges().end(); ++ei) {
            // find edges by head and tail because prob has no name.
            StrGraph::Node *t=0,*h=0;
            strnode_dict::iterator di = nd.find(gd<Name>((*ei)->tail));
            if(di!=nd.end()) {
                t = di->second;
                di = nd.find(gd<Name>((*ei)->head));
                if(di!=nd.end()) {
                    h = di->second;
                    if(!sg->find_edge(t,h))
                        incr_ev_del_edge(gd<Name>(*ei));
                }
            }
            // if tail or head missing we've already called DelNode thus don't need to call DelEdge
        }
    }
    { // find insertions & modifications
        map<DString,DString> nd; // an override dictionary, only if !merge
        for(StrGraph::node_iter ni = sg->nodes().begin(); ni!=sg->nodes().end(); ++ni) {
            DString rename = incr_ev_ins_node(gd<Name>(*ni),gd<StrAttrs>(*ni),merge);
            if(!merge)
                nd[gd<Name>(*ni)] = rename;
        }
        for(StrGraph::graphedge_iter ei = sg->edges().begin(); ei!=sg->edges().end(); ++ei) {
            DString id = gd<Name>(*ei),
                t = gd<Name>((*ei)->tail),
                h = gd<Name>((*ei)->head);
            if(poorEdgeName(id.c_str()))
                id = 0;
            if(!merge) {
                t = nd[t];
                h = nd[h];
            }
			if(Layout::Edge *oldE = getEdge(id,t,h))
				incr_ev_mod_edge(gd<Name>(oldE),gd<StrAttrs>(*ei));
			else
				incr_ev_ins_edge(id,t,h,gd<StrAttrs>(*ei));
        }
    }
    incr_ev_unlock();
}
