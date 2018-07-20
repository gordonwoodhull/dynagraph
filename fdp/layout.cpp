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


#include "fdp.h"
#include <math.h>
#include "macros.h"

namespace Dynagraph {
namespace FDP {

/*cool:
 *Reduce temperature
 */
double FDPServer::cool(int t) {
  return (T0*(numIters-t))/numIters;
}
inline double localK2(FDPLayout::Node *a,FDPLayout::Node *b) {
    Coord tot(0,0);
    int n=0;
    if(gd<NodeGeom>(a).region.boundary.valid)
        tot+=gd<NodeGeom>(a).region.boundary.Size(),++n;
    if(gd<NodeGeom>(b).region.boundary.valid)
        tot+=gd<NodeGeom>(b).region.boundary.Size(),++n;
    if(n)
        tot /= n;
    tot += gd<GraphGeom>(a->g).separation/2;
    return tot%tot;
}
inline double localK(FDPLayout::Node *a,FDPLayout::Node *b) {
    return sqrt(localK2(a,b));
}

void FDPServer::doRep(FDPModel::Node *p, FDPModel::Node *q, double xdelta, double ydelta, double dist2) {
    double force;

    while(dist2 == 0.0) {
      xdelta = 5 - rand()%10;
      ydelta = 5 - rand()%10;
      dist2 = xdelta*xdelta + ydelta*ydelta;
    }
    force = (RepFactor * localK2(gd<FDPNode>(p).layoutN,gd<FDPNode>(q).layoutN))/dist2;
    gd<FDPNode>(q).disp[0] += xdelta *force;
    gd<FDPNode>(q).disp[1] += ydelta *force;
    gd<FDPNode>(p).disp[0] -= xdelta *force;
    gd<FDPNode>(p).disp[1] -= ydelta *force;
}

/*applyRep:
 *Repulsive force =(K*K)/d
 */
void FDPServer::applyRep(FDPModel::Node *p, FDPModel::Node *q) {
    double xdelta, ydelta;

    xdelta = gd<FDPNode>(q).pos[0] - gd<FDPNode>(p).pos[0];
    ydelta = gd<FDPNode>(q).pos[1] - gd<FDPNode>(p).pos[1];
    doRep(p, q, xdelta, ydelta, xdelta*xdelta + ydelta*ydelta);
}

/*applyAttr:
 *Attractive force =(d*d)/K
 */
void FDPServer::applyAttr(FDPModel::Node *p, FDPModel::Node *q) {
    double xdelta, ydelta;
    double force;
    double dist;
    double dist2;

    xdelta = gd<FDPNode>(q).pos[0] - gd<FDPNode>(p).pos[0];
    ydelta = gd<FDPNode>(q).pos[1] - gd<FDPNode>(p).pos[1];
    dist2 = xdelta*xdelta + ydelta*ydelta;
    dist = sqrt(dist2);
    force = (AttFactor *dist)/localK(gd<FDPNode>(p).layoutN,gd<FDPNode>(q).layoutN);
    gd<FDPNode>(q).disp[0] -= xdelta * force;
    gd<FDPNode>(q).disp[1] -= ydelta * force;
    gd<FDPNode>(p).disp[0] += xdelta * force;
    gd<FDPNode>(p).disp[1] += ydelta * force;
}

void FDPServer::doNeighbor(Grid*grid, int i, int j, const node_list &nodes) {
    Cell *cell = grid->find(i, j);
    double xdelta, ydelta,
        dist2;

    if(cell) {
        /*
        if(Verbose >= 3)
            reports[dgr::error] <<  "  doNeighbor(" << i << ',' << j << ") : " << gLength(cell) << endl;
            */
        for(node_list::const_iterator pi = nodes.begin(); pi!=nodes.end(); ++pi) {
            FDPModel::Node *p = *pi;
            for(node_list::const_iterator qi = cell->nodes.begin(); qi!=cell->nodes.end(); ++qi) {
                FDPModel::Node *q = *qi;
                xdelta = gd<FDPNode>(q).pos[0] - gd<FDPNode>(p).pos[0];
                ydelta = gd<FDPNode>(q).pos[1] - gd<FDPNode>(p).pos[1];
                dist2 = xdelta*xdelta + ydelta*ydelta;
                if(dist2 < Radius2)
                    doRep(p, q, xdelta, ydelta, dist2);
            }
        }
    }
}


int FDPServer::VisitCell(Cell *cell,Grid *grid) {
    int i = cell->p.i;
    int j = cell->p.j;

    /*
    if(Verbose >= 3)
        reports[dgr::error] <<  "gridRepulse(" << i << ',' << j << ") : " << gLength(cell) << endl;
    */
    node_list &nodes = cell->nodes;
    for(node_list::iterator pi = nodes.begin(); pi!=nodes.end(); ++pi)
        for(node_list::iterator qi = nodes.begin(); qi!=nodes.end(); ++qi)
            if(pi!=qi)
                applyRep(*pi,*qi);

    doNeighbor(grid, i - 1, j - 1, nodes);
    doNeighbor(grid, i - 1, j    , nodes);
    doNeighbor(grid, i - 1, j + 1, nodes);
    doNeighbor(grid, i    , j - 1, nodes);
    doNeighbor(grid, i    , j + 1, nodes);
    doNeighbor(grid, i + 1, j - 1, nodes);
    doNeighbor(grid, i + 1, j    , nodes);
    doNeighbor(grid, i + 1, j + 1, nodes);

    return 0;
}


void FDPServer::adjust(FDPModel *g, double temp, Grid *grid) {
    double disp[NDIM];          /*incremental displacement */

    if(temp <= 0.0)
        return;

    if(useGrid)
        grid->clear();

    FDPModel::node_iter ni;
    for(ni = g->nodes().begin(); ni!=g->nodes().end(); ++ni) {
        disp[1] = gd<FDPNode>(*ni).disp[0];
        gd<FDPNode>(*ni).disp[0] = gd<FDPNode>(*ni).disp[1] = 0;
        if(useGrid)
            grid->add(int(floor(gd<FDPNode>(*ni).pos[0]/CellSize)),
                  int(floor(gd<FDPNode>(*ni).pos[1]/CellSize)), *ni);
    }

    if(useGrid) {
        for(FDPModel::graphedge_iter ei = g->edges().begin(); ei!=g->edges().end(); ++ei)
            applyAttr((*ei)->tail, (*ei)->head);
        grid->walk(this);
    }
    else {
        for(FDPModel::node_iter ni = g->nodes().begin(); ni!=g->nodes().end(); ++ni) {
            FDPModel::node_iter ni2;
            for((ni2 = ni)++; ni2!=g->nodes().end(); ++ni2)
                applyRep(*ni, *ni2);
            for(FDPModel::outedge_iter ei = (*ni)->outs().begin(); ei!=(*ni)->outs().end(); ++ei)
                applyAttr(*ni, (*ei)->head);
        }
    }

    double temp2 = temp * temp;
    for(ni = g->nodes().begin(); ni!=g->nodes().end(); ++ni) {
        FDPModel::Node *n = *ni;
        if(gd<FDPNode>(n).fixed) continue;
        disp[0] = gd<FDPNode>(n).disp[0];
        disp[1] = gd<FDPNode>(n).disp[1];
        double len2 = disp[0]*disp[0] + disp[1]*disp[1];

        if(len2 < temp2) {
            gd<FDPNode>(n).pos[0] += disp[0];
            gd<FDPNode>(n).pos[1] += disp[1];
        }
        else {
            /*to avoid sqrt, consider abs(x) + abs(y) */
            double len = sqrt(len2);
            gd<FDPNode>(n).pos[0] +=(disp[0] *temp)/len;
            gd<FDPNode>(n).pos[1] +=(disp[1] *temp)/len;
        }

        /*Keep withing box */
        /*
        if(gd<FDPNode>(n).pos[0] < 0) gd<FDPNode>(n).pos[0] = 0;
        else if(gd<FDPNode>(n).pos[0] > Width) gd<FDPNode>(n).pos[0] = Width;
        if(gd<FDPNode>(n).pos[1] < 0) gd<FDPNode>(n).pos[1] = 0;
        else if(gd<FDPNode>(n).pos[1] > Height) gd<FDPNode>(n).pos[1] = Height;
        */
    }

}
/*positionNodes:
 */
void FDPServer::positionNodes(FDPModel *g, Grid *grid) {
  int i;

  for(i = 0; i < numIters; i++)
    adjust(g, cool(i), grid);
}

/*layout:
 *layout graph
 *If useComp flag set, layout connected components
 *separately. Otherwise, layout the entire graph.
 */
void FDPServer::layout() {
#ifdef JJ
  int i;
#endif
  Grid *grid=0;

  if(useGrid)
      grid = new Grid(model.nodes().size());
  if(useComp) {
#ifdef JJ
    makeConnectedComp(g);
    for(i = 0; i < gd<FDPNode>(g).n_cc; i++) {
      positionNodes(gd<FDPNode>(g).cc+i, grid);
    }
    adjustCC(g);
#endif
  }
  else {
    positionNodes(&model, grid);
    //adjustNodes(g);
/*
    if(adjustNodes(g)) {
      AttFactor = Afact2;
      RepFactor = Rfact2;
      positionNodes(g, &info);
    }
*/
  }
  if(useGrid)
      delete grid;

}

} // namespace FDP
} // namespace Dynagraph
