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

#include "Geometry.h"
#include "Solvers.h"

namespace Dynagraph {

#define EPSILON1 1E-6
#define EPSILON2 1E-6


inline void points2coeff(double v0, double v1, double v2, double v3,double coeff[4]) {
    coeff[3] = v3 + 3 * v1 - (v0 + 3 * v2);
    coeff[2] = 3 * v0 + 3 * v2 - 6 * v1;
    coeff[1] = 3 * (v1 - v0);
    coeff[0] = v0;
}
inline int addroot(double root, double roots[4], int rootn)  {
    if(root >= 0 && root <= 1)
        roots[rootn++] = root;
	return rootn;
}
int splineIntersectsLine(const Coord *sps, Segment seg,double *roots) {
    double scoeff[4], xcoeff[2], ycoeff[2];
    double xroots[3], yroots[3], tv, sv, rat;
    int rootn, xrootn, yrootn, i, j;

    xcoeff[0] = seg.a.x;
    xcoeff[1] = seg.b.x - seg.a.x;
    ycoeff[0] = seg.a.y;
    ycoeff[1] = seg.b.y - seg.a.y;
    rootn = 0;
    if(fabs(xcoeff[1])<EPSILON1) {
        if(fabs(ycoeff[1])<EPSILON1) {
            points2coeff(sps[0].x, sps[1].x, sps[2].x, sps[3].x, scoeff);
            scoeff[0] -= xcoeff[0];
            xrootn = solve3(scoeff, xroots);
            points2coeff(sps[0].y, sps[1].y, sps[2].y, sps[3].y, scoeff);
            scoeff[0] -= ycoeff[0];
            yrootn = solve3(scoeff, yroots);
            if(xrootn == 4)
                if(yrootn == 4)
                    return 4;
                else
                    for(j = 0; j < yrootn; j++)
                        rootn = addroot(yroots[j], roots, rootn);
            else if(yrootn == 4)
                for(i = 0; i < xrootn; i++)
                    rootn = addroot(xroots[i], roots, rootn);
            else
                for(i = 0; i < xrootn; i++)
                    for(j = 0; j < yrootn; j++)
                        if(xroots[i] == yroots[j])
                            rootn = addroot(xroots[i], roots, rootn);
            return rootn;
        } else {
            points2coeff(sps[0].x, sps[1].x, sps[2].x, sps[3].x, scoeff);
            scoeff[0] -= xcoeff[0];
            xrootn = solve3(scoeff, xroots);
            if(xrootn == 4)
                return 4;
            for(i = 0; i < xrootn; i++) {
                tv = xroots[i];
                if(tv >= 0 && tv <= 1) {
                    points2coeff(sps[0].y, sps[1].y, sps[2].y, sps[3].y,
                            scoeff);
                    sv = scoeff[0] + tv *(scoeff[1] + tv *
                        (scoeff[2] + tv * scoeff[3]));
                    sv = (sv - ycoeff[0]) / ycoeff[1];
                    if((0 <= sv) && (sv <= 1))
                        rootn = addroot(tv, roots, rootn);
                }
            }
            return rootn;
        }
    } else {
        rat = ycoeff[1] / xcoeff[1];
        points2coeff(sps[0].y - rat * sps[0].x, sps[1].y - rat * sps[1].x,
                sps[2].y - rat * sps[2].x, sps[3].y - rat * sps[3].x, scoeff);
        scoeff[0] += rat * xcoeff[0] - ycoeff[0];
        xrootn = solve3(scoeff, xroots);
        if(xrootn == 4)
            return 4;
        for(i = 0; i < xrootn; i++) {
            tv = xroots[i];
            if(tv >= 0 && tv <= 1) {
                points2coeff(sps[0].x, sps[1].x, sps[2].x, sps[3].x, scoeff);
                sv = scoeff[0] + tv * (scoeff[1] + tv *
                    (scoeff[2] + tv * scoeff[3]));
                sv = (sv - xcoeff[0]) / xcoeff[1];
                if((0 <= sv) && (sv <= 1))
                    rootn = addroot(tv, roots, rootn);
            }
        }
        return rootn;
    }
}

} // namespace Dynagraph
