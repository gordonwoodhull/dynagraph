/*   Copyright (c) AT&T Corp.  All rights reserved.
   
This software may only be used by you under license from 
AT&T Corp. ("AT&T").  A copy of AT&T's Source Code Agreement 
is available at AT&T's Internet website having the URL 

http://www.research.att.com/sw/tools/graphviz/license/

If you received this software without first entering into a license 
with AT&T, you have an infringing copy of this software and cannot 
use it without violating AT&T's intellectual property rights. */

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
