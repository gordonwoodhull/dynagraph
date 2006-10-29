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


#ifndef stringsIn_h
#define stringsIn_h

#include <sstream>
#include "Transform.h"
#include "AttrValues.h"

namespace Dynagraph {

void clearRemoved(const StrAttrs &current,const StrAttrs &apply,StrAttrs &ret);
void ensureAttr(const StrAttrs &att,StrAttrs &A, DString name);
bool shapeChanged(const StrAttrs &oldAttrs,const StrAttrs &newAttrs);
PolyDef readPolyDef(Transform *trans,StrAttrs &attrs);

template<typename Layout>
Update stringsIn(Transform *trans,bool useDotDefaults,Layout *l,const StrAttrs &attrs,bool clearOld) {
	using std::istringstream;
	Update ret;
	StrAttrs allChanges = attrs;
	if(clearOld)
		clearRemoved(gd<StrAttrs>(l),attrs,allChanges);
    StrAttrs &att = gd<StrAttrs>(l);
    ensureAttr(att,allChanges,"resolution");
    ensureAttr(att,allChanges,"separation");
    ensureAttr(att,allChanges,"defaultsize");
	for(StrAttrs::const_iterator ai = allChanges.begin(); ai!=allChanges.end(); ++ai) {
        DString name = ai->first,
            value = ai->second;
		if(name=="resolution") {
			if(value.empty())
                value = useDotDefaults?"1,1":"0.1,0.1";
			istringstream s(value);
			Coord res;
			s >> res;
			if(assign_unclose(gd<GraphGeom>(l).resolution,res))
				ret |= DG_UPD_RESOLUTION;
		}
		else if(name=="separation") {
			if(value.empty())
                value = useDotDefaults?"24,24":"0.5,0.5";
			istringstream s(value);
			Coord sep;
			s >> sep;
			if(assign_unclose(gd<GraphGeom>(l).separation,sep))
				ret |= DG_UPD_SEPARATION;
		}
		else if(name=="edgeseparation") {
			istringstream s(value);
			double esep;
			s >> esep;
			if(assign_unclose(gd<GraphGeom>(l).edgeSeparation,esep))
				ret |= DG_UPD_SEPARATION;
		}
		else if(name=="defaultsize") {
			if(value.empty())
                value = useDotDefaults?"54,36":"1.5,1";
			istringstream s(value);
			s >> gd<GraphGeom>(l).defaultSize;
		}
		else if(name=="ticks") {
			if(value.empty())
                value = "0";
			istringstream s(value);
			s >> gd<GraphGeom>(l).ticks;
		}
		else if(name=="intermediate")
			gd<GraphGeom>(l).reportIntermediate = value=="true";
		else if(name=="emphasizeflow")
			gd<GraphGeom>(l).s_edges = value=="true";
		else if(name=="splinelevel") {
			if(value=="vnode")
				gd<GraphGeom>(l).splineLevel = DG_SPLINELEVEL_VNODE;
			else if(value=="bounds")
				gd<GraphGeom>(l).splineLevel = DG_SPLINELEVEL_BOUNDS;
			else if(value=="shortest")
				gd<GraphGeom>(l).splineLevel = DG_SPLINELEVEL_SHORTEST;
			else if(value=="spline")
				gd<GraphGeom>(l).splineLevel = DG_SPLINELEVEL_SPLINE;
		}
		else if(name=="rankdir") {
			Orientation ori = DG_ORIENT_DOWN;
			if(value=="TB")
				ori = DG_ORIENT_DOWN;
			else if(value=="LR")
				ori = DG_ORIENT_RIGHT;
			else if(value=="BT")
				ori = DG_ORIENT_UP;
			else if(value=="RL")
				ori = DG_ORIENT_LEFT;
			if(assign(gd<Translation>(l).orientation,ori))
				ret |= DG_UPD_TRANSLATION;
		}
		SetAndMark(l,ai->first,value);
	}
	return ret;
}
template<typename Layout>
Update assureAttrs(Transform *trans,typename Layout::Node *n) {
	Update ret;
	StrAttrs &att = gd<StrAttrs>(n);
	DString &shape = att["shape"];
	if(shape.empty()) {
        DString value = (att.find("sides")!=att.end())?"polygon":"ellipse";
        if(SetAndMark(n,"shape",value))
		    ret.flags |= DG_UPD_REGION;
	}
	StrAttrs::iterator ai;
	Coord size = trans->outSize(gd<GraphGeom>(n->g).defaultSize);
    if((ai=att.find("width"))!=att.end() && !ai->second.empty())
		sscanf(ai->second.c_str(),"%lf",&size.x);
	if((ai=att.find("height"))!=att.end() && !ai->second.empty())
		sscanf(ai->second.c_str(),"%lf",&size.y);
	/*
	// old strategy: make a square if only one dimension specified
	if(size.x&&size.y)
		return ret;
	if(size.x||size.y) {
		if(!size.y)
			size.y = size.x;
		if(!size.x)
			size.x = size.y;
	}
	*/
	// new strategy: allow zero size if shape=none or plaintext
	// otherwise use resolution as a minimum
	if(shape!="none" && shape!="plaintext") {
		Coord mini = trans->outSize(gd<GraphGeom>(n->g).resolution);
		size.x = std::max(size.x,mini.x);
		size.y = std::max(size.y,mini.y);
	}
	else {
		if(size.x<0) size.x = .0;
		if(size.y<0) size.y = 0.;
	}
	char buf[20];
	sprintf(buf,"%f",size.x);
	bool ch = SetAndMark(n,"width",buf);
	sprintf(buf,"%f",size.y);
	ch |= SetAndMark(n,"height",buf);
    if(ch)
	    ret.flags |= DG_UPD_REGION;
	return ret;
}
template<typename Layout>
Update stringsIn(Transform *trans,typename Layout::Node *n,const StrAttrs &attrs,bool clearOld) {
	using std::istringstream;
	StrAttrs allChanges;
	if(clearOld)
		clearRemoved(gd<StrAttrs>(n),attrs,allChanges);
	const StrAttrs &A = clearOld?allChanges:attrs;
	Update ret;
	NodeGeom &ng = gd<NodeGeom>(n);
	StrAttrs &att = gd<StrAttrs>(n);
	StrAttrs::const_iterator ai;
	bool chshape = shapeChanged(att,A);
	if(chshape)
		ret.flags |= DG_UPD_POLYDEF;
	for(ai = A.begin(); ai!=A.end(); ++ai) {
		if(ai->first=="pos") {
            if(ai->second.empty())  // position intentionally removed
                ng.pos.valid = false;
            else {
			    ng.pos.valid = true;
			    istringstream stream(ai->second);
			    stream >> ng.pos;
			    ng.pos = trans->in(ng.pos);
            }
			ret.flags |= DG_UPD_MOVE;
		}
		else if(ai->first=="flow")
			ng.flow = atof(ai->second.c_str());
		else if(ai->first=="suppressed") {
			if(assign(ng.suppressed,ai->second=="true"))
				ret.flags |= DG_UPD_POLYDEF|DG_UPD_MOVE|DG_UPD_SUPPRESSION;
		}
		else if(ai->first=="freezetailfanning") 
			ng.freezeTailFanning = ai->second=="true";
		else if(ai->first=="freezeheadfanning") 
			ng.freezeHeadFanning = ai->second=="true";
		else if(ai->first.compare(0,9,"labelsize")==0) {
			int i=ds2int(ai->first.substr(9));
			if(i>=0) {
				istringstream stream(ai->second);
				stream >> gd<NodeLabels>(n)[i].size;
				ret.flags |= DG_UPD_LABEL;
			}
		}
		else if(ai->first.compare(0,14,"labelalign")==0) {
			int i=ds2int(ai->first.substr(14));
			if(i>=0) {
				gd<NodeLabels>(n)[i].align = string2nlp(ai->second);
				ret.flags |= DG_UPD_LABEL;
			}
		}
		else if(ai->first.compare(0,2,"labelbounds")==0) {
			int i=ds2int(ai->first.substr(9));
			if(i>=0) {
				istringstream stream(ai->second);
				stream >> gd<NodeLabels>(n)[i].bounds;
				ret.flags |= DG_UPD_LABEL;
			}
		}
		SetAndMark(n,ai->first,ai->second);
	}
	ret.flags |= assureAttrs<Layout>(trans,n).flags;
	if(ret.flags&(DG_UPD_REGION|DG_UPD_POLYDEF)) {
		if((ai = att.find("boundary"))!=att.end() && !ai->second.empty()) {
			const DString &s = ai->second;
			istringstream stream(s);
			ng.region.shape.Clear();
			stream >> ng.region.shape;
			ng.region.updateBounds();
			gd<Drawn>(n).clear();
			gd<Drawn>(n).push_back(ng.region.shape);
			gd<IfPolyDef>(n).whether = false;
			ret.flags |= DG_UPD_REGION|DG_UPD_DRAWN;
		}
		else if((ai = att.find("shape"))!=att.end() && (ai->second=="none" || ai->second=="plaintext")) {
			Coord size;
			ai = att.find("labelsize");
			if(ai != att.end()) {
				istringstream stream(ai->second);
				stream >> size;
			}
			else {
				// assureAttrs has made sure we have explicit width & height
				size.x = atof(att["width"].c_str());
				size.y = atof(att["height"].c_str());
			}
			size = trans->inSize(size);
			gd<Drawn>(n).clear();
			ng.region.shape.Clear();
			ng.region.boundary = Rect(-size.x/2,size.y/2,size.x/2,-size.y/2);
			gd<IfPolyDef>(n).whether = false;
			ret.flags |= DG_UPD_REGION|DG_UPD_DRAWN;
		}
		else {
			gd<IfPolyDef>(n).whether = true;
			gd<PolyDef>(n) = readPolyDef(trans,att);
			ret.flags |= DG_UPD_POLYDEF;
		}
	}
	return ret;
}
template<typename Layout>
Update stringsIn(Transform *trans,typename Layout::Edge *e,const StrAttrs &attrs,bool clearOld) {
	using std::istringstream;
	StrAttrs allChanges;
	if(clearOld)
		clearRemoved(gd<StrAttrs>(e),attrs,allChanges);
	const StrAttrs &A = clearOld?allChanges:attrs;
	Update ret;
	EdgeGeom &eg = gd<EdgeGeom>(e);
	for(StrAttrs::const_iterator ai = A.begin(); ai!=A.end(); ++ai) {
		bool skip = false;
		if(ai->first=="pos") {
		    const DString &s = ai->second;
            if(s.empty()) {
                eg.pos.Clear();
            }
            else {
			    dgassert(s.length());
			    DString::size_type begin = 0,end=s.size();
                // grr i know i'll get this wrong in some obscure way...
                // just sloppily trying to throw away the [e,x,y ][s,x,y ]
                for(int j = 0; j<2; ++j) {
					if(s[begin]=='e'||s[begin]=='s') {
						++begin;
                        for(int i = 0; i<2; ++i) {
                            dgassert(s[begin]==',');
							++begin;
							// much too expansive: -6.2.-9.4 would be one number...
                            while(s[begin]=='-'||isdigit(s[begin])||s[begin]=='.')
								++begin;
                        }
					}
                    while(s[begin]==' ') ++begin;
                }
			    DString::size_type i = s.find(';',begin);
			    if(i!=DString::npos)
			        end = i;
			    Line newline;
			    istringstream stream(s.substr(begin,end-begin));
			    stream >> newline;
			    if(transformShape(trans,newline)) {
				    eg.pos = newline;
				    ret.flags |= DG_UPD_MOVE;
			    }
			    else
				    skip = true;
            }
		}
		else if(ai->first=="minlen") {
			double len = atof(ai->second.c_str());
			if(len>=.0)
				eg.minLength = len;
			else
				skip = true;
		}
		else if(ai->first=="backward")
			eg.backward = ai->second=="true";
		else if(ai->first=="emphasizeflow")
			eg.s_edge = ai->second=="true";
		if(!skip)
			SetAndMark(e,ai->first,ai->second);
	}
	return ret;
}
template<typename Layout>
void applyStrGraph(Transform *trans,bool useDotDefaults,StrGraph *g,Layout *out, Layout *subg) {
  stringsIn(trans,useDotDefaults,out,gd<StrAttrs>(g),false);
  gd<Name>(out) = gd<Name>(g);
  std::map<DString,typename Layout::Node*> renode;
  for(StrGraph::node_iter ni = g->nodes().begin(); ni!=g->nodes().end(); ++ni) {
    typename Layout::Node *n = out->create_node(gd<Name>(*ni));
    subg->insert(n);
    renode[gd<Name>(*ni)] = n;
    StrAttrs &attrs = gd<StrAttrs>(*ni);
	stringsIn<Layout>(trans,n,attrs,false);
  }
  for(StrGraph::graphedge_iter ei = g->edges().begin(); ei!=g->edges().end(); ++ei) {
    typename Layout::Edge *e = out->create_edge(renode[gd<Name>((*ei)->tail)],
				     renode[gd<Name>((*ei)->head)],gd<Name>(*ei)).first;
    subg->insert(e);
    StrAttrs &attrs = gd<StrAttrs>(*ei);
	stringsIn<Layout>(trans,e,attrs,false);
  }
}

} // namespace Dynagraph

#endif // stringsIn_h
