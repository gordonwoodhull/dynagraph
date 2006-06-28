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
#include <string.h>

#include "common/ag2str.h"
#include "common/emitGraph.h"
#include "common/stringsIn.h"
#include "common/stringsOut.h"
#include "incrface/incrout.h"
#include "incrface/incrparse.h"

#include "dynadag/DynaDAGLayout.h"
#include "fdp/FDPLayout.h"
#include "common/GeneralLayout.h"

#include "incrface/createStringHandlers.h"

#include "dynagraph.version.h"

using namespace std;
using namespace Dynagraph;

using DynaDAG::DynaDAGLayout;
using FDP::FDPLayout;

Transform *g_transform;
bool g_useDotDefaults;
char *g_outdot=0;
int g_count=1;
StrAttrs g_defaultGraphAttrs;
bool g_xeptOut=false;

struct CouldntOpen {};

#ifndef DYNAGRAPH_NO_THREADS
#define TEXT_OUTPUT_MUTEX
#endif

#ifdef TEXT_OUTPUT_MUTEX
boost::mutex g_outputMutex;
#define LOCK_OUTPUT() boost::mutex::scoped_lock lock(g_outputMutex)
#else
#define LOCK_OUTPUT()
#endif

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
	TextChangeOutput(ChangingGraph<Graph> *world) : LinkedChangeProcessor<Graph>(world) {}
	// ChangeProcessor
	void Process() {
		LOCK_OUTPUT();
		emitChanges(cout,this->world_->Q_,gd<Name>(&this->world_->whole_).c_str());
		doOutdot(&this->world_->current_);
	}
};
template<typename Graph>
struct TextWatcherOutput : IncrViewWatcher<Graph> {
	// IncrViewWatcher
	void IncrOpen(ChangeQueue<Graph> &Q) {
		LOCK_OUTPUT();
		cout << "open graph " << gd<Name>(Q.whole) << " " << gd<StrAttrs>(Q.whole) << endl;
		igd<StrAttrChanges>(Q.ModGraph()).clear();
	}
	void IncrClose(ChangeQueue<Graph> &Q) {
		LOCK_OUTPUT();
		cout << "close graph " << gd<Name>(Q.whole) << endl;
	}
	void FulfilGraph(Graph *g) {
		LOCK_OUTPUT();
		cout << "fulfil graph " << gd<Name>(g) << endl;
		emitGraph(cout,g);
	}
	void FulfilNode(typename Graph::Node *n) {
		LOCK_OUTPUT();
		cout << "fulfil node " << gd<Name>(n->g) << " " << gd<Name>(n) << " " << gd<StrAttrs>(n) << endl;
	}
	void FulfilEdge(typename Graph::Edge *e) {
		LOCK_OUTPUT();
		cout << "fulfil edge " << gd<Name>(e->g) << " " << gd<Name>(e) << " " << gd<StrAttrs>(e) << endl;
	}
};
template<typename Layout>
IncrLangEvents *createHandlers(DString gname,const StrAttrs &attrs) {
	if(attrs.look("superengines")) {
		ChangingGraph<GeneralLayout> *world = new ChangingGraph<GeneralLayout>;
		return createStringHandlers<GeneralLayout>(world,WorldGuts<Layout>(attrs.look("superengines"),attrs.look("engines")),
			new TextWatcherOutput<GeneralLayout>,0,new TextChangeOutput<GeneralLayout>(world),
			gname,attrs,g_transform,g_useDotDefaults);
	}
	else {
		ChangingGraph<Layout> *world = new ChangingGraph<Layout>;
		return createStringHandlers<Layout>(world,SimpleGuts<Layout>(attrs.look("engines")),
			new TextWatcherOutput<Layout>,0,new TextChangeOutput<Layout>(world),
			gname,attrs,g_transform,g_useDotDefaults);
	}
}

struct IncrCalledBack : IncrCallbacks {
    IncrCalledBack() {
        g_incrCallback = this;
    }
    ~IncrCalledBack() {
		g_incrCallback = 0;
    }
    IncrLangEvents *incr_cb_create_handler(Name name,StrAttrs &attrs) {
		attrs = g_defaultGraphAttrs+attrs;
		DString type = attrs.look("type"),
			&engines = attrs["engines"],
			&superengines=attrs["superengines"];
		if(!type) {
			if(!engines) {
				if(superengines.find("shapegen",0)!=DString::npos)
					engines = "dynadag";
				else
					engines = "shapegen,dynadag";
			}
			DString::size_type ddpos = engines.find("dynadag",0);
			if(ddpos!=DString::npos) 
				if(engines.find("nsranker",0)==DString::npos && superengines.find("nsranker",0)==DString::npos)
					if(superengines) {
						string s = superengines.c_str(); // ick
						s += ",nsranker";
						superengines = s.c_str();
					}
					else {
						string s = engines.c_str();
						s.insert(ddpos,"nsranker,");
						engines = s.c_str();
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
			ret = createHandlers<DynaDAGLayout>(name,attrs);
		else if(type=="fdp")
			ret = createHandlers<FDPLayout>(name,attrs);
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
	report(r_cmdline,
		"dynagraph arguments:\n"
		"   -v (--version) print version information only\n"
		"	-h (-?) print this help\n"
		"   -d use dot-compatible coordinates (position in points, node size in inches)\n"
		"   -i filename input .dot file (static layout)\n"
		"   -s filename input .incr file (incrface dynamic layout)\n"
		"   -oN filename write stream N to filename\n"
		"   -oL filename output layout steps to filename{step}.dot\n"
		"   -raN report on a to stream N\n"
		"	-a attr=value set default graph attribute\n"
		"   -x break on any exception\n");
	for(int i = 0;i<g_nreports;++i)
		report(r_cmdline,"      %c %s\n",g_reports[i].c,g_reports[i].desc);
}
void print_report_syntax() {
	report(r_error,
		"-o: specify output filename\n"
		"   -oN filename, N is [0-9], a file # specified in -r\n"
		"   -oL filename output layouts to filename1.dot,filename2.dot,...\n");
}
void print_attribute_syntax() {
	report(r_error,
		"-a: specify default graph attribute\n"
		"   -a attr=value\n"
		"   effectively attr=value is prepended onto every 'open graph' attribute list\n");
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
			if(i==argc-1 || args[i][2]) {
				report(r_error,
					"-s: specify input incrface script\n"
					"   -s filename: use script in filename (instead of stdin)\n");
				return 1;
			}
			if(!(incr_yyin = fopen(args[++i],"r"))) {
				report(r_error,"could not open input script %s\n",args[i]);
				return 1;
			}
			//setvbuf(incr_yyin,0,_IONBF,0);
			break;
		case 'r': {// reports
			char last = args[i][strlen(args[i])-1];
			int o = isdigit(last)?(last-'0'):-1;
			for(int j = 2; args[i][j] && isalpha(args[i][j]); ++j) {
				pair<bool,enum reportTypes> val = findSwitchVal(g_reports,g_nreports,args[i][j]);
				if(!val.first) {
					report(r_error,"-r: generate report\n"
								   "   report code '%c' not recognized\n",args[i][j]);
					return 1;
				}
				reports[val.second] = o;
			}
			break;
				  }
		case 'o': // output files
			if(i==argc-1 || !args[i][2]) {
				print_report_syntax();
				return 1;
			}
			if(toupper(args[i][2])=='L') {
				g_outdot = args[++i];
				break;
			}
			else if(!isdigit(args[i][2])) {
				print_report_syntax();
				return 1;
			} else {
				int n = args[i][2]-'0';
				outfile[n] = fopen(args[++i],"w");
				if(!outfile[n]) {
					report(r_error,"-o error: couldn't open file %s for writing\n",args[i]+3);
					return 1;
				}
			}
			break;
		case 'd': // dot-compatible coords
			g_transform  = &g_dotRatios;
			g_useDotDefaults = true;
			break;
		case 'a': // set default attribute
			if(i==argc-1 || args[i][2]) {
				print_attribute_syntax();
				return 1;
			}
			else {
				char *eq = strchr(args[++i],'=');
				DString attr,value;
				if(eq) {
					attr.assign(args[i],eq-args[i]);
					value.assign(eq+1);
				}
				else
					attr.assign(args[i]);
				g_defaultGraphAttrs[attr] = value;
			}
			break;
		case 'x':
			g_xeptOut = true;
			break;
		case 'h':
		case '?':
			print_version();
			print_help();
			return 0;
		case 'v':
			print_version();
			return 0;
		case '-':
			if(!strcmp(args[i]+2,"version")) {
				print_version();
				return 0;
			}
			else if(!strcmp(args[i]+2,"help")) {
				print_version();
				print_help();
				return 0;
			}
			//else fallthru
		default:
			report(r_error,"command not recognized: %s\n",args[i]);
			print_version();
			print_help();
			return 1;
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
		catch(Assertion sert) {
			fprintf(stdout,"message \"(exception) Assertion: %s; %s, %d\"\n",sert.expr,sert.file,sert.line);
			if(g_xeptOut)
				throw;
			if(sert.fatal)
				exit(23);
		}
		catch(DGException2 dgx) {
			fprintf(stdout,"message \"(exception) %s: %s\"\n",dgx.exceptype.c_str(),dgx.param.c_str());
			if(g_xeptOut)
				throw;
			if(dgx.fatal)
				break;
		}
		catch(DGException dgx) {
			fprintf(stdout,"message \"(exception) %s\"\n",dgx.exceptype.c_str());
			if(g_xeptOut)
				throw;
			if(dgx.fatal)
				break;
		}
	}
	incr_shutdown();
	fprintf(stdout,"message \"dynagraph closing\"\n");
	return 0;
}
