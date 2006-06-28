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

#ifndef ShapeGenerator_h
#define ShapeGenerator_h

#include "reorient.h"

namespace Dynagraph {

template<typename Layout>
struct ShapeGenerator : LinkedChangeProcessor<Layout> {
	ShapeGenerator(ChangingGraph<Layout> *world) : LinkedChangeProcessor<Layout>(world) {}
	void Process() {
		Layout *subs[2] = {&this->world_->Q_.insN,&this->world_->Q_.modN};
		for(int i=0; i<2; ++i)
			for(typename Layout::node_iter ni = subs[i]->nodes().begin(); ni !=subs[i]->nodes().end(); ++ni) {
				typename Layout::Node *n = *ni;
				if((i==0 || igd<Update>(n).flags&DG_UPD_POLYDEF) && gd<IfPolyDef>(n).whether) {
					NodeGeom &ng = gd<NodeGeom>(n);
					gd<Drawn>(n).clear();
					try {
						if(!ng.suppressed)
							genpoly(gd<PolyDef>(n),gd<Drawn>(n));
					}
					catch(GenPolyXep xep) {
						std::cout << "message \"" << xep.exceptype << '"' << std::endl;
						// bad or incomplete definitions: just leave blank
					}
					ng.region.shape.Clear();
					if(gd<Drawn>(n).size()) {
						// it would be nice to make this section an update for a DG_UPD_DRAWN
						// flag, so that the user could specify gd<Drawn> instead of this shapegen...
						Line &biggest = gd<Drawn>(n).front(); // first one is biggest
						ng.region.shape.resize(biggest.size());
						for(size_t i = 0; i<biggest.size(); ++i)
							ng.region.shape[i] = reorient(biggest[i],true,gd<Translation>(&this->world_->current_).orientation);
						ng.region.shape.degree = biggest.degree;
					}
					ng.region.updateBounds();
					ModifyNode(this->world_->Q_,n,DG_UPD_REGION|DG_UPD_DRAWN);
				}
			}
		this->NextProcess();
	}
};

} // namespace Dynagraph

#endif //ShapeGenerator_h
