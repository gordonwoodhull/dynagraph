/*   Copyright (c) AT&T Corp.  All rights reserved.
   
This software may only be used by you under license from 
AT&T Corp. ("AT&T").  A copy of AT&T's Source Code Agreement 
is available at AT&T's Internet website having the URL 

http://www.research.att.com/sw/tools/graphviz/license/

If you received this software without first entering into a license 
with AT&T, you have an infringing copy of this software and cannot 
use it without violating AT&T's intellectual property rights. */


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
	sg1->oopsRefreshDictionary();
	sg2->oopsRefreshDictionary();
	{ // find deletions
		for(NGraph1::graphedge_iter ei1 = sg1->edges().begin(),ej1; ei1!=sg1->edges().end(); ei1=ej1) {
            (ej1 = ei1)++;
			// find edges by head and tail because prob has no name.
			NGraph2::Node *t2=0,*h2=0;
			NGraph2::NDict::iterator di2 = sg2->ndict.find(gd<Name>((*ei1)->tail));
            bool gone = true;
			if(di2!=sg2->ndict.end()) {
				t2 = di2->second;
				di2 = sg2->ndict.find(gd<Name>((*ei1)->head));
				if(di2!=sg2->ndict.end()) {
					h2 = di2->second;
					if(sg2->find_edge(t2,h2)) 
                        gone = false;
				}
			}
            if(gone)
                react.del(*ei1);
		}
        for(NGraph1::node_iter ni1 = sg1->nodes().begin(),nj1; ni1!=sg1->nodes().end(); ni1 = nj1) {
            (nj1 = ni1)++;
			if(sg2->ndict.find(gd<Name>(*ni1))==sg2->ndict.end()) 
				react.del(*ni1);
        }
	}
	{ // find insertions & modifications
		for(NGraph2::node_iter ni2 = sg2->nodes().begin(); ni2!=sg2->nodes().end(); ++ni2) {
			NGraph1::NDict::iterator di1 = sg1->ndict.find(gd<Name>(*ni2));
			if(di1!=sg1->ndict.end()) {
				if(StrAttrs *diff = diff_attr(gd<StrAttrs>(di1->second),gd<StrAttrs>(*ni2))) {
					react.mod(di1->second,*ni2,diff);
					delete diff;
				}
			}
			else
				react.ins(*ni2);
		}
		for(NGraph2::graphedge_iter ei2 = sg2->edges().begin(); ei2!=sg2->edges().end(); ++ei2) {
            if(poorEdgeName(gd<Name>(*ei2).c_str()))
                gd<Name>(*ei2) = randomName('e');
			DString t = gd<Name>((*ei2)->tail),
				h = gd<Name>((*ei2)->head);
			NGraph1::NDict::iterator di1 = sg1->ndict.find(t);
			if(di1==sg1->ndict.end()) {
				react.ins(*ei2);
				continue;
			}
			NGraph1::Node *t1 = di1->second;
			di1 = sg1->ndict.find(h);
			if(di1==sg1->ndict.end()) {
				react.ins(*ei2);
				continue;
			}
			NGraph1::Node *h1 = di1->second;
			if(NGraph1::Edge *e = sg1->find_edge(t1,h1)) {
				if(StrAttrs *diff = diff_attr(gd<StrAttrs>(e),gd<StrAttrs>(*ei2))) {
					react.mod(e,*ei2,diff);
					delete diff;
				}
			}
			else react.ins(*ei2);
		}
	}
}
