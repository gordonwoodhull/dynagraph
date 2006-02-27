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

#ifndef DynagraphServer_h
#define DynagraphServer_h

namespace Dynagraph {

template<typename Layout>
struct Server {
	typedef Layout LayoutType;
	// shared by all actors in a system;  represents everything currently inserted
	// note this is a subgraph of the client layout
	Layout *const client, *const current;

	virtual void Process(ChangeQueue<Layout> &Q) = 0;
	Server(Layout *client,Layout *current) : client(client), current(current) {}
	virtual ~Server() {}
};
template<typename Layout>
struct CompoundServer : Server<Layout> {
	typedef std::vector<Server<Layout>*> ServerV;
	ServerV actors;
	void Process(ChangeQueue<Layout> &Q);
	CompoundServer(Layout *client,Layout *currentLayout) : Server<Layout>(client,currentLayout) {}
	~CompoundServer();
};
template<typename Layout>
void CompoundServer<Layout>::Process(ChangeQueue<Layout> &Q) {
	for(typename ServerV::iterator i = actors.begin(); i!=actors.end(); ++i)
		(*i)->Process(Q);
}
template<typename Layout>
CompoundServer<Layout>::~CompoundServer() {
	for(typename ServerV::iterator i = actors.begin(); i!=actors.end(); ++i)
		delete *i;
	actors.clear();
}

// simple server that just updates the current subgraph based on changes.
// this must only be done once, that's why individual layout servers can't be responsible.
template<typename Layout>
struct UpdateCurrent : Server<Layout> {
	UpdateCurrent(Layout *client,Layout *currentLayout) : Server<Layout>(client,currentLayout) {}
	void Process(ChangeQueue<Layout> &Q) {
		Q.UpdateCurrent();
	}
};

} // namespace Dynagraph

#endif //DynagraphServer_h
