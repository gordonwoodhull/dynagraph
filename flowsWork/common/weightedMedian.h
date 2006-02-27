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

#ifndef weightedMedian_h
#define weightedMedian_h

namespace Dynagraph {

typedef std::vector<double> doubV;

template<typename T>
inline T weightedMedian(std::vector<T> &vec) {
	int n = vec.size();
	T m;
	switch(n) {
	case 0:
		m = T(0.0);
		abort();
		break;
	case 1:
		m = vec[0];
		break;
	case 2:
		m = (vec[0] + vec[1]) / 2.0;
		break;
	default:	/* weighted median */
		std::sort(vec.begin(),vec.end());
		if(n % 2)
			m = vec[n / 2];
		else {
			int rm = n / 2,
				lm = rm - 1;
			T rspan = vec[n - 1] - vec[rm],
				lspan = vec[lm] - vec[0];
			if(lspan == rspan)
				m = (vec[lm] + vec[rm]) / T(2.0);
			else {
				T w = vec[lm]*rspan + vec[rm]*lspan;
				m = w / (lspan + rspan);
			}
		}
		break;
	}
	return m;
}

} // namespace Dynagraph

#endif //weightedMedian_h
