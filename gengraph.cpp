/*   Copyright (c) AT&T Corp.  All rights reserved.
   
This software may only be used by you under license from 
AT&T Corp. ("AT&T").  A copy of AT&T's Source Code Agreement 
is available at AT&T's Internet website having the URL 

http://www.research.att.com/sw/tools/graphviz/license/

If you received this software without first entering into a license 
with AT&T, you have an infringing copy of this software and cannot 
use it without violating AT&T's intellectual property rights. */

#include "common/LGraph-cdt.h"
#include "common/StrAttr.h"
#include "common/emitGraph.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

void usage() {
	fprintf(stderr,"gengraph\n"
		"\t-v N\tN vertices\n"
		"\t-e N\tN edges\n"
		"\t-c\tassign random colors (red,yellow,blue) to edges\n"
		"\t-w W\tassign random weights [0..W) to edges\n");
	exit(-1);
}

int main(int narg,char *argh[]) {
	int V = 100, E = 100;
	bool colors = false;
	int maxweight = 0;
	for(int i = 1; i<narg; ++i) {
		if(argh[i][0]!='-' || argh[i][2]!=0)
			usage();
		switch(tolower(argh[i][1])) {
			case 'v':
				if(++i==narg)
					usage();
				V = atoi(argh[i]);
				break;
			case 'e':
				if(++i==narg)
					usage();
				E = atoi(argh[i]);
				break;
			case 'c':
				colors = true;
				break;
			case 'w':
				if(++i==narg)
					usage();
				maxweight = atoi(argh[i]);
				break;
			default:
				usage();
		}
	}
	unsigned seed = (unsigned)time(NULL);
	srand(seed);

	StrGraph g;
	char buf[20];
	sprintf(buf,"%d",seed);
	gd<StrAttrs>(&g)["seed"] = buf;
	vector<StrGraph::Node*> ez;
	ez.resize(V);
	int v = V;
	while(v--) {
		char name[10];
		sprintf(name,"%d",v);
		ez[v] = g.create_node(name);
	}
	while(E--) {
		int t,h;
		do 
			t = rand()%V, h = rand()%V;
		while(t==h || g.find_edge(ez[t],ez[h]) || g.find_edge(ez[h],ez[t])); // play to dynagraph's weaknesses
		char name[10];
		sprintf(name,"%d",E);
		StrGraph::Edge *e = g.create_edge(ez[t],ez[h],name).first;
		if(colors) {
			char *color=0;
			switch(rand()%3) {
				case 0: color = "red";
					break;
				case 1: color = "yellow";
					break;
				case 2: color = "blue";
					break;
			}
			gd<StrAttrs>(e)["color"] = color;
		}
		char buf[10];
		if(maxweight)
			gd<StrAttrs>(e)["weight"] = itoa(rand()%maxweight,buf,10);
	}
	emitGraph(cout,&g);
	return 0;
}
