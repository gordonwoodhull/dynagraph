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


inline double B0(double t) {
    double tmp = 1.0 - t;
    return tmp * tmp * tmp;
}

inline double B1(double t) {
    double tmp = 1.0 - t;
    return 3 * t * tmp * tmp;
}

inline double B2(double t) {
    double tmp = 1.0 - t;
    return 3 * t * t * tmp;
}

inline double B3(double t) {
    return t * t * t;
}

inline double B01(double t) {
    double tmp = 1.0 - t;
    return tmp * tmp *(tmp + 3 * t);
}

inline double B23(double t) {
    double tmp = 1.0 - t;
    return t * t *(3 * tmp + t);
}
