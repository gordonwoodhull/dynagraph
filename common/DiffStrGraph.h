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


StrAttrs *diffAttr(StrAttrs &a1,StrAttrs &a2) {
	StrAttrs *ret = 0;
	for(StrAttrs::iterator i1 = a1.begin(),i2 = a2.begin(); i1!=a1.end() || i2!=a2.end();)
		if(i1->first==i2->first) { // attr in both
			if(i1->second!=i2->second) {
				if(!ret)
					ret = new StrAttrs;
				(*ret)[i2->first] = i2->second;
			}
			i1++;
			i2++;
		}
		else if(i1->first<i2->first) // attr only in first
			i1++;
		else { // attr only in second
			if(!ret)
				ret = new StrAttrs;
			(*ret)[i2->first] = i2->second;
			i2++;
		}
	return ret;
}
StrGraph *DiffStrGraph(StrGraph *sg1,StrGraph *sg2) {
	StrGraph *ret = new StrGraph,
		*insmod = new StrGraph(ret),
		*delnode = new StrGraph(ret),
		*deledge = new StrGraph(ret);
	gd<Name>(insmod) = "insmod";
	gd<Name>(delnode) = "delnode";
	gd<Name>(deledge) = "deledge";
	typedef std::map<string,StrGraph::Node*> nodedict;
	nodedict dict1,dict2,dict3;
	StrGraph::node_iter ni;
	for(ni = sg1->nodes().begin(); ni !=sg1->nodes().end(); ++ni)
		dict1[gd<Name>(*ni)] = *ni;
	for(ni = sg2->nodes().begin(); ni !=sg2->nodes().end(); ++ni)
		dict2[gd<Name>(*ni)] = *ni;
	nodedict::iterator di1,di2;
	for(di1 = dict1.begin(); di1!=dict1.end(); ++di1)
		if((di2 = dict2.find(di1->first))==dict2.end()) { // delete
			StrGraph::Node *n = ret->create_node();
			dict3[di1->first] = n;
			gd<Name>(n) = di1->first;
			delnode->insert(n);
		}
		else { // mod
			StrAttrs *attr = diffAttr(gd<StrAttrs>(di1->second),gd<StrAttrs>(di2->second));
			if(attr) {
				StrGraph::Node *n = ret->create_node();
				dict3[di1->first] = n;
				gd<Name>(n) = di1->first;
				gd<StrAttrs>(n) = *attr;
				delete attr;
				insmod->insert(n);
			}
		}
	for(di2 = dict2.begin(); di2!=dict2.end(); ++di2)
		if((di1 = dict1.find(di2->first))==dict1.end()) { // insert
			StrGraph::Node *n = ret->create_node();
			dict3[di2->first] = n;
			gd<Name>(n) = di2->first;
			gd<StrAttrs>(n) = gd<StrAttrs>(di2->second);
			insmod->insert(n);
		}
	StrGraph::graphedge_iter ei;
	for(ei = sg1->edges().begin(); ei!=sg1->edges().end(); ++ei) {
		if((di2 = dict2.find(gd<Name>((*ei)->tail)))!=dict2.end()) {
			StrGraph::Node *tail = di2->second;
			if((di2 = dict2.find(gd<Name>((*ei)->head)))!=dict2.end()) {
				StrGraph::Node *head = di2->second;
				if(StrGraph::Edge *e2 = sg2->find_edge(tail,head)) { // edge in both
					if(StrAttrs *attr = diffAttr(gd<StrAttrs>(*ei),gd<StrAttrs>(e2))) {  // modified
						StrGraph::Node *t3 = dict3[gd<Name>((*ei)->tail)],
							*h3 = dict3[gd<Name>((*ei)->head)];
						StrGraph::Edge *e3 = ret->create_edge(t3,h3).first;
						gd<Name>(e3) = gd<Name>(*ei);
						gd<StrAttrs>(e3) = *attr;
						delete attr;
						continue;
						insmod->insert(e3);
					}
				}
			}
		}
		// edge deleted
		StrGraph::Node *t3 = dict3[gd<Name>((*ei)->tail)],
			*h3 = dict3[gd<Name>((*ei)->head)];
		StrGraph::Edge *e3 = ret->create_edge(t3,h3).first;
		gd<Name>(e3) = gd<Name>(*ei);
		deledge->insert(e3);
	}
	for(ei = sg2->edges().begin(); ei!=sg2->edges().end(); ++ei) {
		if((di2 = dict1.find(gd<Name>((*ei)->tail)))!=dict1.end()) {
			StrGraph::Node *tail = di2->second;
			if((di2 = dict1.find(gd<Name>((*ei)->head)))!=dict1.end()) {
				StrGraph::Node *head = di2->second;
				if(!sg1->find_edge(tail,head)) { // insert
					StrGraph::Node *t3 = dict3[gd<Name>((*ei)->tail)],
						*h3 = dict3[gd<Name>((*ei)->head)];
					StrGraph::Edge *e3 = ret->create_edge(t3,h3).first;
					gd<Name>(e3) = gd<Name>(*ei);
					gd<StrAttrs>(e3) = gd<StrAttrs>(*ei);
				}
			}
		}
	}
	return ret;
}