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


#include "voronoi/voronoi.h"
#include "fdp/fdp.h"
#include "shortspline/ObAvSplinerEngine.h"
#include "dynadag/DynaDAG.h"
#include "common/ColorByAge.h"
#include "common/FindChangeRect.h"
#include "createLayoutServer.h"
#include "common/breakList.h"

using namespace std;

// is it possible to do this w/out the struct?
typedef Server *(*creatorf)(Layout *cli,Layout *curr);
template<typename ST>
struct tcreator {
	static Server *create(Layout *client,Layout *current) {
		return new ST(client,current);
	}
};
struct creators : map<DString,creatorf> {
	creators() {
		creators &me = *this;

		me["dynadag"] = tcreator<DynaDAG::DynaDAGServer>::create;
		me["fdp"] = tcreator<FDP::FDPServer>::create;
		me["voronoi"] = tcreator<Voronoi::VoronoiServer>::create;
		me["visspline"] = tcreator<ObAvSplinerEngine>::create;
		me["labels"] = tcreator<LabelPlacer>::create;
		me["shapegen"] = tcreator<ShapeGenerator>::create;
        me["colorbyage"] = tcreator<ColorByAge>::create;
	}
} g_creators;
Server *createLayoutServer(Layout *client,Layout *current) {
	CompoundServer *eng = new CompoundServer(client,current);
    FCRData *fcrdata = new FCRData(client);
    FCRBefore *fcrbefore = new FCRBefore(client,current,fcrdata);
    FCRAfter *fcrafter = new FCRAfter(client,current,fcrdata);
	eng->actors.push_back(fcrbefore);
	eng->actors.push_back(new UpdateCurrent(client,current)); // obligatory
	DString serverlist = gd<StrAttrs>(client)["engines"];
	if(serverlist.empty())
		gd<StrAttrs2>(client).put("engines",serverlist = "shapegen,dynadag,labels");
    //gd<StrAttrs>(client)["agecolors"] = "green,blue,black";
    vector<DString> engs;
    breakList(serverlist,engs);
    for(vector<DString>::iterator ei = engs.begin(); ei!=engs.end(); ++ei) {
		creatorf crea = g_creators[*ei];
		if(!crea) {
			/*
			cout << (int)g_creators.size() << " creators:" << endl;
			for(creators::iterator ci = g_creators.begin(); ci!=g_creators.end(); ++ci)
				cout << reinterpret_cast<int>(ci->first.c_str()) << " " << ci->first << " -> " << ci->second << endl;
			*/
			delete eng;
			throw DGException("engine name not known",strdup(ei->c_str())); // excuse me
		}
		Server *server = crea(client,current);
		eng->actors.push_back(server);
	}
	eng->actors.push_back(fcrafter);

	return eng;
}
