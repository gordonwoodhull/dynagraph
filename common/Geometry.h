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


#ifndef Geometry_h
#define Geometry_h

#include <math.h>
#include "moremath.h"
#include <vector>
#include <list>
#include <algorithm>
#include <float.h>
#include "StringDict.h"
#include "useful.h"

namespace Dynagraph {

struct Coord {
	double x,y;
	Coord() : x(-17),y(-17) {}
	Coord(double x, double y) : x(x),y(y){}
	bool operator ==(const Coord &c) const { // not recommended because floats are never equal
		return x==c.x && y==c.y;
	}
	bool operator !=(const Coord &c) const { // not recommended because floats are never equal
		return !(*this==c);
	}
	const Coord operator +(const Coord &c) const {
		return Coord(x+c.x,y+c.y);
	}
	const Coord operator -(const Coord &c) const {
		return Coord(x-c.x,y-c.y);
	}
	const Coord operator -() const {
		return Coord(-x,-y);
	}
	const Coord operator *(double a) const {
		return Coord(a*x,a*y);
	}
	const Coord operator /(double a) const {
		return Coord(x/a,y/a);
	}
	Coord &operator +=(const Coord &c) {
		return *this = *this + c;
	}
	Coord &operator -=(const Coord &c) {
		return *this = *this - c;
	}
	Coord &operator *=(double a) {
		return *this = *this*a;
	}
	Coord &operator /=(double a) {
		return *this = *this/a;
	}
	// multiply each field
	const Coord operator *(Coord a) const {
		return Coord(x*a.x,y*a.y);
	}
	// dot product
	double operator %(Coord a) const {
		return x*a.x+y*a.y;
	}
	Coord operator /(Coord a) const {
		return Coord(x/a.x,y/a.y);
	}
	double Len() const {
		return sqrt(*this % *this);
	}
	Coord Norm() const {
		double d = Len();
		if(d)
			return *this / d;
		else
			return *this;
	}
	Coord Abs() const {
		return Coord(fabs(x),fabs(y));
	}
};
inline bool is_vclose(const Coord &c1,const Coord &c2) {
	return is_vclose(c1.x,c2.x) && is_vclose(c1.y,c2.y);
}
inline bool isnt_vclose(const Coord &c1,const Coord &c2) {
	return !is_vclose(c1,c2);
}
inline const Coord operator *(double a,const Coord &c) {
	return c*a;
}
inline double dSquared(Coord a,Coord b) {
	Coord c = a-b;
	return c%c;
}
inline double dist(Coord a,Coord b) {
	return sqrt(dSquared(a,b));
}
struct Position : Coord {
	bool valid;
	Position() : valid(false) {}
	Position(double x, double y) : Coord(x,y),valid(true) {}
	Position(Coord c) : Coord(c),valid(true) {}
	Position &operator =(const Coord &c) {
		static_cast<Coord&>(*this) = c;
		valid = true;
		return *this;
	}
	void invalidate() {
		valid = false;
		x = y = -17;
	}
	bool operator ==(const Position &p) const { // not recommended because floats are never equal
		return valid?Coord::operator==(p):!p.valid;
	}
	bool operator !=(const Position &p) const { // not recommended because floats are never equal
		return !(*this==p);
	}
};
inline bool is_vclose(const Position &p1,const Position &p2) {
	return p1.valid?p2.valid&&is_vclose(static_cast<const Coord&>(p1),static_cast<const Coord&>(p2)):!p2.valid;
}
inline bool isnt_vclose(const Position &p1,const Position &p2) {
	return !is_vclose(p1,p2);
}
struct Segment {
	Coord a,b;
	Segment(Coord a = Coord(),Coord b = Coord()) : a(a), b(b) {}
};
struct Rect {
	double l,t,r,b;
	Rect() {}
	Rect(const Rect &r) : l(r.l),t(r.t),r(r.r),b(r.b) {}
	Rect(double l,double t,double r,double b) : l(l),t(t),r(r),b(b) {}
	explicit Rect(Coord c) : l(c.x),t(c.y),r(c.x),b(c.y) {}
	void Clear() {
		l = t = r = b = 0.;
	}
	bool operator==(const Rect &r) const { // not recommended because floats are never equal
		return l==r.l && t==r.t && this->r==r.r && b==r.b;
	}
	bool operator!=(const Rect &r) const { // not recommended because floats are never equal
		return !(*this==r);
	}
	double Width() const {
		return r-l;
	}
	double Height() const {
		return fabs(t-b);
	}
	Coord Size() const {
		return Coord(Width(),Height());
	}
	bool Contains(Coord c) const {
		return l<=c.x && c.x<=r &&
			(b<t)?(b<=c.y && c.y<=t):(t<=c.y && c.y<=b);
	}
	Coord Center() const {
		return Coord((l+r)/2.0,(t+b)/2.0);
	}
	Coord SW() const {
		return Coord(l,b);
	}
	Coord NE() const {
		return Coord(r,t);
	}
	Coord SE() const {
		return Coord(r,b);
	}
	Coord NW() const {
		return Coord(l,t);
	}
	Rect operator+(Coord c) const {
		return Rect(l+c.x,t+c.y,r+c.x,b+c.y);
	}
	Rect operator-(Coord c) const {
		return Rect(l-c.x,t-c.y,r-c.x,b-c.y);
	}
	Rect operator|(Rect o) const { // union
		return (b<=t)?Rect(std::min(l,o.l),std::max(t,o.t),std::max(r,o.r),std::min(b,o.b)):
			Rect(std::min(l,o.l),std::min(t,o.t),std::max(r,o.r),std::max(b,o.b));
	}
	Rect operator|=(Rect o) {
		return *this = *this|o;
	}
	bool operator &&(Rect o) const { // intersect test
		return ((b<=t)?(b<o.t && t>o.b):(b>o.t && t<o.b))
			&& l<o.r && r>o.l;
	}
};
inline bool is_vclose(const Rect &r1,const Rect &r2) {
	return is_vclose(r1.l,r2.l) && is_vclose(r1.t,r2.t) && is_vclose(r1.r,r2.r) && is_vclose(r1.b,r2.b);
}
inline bool isnt_vclose(const Rect &r1,const Rect &r2) {
	return !is_vclose(r1,r2);
}

struct Bounds : Rect {
	bool valid;
	Bounds() : valid(false) {}
	Bounds(Rect r) : Rect(r),valid(true) {}
	Bounds(Position p) : Rect(p),valid(p.valid) {}
	void Clear() {
		Rect::Clear();
		valid = false;
	}
	bool operator ==(const Bounds &b) const { // not recommended because floats are never equal
		return valid?Rect::operator ==(b):!b.valid; 
	}
	bool operator !=(const Bounds &b) const { // not recommended because floats are never equal
		return !(*this==b);
	}
	Bounds &operator |=(const Rect &r) {
		if(!valid)
			*this = r;
		else
			static_cast<Rect&>(*this) |= r;
		return *this;
	}
	Bounds &operator |=(const Bounds &b) {
		if(b.valid)
			*this |= static_cast<const Rect&>(b);
		return *this;
	}
};
inline bool is_vclose(const Bounds &b1,const Bounds &b2) {
	return b1.valid?b2.valid&&is_vclose(static_cast<const Rect&>(b1),static_cast<const Rect&>(b2)):!b2.valid;
}
inline bool isnt_vclose(const Bounds &b1,const Bounds &b2) {
	return !is_vclose(b1,b2);
}
struct Region;
struct Line : std::vector<Coord> {
	typedef std::vector<Coord>::iterator iterator; // for gcc 3.0; why?
    Coord &at(int i) { return operator[](i); } // for earlier gccs
	Coord at(int i) const { return operator[](i); }
	int degree;
	Line() : degree(0) {}
	Line(size_t N) : std::vector<Coord>(N),degree(0) {} // ick
	void Clear() {
		clear();
		degree = 0;
	}
	bool Empty() {
		return degree==0 || !size();
	}
	int GetSeg(double y);
	template<typename It> // T* and std::vector<T>::iterator are finally different types in gcc 3.0!
	void AddSeg(It ci) {
		It b = ci,
			e = ci+degree+1;
		if(size()) {
			dgassert(back()==*b);
			b++;
		}
		insert(end(),b,e);
	}
	Position YIntersection(double y);
	Position Intersection(int seg,Segment other);
	Bounds BoundingBox() {
		Bounds ret;
		for(iterator ci = begin(); ci!=end(); ++ci)
			ret |= Rect(*ci);
		return ret;
	}
	Line &operator +=(const Line &append) {
		dgassert(degree==append.degree);
		Coord b = back(),f = append.front();
		dgassert(b==f);
		insert(end(),append.begin()+1,append.end());
		return *this;
	}
	Line operator +(Coord c) const {
		Line ret;
        ret.degree = degree;
		for(const_iterator pi = begin(); pi!=end(); ++pi)
			ret.push_back(*pi+c);
		return ret;
	}
	Line operator -(Coord c) const {
		return *this + -c;
	}
	Line operator *(double a) const {
		Line ret(size());
        ret.degree = degree;
		iterator out = ret.begin();
		for(const_iterator pi = begin(); pi!=end(); ++pi)
			*out++ = *pi*a;
		return ret;
	}
	Line operator /(double a) const {
		return *this * (1.0/a);
	}
	bool operator &&(const Line &l2) const;
	void ClipEndpoints(Line &source,const Coord offsetTail,const Region *tl,
		const Coord offsetHead,const Region *hd); // NG*'s NULL to not clip
private:
	Coord *clip(Coord *in,const Coord offset,const Region &reg,Coord *out);
};
typedef std::list<Line> Lines;

struct Region {
	Bounds boundary;
	Line shape;
	mutable int lastOut; // to make iterative calls quicker
	Region() : lastOut(0) {}
	void Clear();
	void updateBounds(); // from new Line data
	bool Hit(Coord c) const;
	bool Overlaps(Coord ofs1,Coord ofs2,const Region &r);
private:
	bool sameSide(const Coord &p0, const Coord &p1, int seg) const;
};
// check that e.g. YIntersection exists
inline Coord checkPos(Position p) { dgassert(p.valid); return p; }

} // namespace Dynagraph

#include "GeomUtil.h"
#include "LineTricks.h"
#include "StreamGeom.h"

#endif // Geometry_h
