/**********************************************************
*            This software is part of Dynagraph           *
*                http://www.dynagraph.org/                *
*                                                         *
*            Copyright (c) 2006 Gordon Woodhull           *
*        Portions Copyright (c) 1994-2005 AT&T Corp.      *
*  Licensed under the Common Public License, Version 1.0  *
*                                                         *
*      Dynagraph originates in the Graphviz toolset       *
*                   http://graphviz.org                   *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
*                       Many thanks.                      *
**********************************************************/

#ifndef EnginePair_h
#define EnginePair_h

namespace Dynagraph {

template<typename Graph>
struct EnginePair : std::pair<ChangeProcessor<Graph>*,ChangeProcessor<Graph>*> {
	EnginePair() {
		this->first = this->second = 0;
	}
	EnginePair(ChangeProcessor<Graph> *first,ChangeProcessor<Graph> *second) {
		dgassert((first!=0)==(second!=0)); // both or neither folks
		this->first = first;
		this->second = second;
	}
	void Prepend(ChangeProcessing *eng) {
		dgassert(eng);
		eng->next_ = this->first;
		this->first = eng;
		if(!this->second)
			this->second = eng;
	}
	void Prepend(const EnginePair<Graph> &other) {
		if(!other.first&&!other.second)
			return;
		dgassert(other.first&&other.second);
		other.second->next_ = this->first;
		this->first = other.first;
		if(!this->second)
			this->second = other.second;
	}
	void Append(ChangeProcessor<Graph> *eng) {
		dgassert(eng);
		if(this->second)
			this->second->next_ = eng;
		this->second = eng;
		if(!this->first)
			this->first = eng;
	}
	void Append(const EnginePair<Graph> &other) {
		if(!other.first&&!other.second)
			return;
		dgassert(other.first&&other.second);
		if(this->second)
			this->second->next_ = other.first;
		this->second = other.second;
		if(!this->first)
			this->first = other.first;
	}
};

} // namespace Dynagraph

#endif //EnginePair_h
