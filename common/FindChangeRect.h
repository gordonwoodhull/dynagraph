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


struct FCRData {
	Layout changeN,changeE; // subgraph union of ins,mod,del
    FCRData(Layout *clientL) : changeN(clientL),changeE(clientL) {}
};
struct FCRBefore : Server {
    FCRData *data;
	FCRBefore(Layout *clientL,Layout *currentLayout,FCRData *data)  :
        Server(clientL,currentLayout), data(data) {}
	void Process(ChangeQueue &Q);
};
struct FCRAfter : Server {
    FCRData *data;
	FCRAfter(Layout *clientL,Layout *currentLayout,FCRData *data)  :
        Server(clientL,currentLayout),data(data) {}
    ~FCRAfter() {
        delete data;
    }
	void Process(ChangeQueue &Q);
};
