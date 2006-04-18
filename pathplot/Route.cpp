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


#include "PathPlot.h"
#include "common/Solvers.h"
#include "BezCoeffs.h"

#define PATHDEBUGGING 1

#define EPSILON1 1E-6
#define EPSILON2 1E-6

namespace Dynagraph {
namespace PathPlot {

struct TnA : Segment {
	double t;
	TnA() : Segment(Coord(),Coord()),t(0) {}
};

static bool reallyRouteSpline(const SegmentV &barriers,
        const Coord *inps, size_t inpn, TnA *tnas, Segment endSlopes,Line &out);
static void mkSpline(const Coord *inps, size_t inpn, TnA *tnas, Segment endSlopes,
        Coord *sp0, Coord *sv0, Coord *sp1, Coord *sv1);
static bool splineFits(const SegmentV &barriers,Coord pa, Coord va,
        Coord pb, Coord vb, bool forceflag, Line &out);
static bool splineIsInside(const SegmentV &barriers, Coord sps[4]);

bool Route(const SegmentV &barriers, const Line &input,Segment endSlopes,Line &out) {
#if PATHDEBUGGING >= 1
	unsigned i,j;
	/* validate - input region is not self-intersecting */
	for(i = 0; i < barriers.size() - 1; i++)
		for(j = i + 1; j < barriers.size(); j++) {
			/* tell me this isn't BOGUS */
			if(barriers[i].a==barriers[j].a) continue;
			if(barriers[i].a==barriers[j].b) continue;
			if(barriers[i].b==barriers[j].a) continue;
			if(barriers[i].b==barriers[j].b) continue;
			if(segsIntersect(barriers[i].a,barriers[i].b,barriers[j].a,barriers[j].b)) {
				report(r_error,"input region self-intersection: (%.3lf,%.3lf),(%.3lf,%.3lf) X (%.3lf,%.3lf),(%.3lf,%.3lf)\n",
					barriers[i].a.x,
					barriers[i].a.y,
					barriers[i].b.x,
					barriers[i].b.y,
					barriers[j].a.x,
					barriers[j].a.y,
					barriers[j].b.x,
					barriers[j].b.y);
				return false;
			}
		}
#endif

    /* generate the splines */
    endSlopes.a = endSlopes.a.Norm();
    endSlopes.b = endSlopes.b.Norm();
	out.degree = 3;
	out.push_back(input.front());
	TnA *tnas = new TnA[input.size()];
    if(!reallyRouteSpline(barriers, &input[0],input.size(), tnas, endSlopes,out)) //
        return false;
	delete [] tnas;

#if DEBUGPATH
	fprintf(stderr, "%%!PS-splinomatic\n");
	fprintf(stderr, "%% edges\n");
	fprintf(stderr,"0 0 1 setrgbcolor\n");
	for(int ipi = 0; ipi < barriers.size(); ipi++)
			fprintf(stderr, "newpath %f %f moveto %f %f lineto stroke\n",
	edges[ipi].a.x,edges[ipi].a.y,
	edges[ipi].b.x,edges[ipi].b.y);

	fprintf(stderr, "%% input path\n");
	fprintf(stderr,"1 0 0 setrgbcolor\n");
	fprintf(stderr,"newpath");
		for(ipi = 0; ipi < input.size(); ipi++) {
			fprintf(stderr, " %f %f", inps[ipi].x, inps[ipi].y);
	if(ipi == 0) fprintf(stderr," moveto");
	else fprintf(stderr," lineto");
	}
	fprintf(stderr," stroke\n");
	fprintf(stderr, "%% output spline\n");
	fprintf(stderr,"1 .9 .8 setrgbcolor\n");
	fprintf(stderr,"newpath");
		for(int opi = 0; opi < opl; opi++) {
			fprintf(stderr, " %f %f", ops[opi].x, ops[opi].y);
	if(opi == 0) fprintf(stderr," moveto");
	else if(opi % 3 == 0) fprintf(stderr," curveto");
	}
	fprintf(stderr," stroke\n");
#endif

    return true;
}

static bool reallyRouteSpline(const SegmentV &barriers,
        const Coord *inps, size_t inpn, TnA *tnas, Segment endSlopes,Line &out) {
    tnas[0].t = 0;
    size_t i;
    for(i = 1; i < inpn; i++)
        tnas[i].t = tnas[i - 1].t + dist(inps[i], inps[i - 1]);
    for(i = 1; i < inpn; i++)
        tnas[i].t /= tnas[inpn - 1].t;
    for(i = 0; i < inpn; i++) {
        tnas[i].a = endSlopes.a*B1(tnas[i].t);
        tnas[i].b = endSlopes.b*B2(tnas[i].t);
    }

	Coord p1,v1,p2,v2;
    mkSpline(inps, inpn, tnas, endSlopes, &p1, &v1, &p2, &v2);

    if(splineFits(barriers, p1, v1, p2, v2,inpn == 2,out))
        return true;

    Coord cp1 = p1 + v1/3.0,
		cp2 = p2 - v2/3.0;
	double maxd=-1;
	int maxi=-1;
    for(i = 1; i < inpn - 1; i++) {
        double t = tnas[i].t;
		Coord p = p1*B0(t) + cp1*B1(t) + cp2*B2(t) + p2*B3(t);
		double d = dist(p, inps[i]);
        if(d > maxd)
            maxd = d, maxi = (int)i;
    }
    int spliti = maxi;
    Coord splitv1 = (inps[spliti]-inps[spliti - 1]).Norm(),
		splitv2 = (inps[spliti + 1]-inps[spliti]).Norm(),
		splitv = (splitv1+splitv2).Norm();

    if(!reallyRouteSpline(barriers,inps,spliti + 1,tnas,Segment(endSlopes.a,splitv),out) ||
		!reallyRouteSpline(barriers,&inps[spliti],inpn - spliti,tnas,Segment(splitv,endSlopes.b),out))
		return false;
    return true;
}

static void mkSpline(const Coord *inps, size_t inpn, TnA *tnas, Segment endSlopes,
        Coord *sp0, Coord *sv0, Coord *sp1, Coord *sv1) {
	const Coord &front = *inps,&back = inps[inpn-1];
	double c[2][2] = {{0,0},{0,0}},
		x[2] = {0,0};
    for(size_t i = 0; i < inpn; i++) {
        c[0][0] += tnas[i].a%tnas[i].a;
        c[0][1] += tnas[i].a%tnas[i].b;
        c[1][0] = c[0][1];
        c[1][1] += tnas[i].b%tnas[i].b;
        Coord tmp = inps[i]-(front*B01(tnas[i].t)+
			back*B23(tnas[i].t));
        x[0] += tnas[i].a%tmp;
        x[1] += tnas[i].b%tmp;
    }
    double det01 = c[0][0] * c[1][1] - c[1][0] * c[0][1],
		det0X = c[0][0] * x[1] - c[0][1] * x[0],
		detX1 = x[0] * c[1][1] - x[1] * c[0][1];
    double scale0 = 0.0,
		scale3 = 0.0;
    if(det01) {
        scale0 = detX1 / det01;
        scale3 = det0X / det01;
    }
    if(absol(det01) < EPSILON1 || scale0 <= 0.0 || scale3 <= 0.0) {
        double d01 = dist(front, back) / 3.0;
        scale0 = d01;
        scale3 = d01;
    }
    *sp0 = front;
    *sv0 = endSlopes.a*scale0;
    *sp1 = back;
    *sv1 = endSlopes.b*scale3;
}

static bool splineFits(const SegmentV &barriers,Coord pa, Coord va,
        Coord pb, Coord vb, bool forceflag, Line &out) {
    double a, b;

#if 0
    double d = dist(pa,pb);
    a = d, b = d;
#else
    a = b = 4;
#endif
    while(1) {
		Coord sps[4] = {pa,
			pa + va*a/3.0,
			pb - vb*b/3.0,
			pb};
        if(splineIsInside(barriers, sps)) {
			out.AddSeg(sps);
			report(r_splineRoute,"success: %f %f\n", a, b);
            return true;
        }
        if(a == 0 && b == 0) {
            if(forceflag) {
				out.AddSeg(sps);
				report(r_splineRoute,"forced straight line: %f %f\n", a, b);
                return true;
            }
            break;
        }
        if(a > .01)
            a /= 2, b /= 2;
        else
            a = b = 0;
    }
#if DEBUG >= 1
fprintf(stderr, "failure\n");
#endif
    return false;
}

static bool splineIsInside(const SegmentV &barriers, Coord sps[4]) {
	double roots[4];
    for(unsigned ei = 0; ei < barriers.size(); ei++) {
		int rootn = splineIntersectsLine(sps, barriers[ei], roots);
        if(rootn == 4)
            continue; // return 1;
        for(int rooti = 0; rooti < rootn; rooti++) {
            if(roots[rooti] < EPSILON2 || roots[rooti] > 1 - EPSILON2)
                continue;
            double t = roots[rooti],
				td = B3(t),
				tc = B2(t),
				tb = B1(t),
				ta = B0(t);
			Coord ip(ta * sps[0].x + tb * sps[1].x +
                    tc * sps[2].x + td * sps[3].x,
				ta * sps[0].y + tb * sps[1].y +
                    tc * sps[2].y + td * sps[3].y);
            if(dSquared(ip, barriers[ei].a) < EPSILON1 ||
                    dSquared(ip, barriers[ei].b) < EPSILON1)
                continue;
            return false;
        }
    }
    return true;
}

} // namespace PathPlot
} // namespace Dynagraph
