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

#ifndef createStringHandlers_h
#define createStringHandlers_h

#include "IncrStrGraphHandler.h"
#include "WorldInABox.h"
#include "TextWatcherOutput.h"
#include "common/NamedToNamedChangeTranslator.h"
#include "common/LayoutToLayoutTranslator.h"
#include "common/createEngine.h"
#include "common/InternalTranslator.h"
#include "common/StringLayoutTranslator.h"
#include "common/CoordTranslator.h"

#include "dynadag/DynaDAGLayout.h"
#include "fdp/FDPLayout.h"

namespace Dynagraph {

template<typename OuterLayout,typename InnerLayout>
struct translator_traitses {
	typedef NamedToNamedChangeTranslator<OuterLayout,InnerLayout,
		LayoutToLayoutTranslator<OuterLayout,InnerLayout> > in_translator;
	typedef NamedToNamedChangeTranslator<InnerLayout,OuterLayout,
		LayoutToLayoutTranslator<InnerLayout,OuterLayout> > out_translator;
};

template<typename Layout>
struct SimpleGuts {
	typedef Layout ResultLayout;
	DString engines_;
	SimpleGuts(DString engines) : engines_(engines) {}
	EnginePair<Layout> operator()(ChangingGraph<Layout> &chraph,Transform *transform) {
		return createEngine(engines_,&chraph);
	}
};
template<typename GutsCreator,
	typename InTranslator = translator_traitses<GeneralLayout,GutsCreator::ResultLayout>::in_translator,
	typename OutTranslator = translator_traitses<GeneralLayout,GutsCreator::ResultLayout>::out_translator>
struct BoxGuts {
	typedef GeneralLayout ResultLayout;
	DString engines_;
	GutsCreator gutsFun_;
	BoxGuts(GutsCreator gutsFun, DString engines) : gutsFun_(gutsFun),engines_(engines) {}
	EnginePair<GeneralLayout> operator()(ChangingGraph<GeneralLayout> &chraph,Transform *transform) {
		typedef GutsCreator::ResultLayout InnerLayout;
		typedef WorldInABox<GeneralLayout,InnerLayout,InTranslator,OutTranslator> Box;
		ChangingGraph<InnerLayout> *innerWorld = new ChangingGraph<InnerLayout>;
		gd<Name>(&innerWorld->whole_) = "INR";
		// CP::Open makes this unnecessary
		//gd<GraphGeom>(&innerWorld->current_) = gd<GraphGeom>(&chraph.current_); // ensure attributes read from strings get duped in b4 engines init
		Box *box = new Box(&chraph,innerWorld);
		EnginePair<InnerLayout> innerEngines = gutsFun_(*innerWorld,transform);
		box->assignEngine(innerEngines,transform);
		EnginePair<GeneralLayout> engines = createEngine(engines_,&chraph);
		engines.Append(box);
		return engines;
	}
};
/*
template<typename GutsCreator,
	typename InTranslator = translator_traitses<GeneralLayout,GutsCreator::ResultLayout>::in_translator,
	typename OutTranslator = translator_traitses<GeneralLayout,GutsCreator::ResultLayout>::out_translator>
BoxGuts<GutsCreator,InTranslator,OutTranslator> boxGuts(GutsCreator gutsFun,DString engines) {
	return BoxGuts<GutsCreator,InTranslator,OutTranslator>(gutsFun,engines);
}
*/
template<typename Layout,typename GutsCreator>
IncrLangEvents *createStringHandlers(ChangingGraph<Layout> *chraph,GutsCreator gutsFun,IncrViewWatcher<Layout> *watcher,
									  LinkedChangeProcessor<Layout> *before,LinkedChangeProcessor<Layout> *after,
									  DString gname,const StrAttrs &attrs,Transform *transform, bool useDotDefaults) {
	IncrStrGraphHandler<Layout> *handler = new IncrStrGraphHandler<Layout>(chraph);
	handler->watcher_ = watcher;

    gd<Name>(&chraph->whole_) = gname;
    SetAndMark(chraph->Q_.ModGraph(),attrs);

	// apply first graph attributes before creating engines (not pretty)
	// (obsoleted by ChangeProcessor::Open and non-reliance of engines on attributes already set)
	//StringToLayoutTranslator<Layout,Layout>(transform,useDotDefaults).ModifyGraph(chraph->Q_.ModGraph(),&chraph->whole_);

	EnginePair<Layout> engines = gutsFun(*chraph,transform);

	// engine to translate strings to binary attrs
	typedef InternalTranslator2<Layout,StringToLayoutTranslator<Layout,Layout> > StringsInEngine;
	StringsInEngine *xlateIn = new StringsInEngine(engines.first->world_,StringToLayoutTranslator<Layout,Layout>(transform,useDotDefaults));
	engines.Prepend(xlateIn);

	// engine to translate binary attrs to strings
	typedef InternalTranslator2<Layout,LayoutToStringTranslator<Layout,Layout> > StringsOutEngine;
	StringsOutEngine *xlateOut = new StringsOutEngine(engines.first->world_,transform);
	engines.Append(xlateOut);

	if(before)
		engines.Prepend(before);
	if(after)
		engines.Append(after);
	handler->next_ = engines.first;
	return handler;
}
template<typename GutsCreator>
IncrLangEvents *createHandlers(GutsCreator gutsFun,DString gname,const StrAttrs &attrs) {
	typedef typename GutsCreator::ResultLayout ResultLayout;
	ChangingGraph<ResultLayout> *world = new ChangingGraph<ResultLayout>;
	return createStringHandlers<ResultLayout>(world,gutsFun,
		new TextWatcherOutput<ResultLayout>,0,new OutputIncrface<ResultLayout>(world,dgr::output),
		gname,attrs,g_transform,g_useDotDefaults);
}
template<typename GutsCreator>
IncrLangEvents *maybeTranslate(GutsCreator gutsFun,DString gname,const StrAttrs &attrs) {
	/*
	if(attrs.look("coordtranslation","true")=="true)
		return createHandlers(CoordTranslatorGuts(gutsFun),gname,attrs);
	else
	*/
		return createHandlers(gutsFun,gname,attrs);
}
template<typename Layout>
IncrLangEvents *maybeSuper(DString gname,const StrAttrs &attrs) {
	if(attrs.look("superengines")) 
		return maybeTranslate(BoxGuts<SimpleGuts<Layout> >(SimpleGuts<Layout>(attrs.look("engines")),attrs.look("superengines")),gname,attrs);
	else
		return maybeTranslate(SimpleGuts<Layout>(attrs.look("engines")),gname,attrs);
}
inline IncrLangEvents *createHandlers(DString type,DString gname,const StrAttrs &attrs) {
	if(type=="dynadag") 
		return maybeSuper<DynaDAG::DynaDAGLayout>(gname,attrs);
	else if(type=="fdp")
		return maybeSuper<FDP::FDPLayout>(gname,attrs);
	dgassert(false);
	return 0;
}

} // namespace Dynagraph
#endif // createStringHandlers_h
