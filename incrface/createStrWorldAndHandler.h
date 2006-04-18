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

#include "incrface/IncrStrGraphHandler.h"
#include "common/WorldInABox.h"
#include "common/LayoutToLayoutTranslator.h"
#include "common/createEngine.h"
#include "common/stringizeEngine.h"

namespace Dynagraph {

template<typename Layout>
struct WorldGuts {
	DString engines_,superengines_;
	WorldGuts(DString superengines, DString engines) : engines_(engines),superengines_(superengines) {}
	EnginePair<GeneralLayout> operator()(ChangeQueue<GeneralLayout> &Q,IncrWorld<GeneralLayout> &world) {
		SetAndMark(Q.ModGraph(),"engines",engines_);
		typedef WorldInABox<GeneralLayout,Layout,LayoutToLayoutTranslator<GeneralLayout,Layout>,LayoutToLayoutTranslator<Layout,GeneralLayout> > Box;
		Box *box = new Box;
		box->assignEngine(engines_,world);
		EnginePair<GeneralLayout> engine(box,box);
		engine.Prepend(createEngine<GeneralLayout>(superengines_,&world.whole_,&world.current_));
		return engine;
	}
};
template<typename Layout>
struct SimpleGuts {
	DString engines_;
	SimpleGuts(DString engines) : engines_(engines) {}
	EnginePair<Layout> operator()(ChangeQueue<Layout> &Q,IncrWorld<Layout> &world) {
		SetAndMark(Q.ModGraph(),"engines",engines_);
		return createEngine(engines_,&world.whole_,&world.current_);
	}
};
template<typename Layout,typename GutsCreator>
IncrLangEvents *createStrWorldAndHandler(GutsCreator gutsFun,IncrViewWatcher<Layout> *watcher,
									  LinkedChangeProcessor<Layout> *before,LinkedChangeProcessor<Layout> *after,
									  Transform *transform, bool useDotDefaults) {
	IncrWorld<Layout> *world = new IncrWorld<Layout>;
	IncrStrGraphHandler<Layout> *handler = new IncrStrGraphHandler<Layout>(world);
	handler->watcher_ = watcher;

	// apply first graph attributes before creating engine (not pretty)
	StringToLayoutTranslator<Layout,Layout>(transform,useDotDefaults).ModifyGraph(&world->whole_,&world->whole_);

	EnginePair<Layout> eng0 = gutsFun(handler->Q_,*world);
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
