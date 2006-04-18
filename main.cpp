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


#include <stdio.h>
#include <fstream>
#include "common/ag2str.h"
#include "common/emitGraph.h"
#include "common/stringsIn.h"
#include "common/stringsOut.h"
#include "incrface/incrout.h"
#include "incrface/incrparse.h"

#include "dynadag/DynaDAGLayout.h"
#include "fdp/FDPLayout.h"
#include "common/GeneralLayout.h"

#include "incrface/createStrWorldAndHandler.h"

#include "dynagraph.version.h"

using namespace std;
using namespace Dynagraph;

using DynaDAG::DynaDAGLayout;
using FDP::FDPLayout;

Transform *g_transform;
bool g_useDotDefaults;
char *g_outdot=0;
int g_count=1;

struct CouldntOpen {};
template<typename Layout>
void doOutdot(Layout *l) {
	if(g_outdot) {
		char filename[100];
		sprintf(filename,"%s%d.dot",g_outdot,g_count);
		fstream f(filename,fstream::out);
		if(f.fail()) {
			report(r_error,"couldn't write to %s\n",filename);
			throw CouldntOpen();
		}
		emitGraph(f,l);
		++g_count;
	}
}

template<typename Graph>
struct TextChangeOutput : LinkedChangeProcessor<Graph> {
	// ChangeProcessor
	void Process(ChangeQueue<Graph> &Q) {
		emitChanges(cout,Q,gd<Name>(Q.whole).c_str());
		Q.Execute(true);
		ModifyFlags(Q) = 0;
		doOutdot(Q.current);
	}
};
template<typename Graph>
struct TextWatcherOutput : IncrViewWatcher<Graph> {
	// IncrViewWatcher
	void IncrOpen(ChangeQueue<Graph> &Q) {
		cout << "open graph " << gd<Name>(Q.whole) << " " << gd<StrAttrs>(Q.whole) << endl;
		igd<StrAttrChanges>(Q.ModGraph()).clear();
	}
	void IncrClose(ChangeQueue<Graph> &Q) {
		cout << "close graph " << gd<Name>(Q.whole) << endl;
	}
	void FulfilGraph(Graph *g) {
		cout << "fulfil graph " << gd<Name>(g) << endl;
		emitGraph(cout,g);
	}
	void FulfilNode(typename Graph::Node *n) {
		cout << "fulfil node " << gd<Name>(n->g) << " " << gd<Name>(n) << " " << gd<StrAttrs>(n) << endl;
	}
	void FulfilEdge(typename Graph::Edge *e) {
		cout << "fulfil edge " << gd<Name>(e->g) << " " << gd<Name>(e) << " " << gd<StrAttrs>(e) << endl;
	}
};
template<typename Layout>
IncrLangEvents *createHandlers(DString name,DString superengines,DString engines) {
	if(superengines) 
		return createStrWorldAndHandler<GeneralLayout>(WorldGuts<Layout>(superengines,engines),
			new TextWatcherOutput<GeneralLayout>,0,new TextChangeOutput<GeneralLayout>,
			g_transform,g_useDotDefaults);
	else 
		return createStrWorldAndHandler<Layout>(SimpleGuts<Layout>(engines),
			new TextWatcherOutput<Layout>,0,new TextChangeOutput<Layout>,
			g_transform,g_useDotDefaults);
}

struct IncrCalledBack : IncrCallbacks {
    IncrCalledBack() {
        g_incrCallback = this;
    }
    ~IncrCalledBack() {
		g_incrCallback = 0;
    }
    IncrLangEvents *incr_cb_create_handler(Name name,const StrAttrs &attrs) {
		StrAttrs::const_iterator ai = attrs.find("type");
		DString type,engines,superengines=attrs.look("superengines");
		bool setEngs=false;
		if(ai!=attrs.end()) 
			type = ai->second;
		else {
			engines = attrs.look("engines");
			if(!engines) {
				if(superengines.find("shapegen",0)!=DString::npos)
					engines = "dynadag";
				else
					engines = "shapegen,dynadag";
				setEngs = true;
			}
			if(engines.find("dynadag",0)!=DString::npos)
				type = "dynadag";
			else if(engines.find("fdp",0)!=DString::npos)
				type = "fdp";
			else
				throw DGException2("could not deduce graph type from engine list",engines);
		}
		IncrLangEvents *ret;
		if(type=="dynadag") 
			ret = createHandlers<DynaDAGLayout>(name,superengines,engines);
		else if(type=="fdp")
			ret = createHandlers<FDPLayout>(name,superengines,engines);
    	return ret;
	}
    void incr_cb_destroy_handler(IncrLangEvents *h) {
        delete h;
    }
    // echo all fulfils (prob from another server not client!)
    void incr_cb_fulfil_graph(DString name,StrGraph *sg) {
        cout << "fulfil graph " << name << endl;
        cout << *sg;
	}
    void incr_cb_fulfil_node(DString graphname,DString nodename,const StrAttrs &attrs) {
        cout << "fulfil node " << graphname << ' ' << nodename << attrs << endl;
	}
    void incr_cb_fulfil_edge(DString graphname,DString edgename,const StrAttrs &attrs) {
        cout << "fulfil edge " << graphname << ' ' << edgename << attrs << endl;
	}
    void incr_cb_message(const char *msg) {
        // pass through
        cout << "message \"" << msg << '"' << endl;
	}
} g_incrPhone;
template<typename V>
struct switchval {
	char c;
	V v;
	char *desc;
};
switchval<enum reportTypes> g_reports[] = {
	{'c',r_crossopt,"crossopt"},
	{'t',r_timing,"timing breakdown"},
	{'d',r_dynadag,"dynadag tallies"},
	{'g',r_modelDump,"dump graph"},
	{'q',r_dumpQueue,"dump input queue before each cycle"},
	{'r',r_readability,"readability"},
	{'s',r_stability,"stability"},
	{'p',r_progress,"progress"},
	{'b',r_bug,"bug of the day"}
};
int g_nreports = sizeof(g_reports)/sizeof(switchval<enum reportTypes>);
template<typename V>
pair<bool,V> findSwitchVal(switchval<V> *array,int n,char c) {
	for(int i =0;i<n;++i)
		if(array[i].c==c)
			return make_pair(true,array[i].v);
	return make_pair(false,V());
}
template<typename V>
pair<bool,char*> findDescription(switchval<V> *array,int n,V v) {
	for(int i=0; i<n; ++i)
		if(array[i].v==v)
			return make_pair(true,array[i].desc);
	return make_pair(false,(char*)0);
}
void print_version() {
	report(r_cmdline,"Dynagraph version %s\n",DYNAGRAPH_VERSION_DOTS_QUOTED);
}
void print_help() {
	report(r_cmdline,"dynagraph arguments:\n"
		"   -d use dot-compatible coordinates (position in points, node size in inches)\n"
		"   -i filename input .dot file\n"
		"   -oN filename write stream N to filename\n"
		"   -oL filename output layout steps to filename{step}.dot\n");
}


int main(int argc, char *args[]) {
	enableReport(r_error,stderr);
	enableReport(r_cmdline,stdout);
	loops.sep = ',';
	char *dotfile = 0;
	FILE *outfile[10];
	timer.Start();
	for(int i = 0;i<10;++i) outfile[i] = 0;
	map<int,int> reports;
	for(int i = 1; i<argc; ++i) {
		if(args[i][0]!='-') {
			report(r_error,"not a valid command: %s\n",args[i]);
			return 1;
		}
		switch(args[i][1]) {
		case 'i': // input file (otherwise dynamic)
			if(i==argc-1) {
				report(r_error,"-i must be followed by filename\n");
				return 1;
			}
			if(args[i][2]) {
				report(r_error,"syntax: -i filename\n");
				return 1;
			}
			dotfile = args[++i];
			break;
		case 's': // dynamic script
			if(i==argc-1) {
				report(r_error,"-s must be followed by filename\n");
				return 1;
			}
			if(args[i][2]) {
				report(r_error,"syntax: -s filename\n");
				return 1;
			}
			if(!(incr_yyin = fopen(args[++i],"r"))) {
				report(r_error,"could not open input script %s\n",args[i]);
				return 1;
			}
			//setvbuf(incr_yyin,0,_IONBF,0);
			break;
		case 'r': {// reports
			int o;
			char last = args[i][strlen(args[i])-1];
			o = isdigit(last)?(last-'0'):-1;
			for(int j = 2; args[i][j] && isalpha(args[i][j]); ++j) {
				pair<bool,enum reportTypes> val = findSwitchVal(g_reports,g_nreports,args[i][j]);
				if(!val.first) {
					report(r_error,"-r: generate report\n\t'%c' not recognized\n",args[i][j]);
					return 1;
				}
				reports[val.second] = o;
			}
			break;
				  }
		case 'o': {// output files
			if(i==argc-1) {
				report(r_error,"-o must be followed by filename\n");
				return 1;
			}
			if(!args[i][2]) {
				report(r_error,"syntax: -o[0..9L] filename\n");
				return 1;
			}
			if(toupper(args[i][2])=='L') {
				g_outdot = args[++i];
				break;
			}
			else if(!isdigit(args[i][2])) {
				report(r_error,"-o: output file\n"
							   "\tsyntax: -oN filename, N is [0-9], a file # specified in -r\n"
							   "\t\tOR -oL filename output layouts to filename1.dot,filename2.dot,...\n");
				return 1;
			}
			int n = args[i][2]-'0';
			outfile[n] = fopen(args[++i],"w");
			if(!outfile[n]) {
				report(r_error,"-o error: couldn't open file %s for writing\n",args[i]+3);
				return 1;
			}
			break;
				  }
		case 'd': // dot-compatible coords
			g_transform  = &g_dotRatios;
			g_useDotDefaults = true;
			break;
		case 'v':
			print_version();
			return 0;
		default:
			report(r_error,"switch -%c not recognized\n",args[i][1]);
			print_help();
			return 1;
		case 'h':
		case '?':
			print_version();
			print_help();
			return 0;
		}
	}
	setvbuf(stdin,0,_IONBF,0);
	setvbuf(stdout,0,_IONBF,0);
	setvbuf(stderr,0,_IONBF,0);
	for(map<int,int>::iterator ri = reports.begin(); ri!=reports.end(); ++ri) {
		FILE *f = ri->second==-1?stderr:outfile[ri->second];
		if(!f) {
			char c = '0' + ri->second;
			char outfilename[300];
			sprintf(outfilename,"%s.dd%c",dotfile,c);
			report(r_cmdline,"outfile %c not specified; opening %s\n",c,outfilename);
			if(!(outfile[ri->second] = fopen(outfilename,"w"))) {
				report(r_error,"couldn't open %s for writing\n",outfilename);
				return 1;
			}
		}
		enableReport(ri->first,f);
	}
	if(!g_transform)
		g_transform = new Transform(Coord(1,1),Coord(1,1));
	while(1) {
		try {
			incr_yyparse(); 
			break; // end of stream
		}
		catch(DGException2 dgx) {
			fprintf(stdout,"message \"%s: %s\"\n",dgx.exceptype.c_str(),dgx.param.c_str());
			if(dgx.fatal)
				break;
		}
		catch(DGException dgx) {
			fprintf(stdout,"message \"exception: %s\"\n",dgx.exceptype.c_str());
			if(dgx.fatal)
				break;
		}
	}
	fprintf(stdout,"message \"dynagraph closing\"\n");
	return 0;
}
