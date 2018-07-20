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

using namespace std;

namespace Dynagraph {

int Line::GetSeg(double y) {
    for(unsigned i = 0; i < size() - 1; i += degree)
        if(sequence(double(at(i).y),y,double(at(i+degree).y)) ||
                sequence(double(at(i+degree).y),y,double(at(i).y)))
            return i;
    return -1;
}
Position Line::YIntersection(double y) {
    if(!size())
        return Position();
    for(unsigned i = 0; i < size(); i += degree)
        if(at(i).y == y)
            return Position(at(i));
    int seg = GetSeg(y);
    if(seg < 0)
        return Position();
    double high,low;
    if(at(seg).y < at(seg+degree).y) {
        high = 1.0;
        low = 0.0;
    }
    else  {
        high = 0.0;
        low = 1.0;
    }
    double close = DBL_MAX;
    Coord ret;
    do {
        double t = (high + low) / 2.0;
        Coord p = Bezier(&at(seg),degree,t);
        double d = absol(p.y - y);
        if(d < close) {
            ret = p;
            close = d;
        }
        if(p.y > y)
            high = t;
        else
            low = t;
    }
    while(absol(high - low) > .01); /* should be adaptive */
    return Position(ret);
}
const double DETAIL = 0.0005;
inline Coord *Line::clip(Coord *in, const Coord offset, const Region &reg, Coord *out) {
    double high = 1.0,
        low = 0.0;
    bool lowInside = reg.Hit(in[0]-offset),
        highInside = reg.Hit(in[degree]-offset);
    if(lowInside == highInside)
        return in;
    Coord work[4],
        *lowSeg,*highSeg;
    if(lowInside) {
        lowSeg = 0;
        highSeg = work;
    }
    else {
        lowSeg = work;
        highSeg = 0;
    }

    do {
        double t = (high + low) / 2.0;
        Coord p = Bezier(in,degree,t,lowSeg,highSeg);
        bool inside = reg.Hit(p-offset);
        if(inside == lowInside)
            low = t;
        else
            high = t;
    }
    while(high - low > DETAIL); /* should be adaptive with resolution */
    for(int i=0;i<=degree;++i)
        out[i] = work[i];
    return out;
}
Position Line::Intersection(int seg,Segment other) {
    switch(degree) {
    case 1:
        return lines_intersect(Segment(at(seg),at(seg+1)),other);
    case 3:
        double roots[4];
        /*
        if(between(other.a,at(seg),other.b))
            return Position(at(seg));
        if(between(other.a,at(seg+3),other.b))
            return Position(at(seg+3));
            */
        if(splineIntersectsLine(&at(seg),other,roots)>0)
            return Position(Bezier(&at(seg),3,roots[0]));
        else
            return Position();
    default:
    case 0:
        return Position();
    }
}

void showshape(const Line &shape,Coord ul,Coord ofs = Coord(0,0)) {
    if(shape.size()) {
        reports[dgr::bug] << "<path d=\"M" << shape[0].x+ofs.x-ul.x << ' ' << ul.y-(shape[0].y+ofs.y) << ' ';
        switch(shape.degree) {
        case 1: reports[dgr::bug] << "L";
            break;
        case 3: reports[dgr::bug] << "C";
            break;
        default:
            dgassert(0);
        }
        for(Line::const_iterator ci = shape.begin()+1;ci!=shape.end(); ++ci)
            reports[dgr::bug] << ci->x+ofs.x-ul.x << ' ' << ul.y-(ci->y+ofs.y);
        reports[dgr::bug] << '"' << endl <<
            "style=\"fill:none;stroke:rgb(0,0,0);stroke-width:1\"/>" << endl;
    }
}
void Line::ClipEndpoints(Line &source,const Coord offsetTail,const Region *tl,
                                    const Coord offsetHead, const Region *hd) {
    if(!tl&&!hd) {
        *(Line*)this = source;
        return;
    }
    Clear();
    degree = source.degree;
    if(source.Empty())
        return;

    /* find terminal sections */
    int firstI,lastI;
    if(tl) {
        for(firstI = 0; firstI < int(source.size())-1; firstI += degree)
            if(!tl->Hit(source[firstI + degree]-offsetTail))
                break;
    }
    else
        firstI = 0;
    if(hd) {
        for(lastI = source.size() - degree - 1; lastI >= 0; lastI -= degree)
            if(!hd->Hit(source[lastI]-offsetHead))
                break;
    }
    else
        lastI = source.size() - degree - 1;
    /*
    if(firstI>lastI) {
        reports[dgr::bug] << "<?xml version=\"1.0\" standalone=\"no\"?>" << endl <<
            "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\"" << endl <<
            "\"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">" << endl;
        reports[dgr::bug] << "<svg width=\"100%%\" height=\"100%%\">" << endl;
        const PolylineRegion *pr = static_cast<const PolylineRegion*>(tl);
        Coord ul = offsetTail; // (offsetTail.x - pr->boundary.Width()/2,offsetTail.y + pr->boundary.Height()/2);
        showshape(pr->shape,ul,offsetTail);
        pr = static_cast<const PolylineRegion*>(hd);
        showshape(pr->shape,ul,offsetHead);
        showshape(source,ul);
        reports[dgr::bug] << "</svg>" << endl;
    }
    */
    Coord temp[4];
    for(int i = firstI; i <= lastI; i += degree) {
        Coord *section = &source[i];
        if(i==firstI && tl)
            section = clip(&*section,offsetTail,*tl,temp);
        if(i==lastI && hd)
            section = clip(&*section,offsetHead,*hd,temp);
        AddSeg(section);
    }
}
inline void advance(int &A,int &B,int N) {
    B++;
    A = (A+1)%N;
}
enum {Pin=1,Qin=2,Unknown=0};

bool Line::operator &&(const Line &l2) const {
    // only for closed polys!
    int      n = size()-1,
             m = l2.size()-1;
    int      a = 0;
    int      b = 0;
    int      aa = 0;
    int      ba = 0;
    int      a1, b1;
    Coord    A, B;
    double    cross;
    bool     bHA, aHB;
    int      inflag = Unknown;

    do {
      a1 = (a + n - 1) % n;
      b1 = (b + m - 1) % m;

      A = at(a) - at(a1);
      B = l2[b] - l2[b1];

      cross = tri_area_2(Coord(0,0), A, B );
      bHA = leftOf( at(a1), at(a), l2[b] );
      aHB = leftOf( l2[b1], l2[b], at(a) );

        /* If A & B intersect, update inflag. */
      if (intersection( at(a1), at(a), l2[b1], l2[b]).valid )
        return 1;

                /* Advance rules. */
      if ( (cross == 0) && !bHA && !aHB ) {
        if ( inflag == Pin )
          advance( b, ba, m);
        else
          advance( a, aa, n);
      }
      else if ( cross >= 0 )
        if ( bHA )
          advance( a, aa, n);
        else {
          advance( b, ba, m);
      }
      else /* if ( cross < 0 ) */{
        if ( aHB )
          advance( b, ba, m);
        else
          advance( a, aa, n);
      }

    } while ( ((aa < n) || (ba < m)) && (aa < 2*n) && (ba < 2*m) );

    return 0;

}

} // namespace Dynagraph
