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

#ifndef diff_strgraph_h
#define diff_strgraph_h

#include "randomName.h"

namespace Dynagraph {

inline StrAttrs *diff_attr(StrAttrs &a1,StrAttrs &a2) {
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
		else if(i1->first<i2->first) {// attr only in first
			if(!ret)
				ret = new StrAttrs;
			(*ret)[i1->first] = 0;
			i1++;
		}
		else { // attr only in second
			if(!ret)
				ret = new StrAttrs;
			(*ret)[i2->first] = i2->second;
			i2++;
		}
	return ret;
}
/*
struct React {
	void ins(NGraph2::Node *n);
	void ins(NGraph2::Edge *e);
	void mod(NGraph1::Node *n1,NGraph2::Node *n2,StrAttrs *attrs);
	void mod(NGraph1::Edge *e1,NGraph2::Edge *e2,StrAttrs *attrs);
	void del(NGraph1::Node *n);
	void del(NGraph1::Edge *e);
}
*/

template<typename React,typename NGraph1,typename NGraph2>
void diff_strgraph(NGraph1 *sg1,NGraph2 *sg2,React &react) {
	{ // find deletions
		for(typename NGraph1::graphedge_iter ei1 = sg1->edges().begin(),ej1; ei1!=sg1->edges().end(); ei1=ej1) {
            (ej1 = ei1)++;
            bool gone = true;
			Name name = gd<Name>(*ei1);
			if(name && !isRandomName(name,'e'))
				gone = sg2->fetch_edge(name)==0;
			else {
				if(typename NGraph2::Node *t2 = sg2->fetch_node(gd<Name>((*ei1)->tail))) 
					if(typename NGraph2::Node *h2 = sg2->fetch_node(gd<Name>((*ei1)->head)))
						if(sg2->has_edge(t2,h2))
							gone = false;
			}
            if(gone)
                react.del(*ei1);
		}
        for(typename NGraph1::node_iter ni1 = sg1->nodes().begin(),nj1; ni1!=sg1->nodes().end(); ni1 = nj1) {
            (nj1 = ni1)++;
			if(!sg2->fetch_node(gd<Name>(*ni1)))
				react.del(*ni1);
        }
	}
	{ // find insertions & modifications
		for(typename NGraph2::node_iter ni2 = sg2->nodes().begin(); ni2!=sg2->nodes().end(); ++ni2) {
			if(typename NGraph1::Node *n1 = sg1->fetch_node(gd<Name>(*ni2))) {
				if(StrAttrs *diff = diff_attr(gd<StrAttrs>(n1),gd<StrAttrs>(*ni2))) {
					react.mod(n1,*ni2,diff);
					delete diff;
				}
			}
			else
				react.ins(*ni2);
		}
		for(typename NGraph2::graphedge_iter ei2 = sg2->edges().begin(); ei2!=sg2->edges().end(); ++ei2) {
			typename NGraph1::Edge *e = 0;
			if(poorEdgeName(gd<Name>(*ei2).c_str())) {
                gd<Name>(*ei2) = randomName('e');
				e = sg1->fetch_edge(gd<Name>((*ei2)->tail),gd<Name>((*ei2)->head));
			}
			else
				e = sg1->fetch_edge(gd<Name>(*ei2));
			if(e) {
				if(StrAttrs *diff = diff_attr(gd<StrAttrs>(e),gd<StrAttrs>(*ei2))) {
					react.mod(e,*ei2,diff);
					delete diff;
				}
			}
			else react.ins(*ei2);
		}
	}
}

} // namespace Dynagraph

#endif //diff_strgraph_h
