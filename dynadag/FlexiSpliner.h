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

#ifndef FlexiSpliner_h
#define FlexiSpliner_h

namespace Dynagraph {
namespace DynaDAG {

struct FlexiSpliner {
    FlexiSpliner(Config &config) : config(config) {}
    bool MakeEdgeSpline(DDPath *path,SpliningLevel splineLevel);//,ObstacleAvoiderSpliner<DynaDAGLayout> &obav);
private:
    Config &config;
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // FlexiSpliner_h
