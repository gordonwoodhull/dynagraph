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


#include <iostream>
struct GeomParseError : DGException {
	std::string val;
	GeomParseError(std::string val = std::string()) :
	  DGException("coord reader expected comma"),val(val) {}
};
struct BadSplineValue : DGException {
	int val;
	BadSplineValue(int val) :
	  DGException("bezier degree must be 1 or 3"),val(val) {}
};
inline std::ostream & operator <<(std::ostream &os,const Coord &write) {
	os.flags(os.flags()|std::ios::fixed);
	os << write.x << ',' << write.y;
	return os;
}
inline std::istream & operator >>(std::istream &is, Coord &read) {
	is >> read.x;
	char c;
	is >> c;
	if(c!=',') {
		fprintf(stderr,">>Coord: %c %d\n",c,c);
		GeomParseError xep;
		xep.val.assign(1,c);
		throw xep;
	}
	is >> read.y;
	return is;
}
inline std::ostream & operator <<(std::ostream &os,const Position &write) {
	if(write.valid)
		os << (const Coord &)write;
	return os;
}
inline std::istream & operator >>(std::istream &is, Position &read) {
	if(read.valid = !is.eof())
		is >> (Coord &)read;
	return is;
}
inline std::ostream & operator <<(std::ostream &os,const Rect &write) {
	os << write.SW() << "," << write.NE();
	return os;
}
inline std::istream & operator >>(std::istream &is, Rect &read) {
	Coord sw,ne;
	is >> sw;
	char c;
	is >> c;
	if(c!=',') {
		fprintf(stderr,">>Coord: %c %d\n",c,c);
		GeomParseError xep;
		xep.val.assign(1,c);
		throw xep;
	}
	is >> ne;
	read.l = sw.x;
	read.b = sw.y;
	read.t = ne.x;
	read.r = ne.y;
	return is;
}
inline std::ostream & operator <<(std::ostream &os,const Bounds &write) {
	if(write.valid)
		os << (const Rect &)write;
	return os;
}
inline std::istream & operator >>(std::istream &is, Bounds &read) {
	if(read.valid = !is.eof())
		is >> (Rect &)read;
	return is;
}
inline std::istream & operator >>(std::istream &is,Line &read) {
	read.clear();
	char ch;
	if(!is.eof()) {
		is.get(ch);
		if(ch=='b') {
			int level;
			is >> level;
			switch(level) {
			case 1:
			case 3:
				read.degree = level;
				break;
			default:
				throw BadSplineValue(level);
			}
		}
		else {
			read.degree = 1;
			is.unget();
		}
	}
	while(!is.eof()) {
		Coord c;
		is >> c;
		read.push_back(c);
	}
	return is;
}
inline std::ostream & operator <<(std::ostream &os,const Line &write) {
	os << 'b' << write.degree;
	for(Line::const_iterator ci = write.begin(); ci!=write.end(); ++ci)
		os << ' ' << *ci;
	return os;
}
inline std::istream & operator >>(std::istream &is,Lines &read) {
	read.clear();
	while(!is.eof()) {
		read.push_back(Line());
		is >> read.back();
	}
	return is;
}
inline std::ostream & operator <<(std::ostream &os,const Lines &write) {
	for(Lines::const_iterator li = write.begin(); li!=write.end(); ++li)
		os << *li << ';';
	return os;
}
