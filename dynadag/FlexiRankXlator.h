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

#ifndef FlexiRankXlator_h
#define FlexiRankXlator_h

namespace Dynagraph {
namespace DynaDAG {

template<typename Layout>
struct FlexiRankXlator {
    typedef int index;
    static bool Above(Layout *l,index a,index b) {
        return a<b;
    }
    static bool Below(Layout *l,index a,index b) {
        return a>b;
    }
    static index CoordToRank(Layout *l,double y) {
#ifndef DOWN_GREATER
        return -ROUND(y/gd<GraphGeom>(l).resolution.y);
#else
        return ROUND(y/gd<GraphGeom>(l).resolution.y);
#endif
    }
    static double RankToCoord(Layout *l,index r) {
#ifndef DOWN_GREATER
        return -r*gd<GraphGeom>(l).resolution.y;
#else
        return r*gd<GraphGeom>(l).resolution.y;
#endif
    }
    static index HeightToDRank(Layout *l,double dy) {
        return ROUND(dy/gd<GraphGeom>(l).resolution.y);
    }
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // FlexiRankXlator_h
