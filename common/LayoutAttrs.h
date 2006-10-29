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


#ifndef LayoutAttrs_h
#define LayoutAttrs_h

#include "LGraph-cdt.h"
#include "StrAttr.h"
#include "traversal.h"
#include "Geometry.h"
#include "genpoly.h"
#include "Interruptible.h"

// wrong
#include "dynadag/NSRankerAttrs.h"
#include "dynadag/ExtraRanks.h"
#include "dynadag/EdgeSuppressionAttrs.h"

namespace Dynagraph {

/*
    UPDATE flags. use with ChangeQueue::ModNode,ModEdge to set this
	modify subgraph specific flag.
	(this update flag is a poor use for instance-specific data, because the data
	is unused in the other subgraphs)
*/
typedef enum UpdateFlags_ {
	DG_UPD_MOVE = 1<<0, // NodeGeom:: or EdgeGeom:: pos attribute
	DG_UPD_REGION = 1<<1, // NodeGeom::region
	DG_UPD_NAIL = 1<<2, // NodeGeom::nail
	DG_UPD_TAIL = 1<<3, // EdgeGeom::tailPort or ::tailClipped
	DG_UPD_HEAD = 1<<4, // (sorry - can't change end nodes! create a new edge and delete this one)
	DG_UPD_LENGTH = 1<<6, // EdgeGeom::minLength,fromBottom,toTop
	DG_UPD_CONSTRAINT = 1<<8, // EdgeGeom::
	DG_UPD_LABEL = 1<<9, // all Geom:: any label changed (hmm this update flag model is breaking down.  should i reserve 5 bits for 32 labels har har)
	DG_UPD_BOUNDS = 1<<10, // GraphGeom::
	DG_UPD_NESTING = 1<<11, // dream on
	DG_UPD_EDGESTYLE = 1<<12, // GraphGeom::splineLevel (useful for debugging.  real edge styles also should be on the way...)
	DG_UPD_RESOLUTION = 1<<13, // GraphGeom::
	DG_UPD_SEPARATION = 1<<14, // GraphGeom::
	DG_UPD_DRAWN = 1<<15, // nodes,graphs,edges: the Drawn lines have changed
	DG_UPD_POLYDEF = 1<<16, // anything in node's PolyDef
	DG_UPD_CHANGERECT = 1<<17, // GraphGeom::changerect
	DG_UPD_SUPPRESSION = 1<<18, // NodeGeom::suppressed, DynaDAG::Suppression
	DG_UPD_TRANSLATION = 1<<19, // layout's Translation
	DG_UPD_FLOWTANGENT = 1<<20 // NodeGeom::
} UpdateFlags;
struct Update { // subgraph-specific datum
	unsigned flags;
	Update(unsigned flags = 0) : flags(flags) {}
	Update operator |(const Update &other) {
		return Update(flags|other.flags);
	}
	Update operator &(const Update &other) {
		return Update(flags&other.flags);
	}
	Update operator |=(const Update &other) {
		return *this = *this|other;
	}
	Update operator &=(const Update &other) {
		return *this = *this&other;
	}
};

/*
	MODEL pointer - annoying type unsafe pocket in each layout object for
	a layout server to put a back-pointer; there's no provision for additional servers

	Really, the layout servers should be templated on the layout graph type, and
	the GraphAttrs, NodeAttrs, EdgeAttrs shouldn't be defined here but in client code.

	But I haven't seen a great need yet, and don't know what compiler-specific
	tricks you'd have to pull to still have separate compilation.
*/
struct ModelPointer {
	void *model;
	ModelPointer() : model(0) {}
};

// the lines to render an object. applies to all nodes, edges, and the Layout itself (right now just for debugging)
// position-relative for nodes, absolute for others
struct Drawn : Lines {};

/*
	GRAPH attributes
*/
typedef enum SpliningLevel_ {
	DG_SPLINELEVEL_VNODE,
	DG_SPLINELEVEL_BOUNDS,
	DG_SPLINELEVEL_SHORTEST,
	DG_SPLINELEVEL_SPLINE
} SpliningLevel;
struct GraphGeom {
	Bounds bounds, // of all shown
		changerect; // what area changed last FindChangedRect::Process
	SpliningLevel splineLevel;
	Coord labelGap, // between node and label
		resolution, // smallest units in each dimension (e.g. 1,1 for integer)
		separation, // DynaDAG: x: space between nodes and edges; y: multiplier for edge minLength
		defaultSize; // node size to use if neither width nor height specified
	double edgeSeparation;
	float ticks; // time limit, in seconds, 0 - no limit (NYI)
	bool reportIntermediate, // report intermediate (crude) layouts
		s_edges; // draw all reversed edges as Ss
	GraphGeom() : splineLevel(DG_SPLINELEVEL_SPLINE),labelGap(0,0),resolution(0.1,0.1),
		separation(0.5,0.5),defaultSize(1.5,1),edgeSeparation(-1.),ticks(0),reportIntermediate(false),s_edges(false) {}
};
struct StaticLabel {
	Rect bounds;
};
struct StaticLabels {
	std::vector<StaticLabel> labels;
};
// used by CoordTranslator; intention is to design this so that full matrix-based translation
// could be supported in future
typedef enum Orientation_ {
	DG_ORIENT_RIGHT,
	DG_ORIENT_UP,
	DG_ORIENT_LEFT,
	DG_ORIENT_DOWN
} Orientation;
struct Translation {
	Orientation orientation;
	Translation() : orientation(DG_ORIENT_DOWN) {}
};

// generated shapes for nodes (relative to NodeGeom::pos);
// this (unlike NodeGeom::region), is not translated according to Translation::orientation
// in other words, translate dynagraph coords first, then add these.
// (nyi) line ([0]) + arrows for edges (absolute canvas coords)

/*
	NODE attributes
*/
typedef enum NailType_ {
	DG_NONAIL = 0,
	DG_NAIL_X = 1,
	DG_NAIL_Y = 2,
	DG_NAIL_BOTH = 3
} NailType;
struct NodeGeom {
	Position pos;
	Region region;
	NailType nail;
	double flow;
	Coord flowTan; // for flow nodes, the slope at which edges should enter/exit
	bool suppressed,
		freezeTailFanning,
		freezeHeadFanning;
	NodeGeom() : nail(DG_NONAIL),flow(0.0),flowTan(.0,0.),suppressed(false),freezeTailFanning(false),freezeHeadFanning(false) {}
	Bounds BoundingBox() {
		if(!pos.valid)
			return Bounds();
		if(region.boundary.valid)
			return Bounds(region.boundary+pos);
		else
			return Bounds(pos);
	}
	bool Overlaps(const NodeGeom &o) {
		return region.Overlaps(pos,o.pos,o.region);
	}
};
typedef enum NodeLabelAlignment_ {
	DG_NODELABEL_CENTER,
	DG_NODELABEL_RIGHT,
	DG_NODELABEL_TOP,
	DG_NODELABEL_LEFT,
	DG_NODELABEL_BOTTOM,
	DG_NODELABEL_BETTER
} NodeLabelAlignment;
struct NodeLabel {
	NodeLabelAlignment align;
	// if DG_NODELABEL_BETTER:
	// place labels by matching one or two reference points on the node
	// and the and the label.  for example (0.5,0.5),(0.5,0.5) centers
	// (0,0),(0,1) puts the label above, aligned at the left
	Coord nodeAlign,labelAlign, // 0 top/left - 1 bottom/right
		nodeOffset,labelOffset; // in respective plain coordinates
	// labels can be stretched by matching a second reference point
	bool scaleX,scaleY; // whether to use the x or y of the other refpoint
	Coord nodeAlign2,labelAlign2,
		nodeOffset2,labelOffset2;

	Coord size;
	// output
	Bounds bounds;

	NodeLabel(NodeLabelAlignment align = DG_NODELABEL_CENTER, Coord size = Coord(0,0)) :
		align(align), nodeAlign(0,0),labelAlign(0,0),nodeOffset(0,0),labelOffset(0,0),
		scaleX(false),scaleY(false),nodeAlign2(0,0),labelAlign2(0,0),nodeOffset2(0,0),labelOffset2(0,0),
		size(size) {}
};
typedef std::vector<NodeLabel> NodeLabels;
struct Port {
	Coord pos;
	double angle;
	Port(Coord pos=Coord(),double angle=0) : pos(pos),angle(angle) {}
};
typedef std::vector<std::pair<int,Port> > IdentifiedPorts;
struct IfPolyDef : PolyDef {
	bool whether; // whether to use, else client has other way to fill NodeGeom::region
	IfPolyDef() : whether(true) {}
};

/*
	EDGE attributes
*/
struct EdgeGeom {
	Line pos;
	Port tailPort,headPort;
	bool tailClipped,headClipped;
	// DynaDAG-specific attributes (consider moving into DynaDAG now that we have typed graphs)
	double minLength; // minimum Y-spanning of edge (multiplied by GraphGeom::separation::y)
	bool fromBottom, // how to measure length
		toTop; // default both true: measure from bottom of tail to top of head
	bool constraint, // whether this edge affects ranking; set false by DynaDAG if last in cycle or if a node is nailed
		manualRoute, // try to use the line specified in pos
		backward, // do rank constaints backward
		s_edge; // draw as S if reversed (drawn against flow)
	EdgeGeom() : tailPort(Coord(0.0,0.0)),headPort(Coord(0.0,0.0)),tailClipped(true),
		headClipped(true),minLength(1.0),fromBottom(true),toTop(true),constraint(true),
		manualRoute(false),backward(false),s_edge(false) {}
};
struct EdgeLabel {
	// input
	double where, // 0 at tail, 1 at head
		length; // length of straight line to fit here
	Line *shape; // if !=0, translate this shape each time into gd<Drawn>
	// if shape==0, output, otherwise these are points in the shape which should be matched to what we find.
	Position pos1,pos2;
	EdgeLabel(double where = 0.5f,double length = 0.0f) : where(where),length(length),shape(0) {}
};
typedef std::vector<EdgeLabel> EdgeLabels;

// These are the basic layout description attributes
struct GraphAttrs : Name,Hit,Drawn,GraphGeom,Translation,StaticLabels,Interruptible,DynaDAG::ExtraRanks {
	GraphAttrs(Name name) : Name(name) {}
};
struct NodeAttrs : Name,Hit,Drawn,NodeGeom,NodeLabels,IfPolyDef, DynaDAG::NSRankerNode {
	NodeAttrs(Name name) : Name(name) {}
};
struct EdgeAttrs : Name,Hit,Drawn,EdgeGeom,EdgeLabels, DynaDAG::NSRankerEdge,DynaDAG::Suppression {
	EdgeAttrs(Name name) : Name(name) {}
};

// This adds the ModelPointer which is needed if the graph will be used directly with an engine
struct EngineGraphAttrs : GraphAttrs,ModelPointer {
	EngineGraphAttrs(Name name = Name()) : GraphAttrs(name) {}
};
struct EngineNodeAttrs : NodeAttrs,ModelPointer {
	EngineNodeAttrs(Name name) : NodeAttrs(name) {}
};
struct EngineEdgeAttrs : EdgeAttrs,ModelPointer {
	EngineEdgeAttrs(Name name) : EdgeAttrs(name) {}
};

// This adds string attributes, which are needed for the incrface ("for humans")
struct GeneralGraphAttrs : GraphAttrs,StrAttrs {
	GeneralGraphAttrs(Name name = Name()) : GraphAttrs(name) {}
};
struct GeneralNodeAttrs : NodeAttrs,StrAttrs {
	GeneralNodeAttrs(Name name) : NodeAttrs(name) {}
};
struct GeneralEdgeAttrs : EdgeAttrs,StrAttrs {
	GeneralEdgeAttrs(Name name) : EdgeAttrs(name) {}
};

// This combines all of the above so that the graph can be used for both incrface and with engines
struct EverythingGraphAttrs : GraphAttrs,ModelPointer,StrAttrs {
	EverythingGraphAttrs(Name name = Name()) : GraphAttrs(name) {}
};
struct EverythingNodeAttrs : NodeAttrs,ModelPointer,StrAttrs {
	EverythingNodeAttrs(Name name) : NodeAttrs(name) {}
};
struct EverythingEdgeAttrs : EdgeAttrs,ModelPointer,StrAttrs {
	EverythingEdgeAttrs(Name name) : EdgeAttrs(name) {}
};

struct LayoutUpdates : StrAttrChanges,Update {};

} // namespace Dynagraph

#endif //LayoutAttrs_h
