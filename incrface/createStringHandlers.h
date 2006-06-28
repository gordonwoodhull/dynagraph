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

#ifndef createStrWorldAndHandler_h
#define createStrWorldAndHandler_h

#include "IncrStrGraphHandler.h"
#include "common/WorldInABox.h"
#include "common/NamedToNamedChangeTranslator.h"
#include "common/LayoutToLayoutTranslator.h"
#include "common/createEngine.h"
#include "common/stringizeEngine.h"

namespace Dynagraph {

template<typename OuterLayout,typename InnerLayout>
struct translator_traitses {
	typedef NamedToNamedChangeTranslator<OuterLayout,InnerLayout,
		GoingQueueTransition<OuterLayout>,LayoutToLayoutTranslator<OuterLayout,InnerLayout> > in_translator;
	typedef NamedToNamedChangeTranslator<InnerLayout,OuterLayout,
		ReturningQueueTransition<InnerLayout>,LayoutToLayoutTranslator<InnerLayout,OuterLayout> > out_translator;
};

template<typename Layout>
struct WorldGuts {
	DString engines_,superengines_;
	WorldGuts(DString superengines, DString engines) : engines_(engines),superengines_(superengines) {}
	EnginePair<GeneralLayout> operator()(ChangingGraph<GeneralLayout> &chraph) {
		typedef WorldInABox<GeneralLayout,Layout,
			typename translator_traitses<GeneralLayout,Layout>::in_translator,
			typename translator_traitses<GeneralLayout,Layout>::out_translator> Box;
		Box *box = new Box(&chraph);
		box->assignEngine(engines_);
		EnginePair<GeneralLayout> engine = box->engines();
		engine.Prepend(createEngine<GeneralLayout>(superengines_,&chraph));
		return engine;
	}
};
template<typename Layout>
struct SimpleGuts {
	DString engines_;
	SimpleGuts(DString engines) : engines_(engines) {}
	EnginePair<Layout> operator()(ChangingGraph<Layout> &chraph) {
		return createEngine(engines_,&chraph);
	}
};
template<typename Layout,typename GutsCreator>
IncrLangEvents *createStringHandlers(ChangingGraph<Layout> *chraph,GutsCreator gutsFun,IncrViewWatcher<Layout> *watcher,
									  LinkedChangeProcessor<Layout> *before,LinkedChangeProcessor<Layout> *after,
									  DString gname,const StrAttrs &attrs,Transform *transform, bool useDotDefaults) {
	IncrStrGraphHandler<Layout> *handler = new IncrStrGraphHandler<Layout>(chraph);
	handler->watcher_ = watcher;

    gd<Name>(&chraph->whole_) = gname;
    SetAndMark(chraph->Q_.ModGraph(),attrs);

	// apply first graph attributes before creating engine (not pretty)
	StringToLayoutTranslator<Layout,Layout>(transform,useDotDefaults).ModifyGraph(chraph->Q_.ModGraph(),&chraph->whole_);

	EnginePair<Layout> eng0 = gutsFun(*chraph);
	EnginePair<Layout> engine = stringizeEngine(eng0,transform,useDotDefaults);
	if(before)
		engine.Prepend(before);
	if(after)
		engine.Append(after);
	handler->next_ = engine.first;
	return handler;
}

} // namespace Dynagraph
#endif // createStrWorldAndHandler_h
