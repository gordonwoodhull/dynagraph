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

#include "LayoutConfigurators.h"
#include "common/StringizerConfigurator.h"
#include "incrface/RegisteringConfigurator.h"
#include "common/NamedToNamedChangeTranslator.h"

#include <boost/mpl/joint_view.hpp>

namespace mpl = boost::mpl;

namespace Dynagraph {
    
typedef boost::mpl::joint_view<LayoutConfigurators, 
    mpl::list<StringizerConfigurator,RegisteringConfigurator> > IncrConfigurators;

void createIncrConfiguration(Name name,StrAttrs &attrs) {
	configureLayout<IncrConfigurators>(name,attrs);
}

template<typename InTranslator, typename OutTranslator, typename OuterLayout, typename InnerLayout>
EnginePair<OuterLayout> wrapWorld(ChangingGraph<InnerLayout> *innerWorld, EnginePair<InnerLayout> innerEngines) {
	typedef InternalWorld<OuterLayout, InnerLayout> InWorld;
	ChangingGraph<OuterLayout> *outerWorld = new ChangingGraph<GeneralLayout>;
	InWorld *inWorld = new InWorld(outerWorld,innerWorld);
	inWorld->inTranslators_.push_back(new InTranslator(outerWorld,innerWorld));
	inWorld->outTranslators_.push_back(new OutTranslator(innerWorld,outerWorld));
	//innerEngines.Prepend(new UpdateCurrentProcessor<InnerLayout>(innerWorld));
	inWorld->innerEngines_ = innerEngines;
	inWorld->CompleteConfiguration();
	EnginePair<OuterLayout> outerEngines;
	outerEngines.Append(inWorld);
	outerEngines.Append(new OkayEngine<OuterLayout>(outerWorld));
    return outerEngines;
}
// the final type of layout is built up by the chain of templated configurators
// this configurator wraps any layout/engines in all-purpose layout/engines
struct GeneralizerConfigurator {
    static EnginePair<GeneralLayout> s_outerEngines;
    // possibly overload this for Layout already==GeneralLayout
	template<typename ConfigRange,typename Layout> 
	static void config(DString name,const StrAttrs &attrs, ChangingGraph<Layout> *innerWorld, EnginePair<Layout> innerEngines) {
		BOOST_MPL_ASSERT((typename boost::is_same<typename boost::mpl::first<ConfigRange>::type, 
    	                                        typename boost::mpl::second<ConfigRange>::type>)); // must be end of line
		typedef NamedToNamedChangeTranslator<GeneralLayout,Layout,LayoutToLayoutTranslator<GeneralLayout,Layout>,false> InTranslator;
		typedef NamedToNamedChangeTranslator<Layout,GeneralLayout,LayoutToLayoutTranslator<Layout,GeneralLayout>,false> OutTranslator;
		gd<Name>(&innerWorld->whole_) = std::string(name)+"_specific";
                s_outerEngines = wrapWorld<InTranslator, OutTranslator, GeneralLayout>(innerWorld, innerEngines); // not thread-safe
    }
};
EnginePair<GeneralLayout> GeneralizerConfigurator::s_outerEngines;

  typedef boost::mpl::joint_view<LayoutConfigurators, mpl::list<GeneralizerConfigurator> >
    CppConfigurators;

EnginePair<GeneralLayout> createCppConfiguration(Name name,StrAttrs &attrs) {
    configureLayout<CppConfigurators>(name,attrs);
    return GeneralizerConfigurator::s_outerEngines;
}



} // namespace Dynagraph
