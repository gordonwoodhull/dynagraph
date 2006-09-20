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

#ifndef ChangeTranslator_h
#define ChangeTranslator_h

#include "ChangeProcessor.h"

namespace Dynagraph {

/* translators are different from processors in that
	* they have both a source and destination world
	* they are not chained
  the first difference is obvious.  the second one is debatable.  
  it does not allow e.g. intermediate translations.
  it does make InternalWorld slightly easier to implement, but not so much so.
  we'll see.
*/
template<typename SourceGraph,typename DestGraph>
struct ChangeTranslator {
	ChangingGraph<SourceGraph> *sourceWorld_;
	ChangingGraph<DestGraph> *destWorld_;
	ChangeTranslator(ChangingGraph<SourceGraph> *sourceWorld,ChangingGraph<DestGraph> *destWorld)
		: sourceWorld_(sourceWorld),destWorld_(destWorld) {}

	virtual void Open() {}
	virtual void Process() {}
	virtual void Close() {}
};

} // namespace Dynagraph

#endif // ChangeTranslator_h
