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


#include "common/Geometry.h"

namespace Dynagraph {
namespace PathPlot {

typedef std::vector<Segment> SegmentV;
typedef std::vector<Line> LineV;

struct InvalidBoundary : DGException {
  InvalidBoundary() : DGException("input to shortest path algorithm is not a polygon boundary",true) {}
};
struct EndpointNotInPolygon : DGException {
  bool which;
  EndpointNotInPolygon(bool which) :
    DGException("end-points in shortest path algorithm input are not within the boundary polygon",true),
    which(which) {}
};
/* find shortest euclidean path within a simple polygon */
void Shortest(const Line &boundary, Segment endpoints, Line &out);

/* fit a spline to an input polyline, without touching barrier segments */
bool Route(const SegmentV &barriers, const Line &inputRoute,
    Segment endSlopes,Line &out);

/* utility function to convert from a set of polygonal obstacles to barriers */
void PolyBarriers(const LineV &polygons, SegmentV &out);

} // namespace PathPlot
} // namespace Dynagraph
