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

#include "dynadag/DynaDAGLayout.h"
#include "fdp/FDPLayout.h"
#include "common/FindChangeRect.h"
#include "common/breakList.h"

template<typename Layout>
struct ServerCreator {
	typedef Server<Layout>* (*create_fn)(Layout *cli,Layout *curr);
};
template<typename ServerType>
struct ServerCreatorInstance {
	typedef typename ServerType::LayoutType Layout;
	static Server<Layout>* create(Layout *client,Layout *current) {
		return new ServerType(client,current);
	}
};

template<typename Layout>
struct creators;

template<>
struct creators<DynaDAG::DynaDAGLayout> : std::map<DString,ServerCreator<DynaDAG::DynaDAGLayout>::create_fn> {
	creators();
	static creators the_creators;
};

template<>
struct creators<FDP::FDPLayout> : std::map<DString,ServerCreator<FDP::FDPLayout>::create_fn> {
	creators();
	static creators the_creators;
};

// creates the servers specified in gd<StrAttrs>(client)["engines"]
template<typename Layout>
Server<Layout> *createLayoutServer(Layout *client,Layout *current) {
	CompoundServer<Layout> *eng = new CompoundServer<Layout>(client,current);
    FCRData<Layout> *fcrdata = new FCRData<Layout>(client);
    FCRBefore<Layout> *fcrbefore = new FCRBefore<Layout>(client,current,fcrdata);
    FCRAfter<Layout> *fcrafter = new FCRAfter<Layout>(client,current,fcrdata);
	eng->actors.push_back(fcrbefore);
	eng->actors.push_back(new UpdateCurrent<Layout>(client,current)); 
	DString engines = gd<StrAttrs>(client)["engines"];
	if(engines.empty())
		throw DGException("engine(s) were not specified on call to createLayoutServer");
    //gd<StrAttrs>(client)["agecolors"] = "green,blue,black";
    std::vector<DString> engs;
    breakList(engines,engs);
	creators<Layout> &the_creators = creators<Layout>::the_creators;
    for(std::vector<DString>::iterator ei = engs.begin(); ei!=engs.end(); ++ei) {
		typename ServerCreator<Layout>::create_fn crea = the_creators[*ei];
		if(!crea) {
			std::cout << the_creators.size() << " creators:" << std::endl;
			for(typename creators<Layout>::iterator ci = the_creators.begin(); ci!=the_creators.end(); ++ci)
				std::cout << reinterpret_cast<int>(ci->first.c_str()) << " " << ci->first << " -> " << ci->second << std::endl;
			delete eng;
			throw DGException2("engine name not known or not appropriate for graph type",strdup(ei->c_str())); // excuse me
		}
		Server<Layout> *server = crea(client,current);
		eng->actors.push_back(server);
	}
	eng->actors.push_back(fcrafter);

	return eng;
}
