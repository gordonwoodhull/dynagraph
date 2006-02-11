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
#include "incrface/DynaView.h"
#include "dynadag/DynaDAG.h"
#include "incrface/incrout.h"
#include "incrface/incrparse.h"
#include "TestTraversals.h"

using namespace std;
using DynaDAG::DDChangeQueue;
using DynaDAG::DynaDAGLayout;
using FDP::FDPLayout;
#define CATCH_XEP

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

template<typename Layout>
struct TextView : DynaView<Layout> {
	void IncrHappened() {
		DynaView<Layout> &dad = *this;
		emitChanges(cout,dad.Q,gd<Name>(&dad.layout).c_str());
		dad.Q.Okay(true);
		doOutdot(&dad.current);
	}
	void IncrNewNode(typename Layout::Node *n) {}
	void IncrNewEdge(typename Layout::Edge *e) {}
	TextView(Name name) : DynaView<Layout>(name,g_transform,g_useDotDefaults) {}
};
struct IncrCalledBack : IncrCallbacks {
    IncrCalledBack() {
        g_incrCallback = this;
    }
    ~IncrCalledBack() {
		g_incrCallback = 0;
    }
    IncrLangEvents *incr_cb_create_handler(Name name,const StrAttrs &attrs) {
		StrAttrs::const_iterator ai = attrs.find("type");
		DString type,setEngs;
		if(ai!=attrs.end()) 
			type = ai->second;
		else {
			StrAttrs::const_iterator ai = attrs.find("engines");
			DString engines;
			if(ai!=attrs.end())
				engines = ai->second;
			else
				setEngs = engines = "shapegen,dynadag,labels";
			if(engines.find("dynadag",0)!=DString::npos)
				type = "dynadag";
			else if(engines.find("fdp",0)!=DString::npos)
				type = "fdp";
			else
				throw DGException2("could not deduce graph type from engine list",engines.c_str());
		}
		IncrLangEvents *ret;
		if(type=="dynadag") {
			TextView<DynaDAGLayout> *view = new TextView<DynaDAGLayout>(name);
			if(setEngs)
				gd<StrAttrs2>(&view->layout).put("engines",setEngs);
			ret = view;
		}
		else if(type=="fdp") {
			TextView<FDPLayout> *view = new TextView<FDPLayout>(name);
			if(setEngs)
				gd<StrAttrs2>(&view->layout).put("engines",setEngs);
			ret = view;
		}
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
};
IncrCalledBack g_incrPhone;
void optimizeAll(DDChangeQueue &Q,DynaDAGLayout *current) {
	for(DynaDAGLayout::node_iter ni = current->nodes().begin(); ni!=current->nodes().end(); ++ni) {
		gd<NodeGeom>(*ni).pos.invalidate();
		Q.ModNode(*ni,DG_UPD_MOVE);
	}
}
void dumpQueue(DDChangeQueue &Q) {
	if(reportEnabled(r_dumpQueue)) {
		DynaDAGLayout::node_iter ni;
		DynaDAGLayout::graphedge_iter ei;
		if(Q.insN.nodes().size() || Q.insE.nodes().size()) {
			report(r_dumpQueue,"insert\n");
			for(ni = Q.insN.nodes().begin(); ni!=Q.insN.nodes().end(); ++ni)
				report(r_dumpQueue,"\t%s\n",gd<Name>(*ni).c_str());
			for(ei = Q.insE.edges().begin(); ei!=Q.insE.edges().end(); ++ei)
				report(r_dumpQueue,"\t%s->%s\n",gd<Name>((*ei)->tail).c_str(),gd<Name>((*ei)->head).c_str());
		}
	}
}
template<typename V>
struct switchval {
	char c;
	V v;
	char *desc;
};
enum travmode {unspecified,dynamic,nodes_first,wandering,all,depth_first,breadth_first,random_insert,create};
switchval<enum travmode> g_traversals[] = {
	{'n',nodes_first,"nodes then edges"},
	{'w',wandering,"foolish wandering"},
	{'a',all,"all at once"},
	{'d',depth_first,"depth first"},
	{'b',breadth_first,"breadth first"},
	{'r',random_insert,"random"}
};
int g_ntraversals = sizeof(g_traversals)/sizeof(switchval<enum travmode>);
template<typename Layout>
Stepper<Layout> *createStepper(enum travmode type,Layout *l,int N,double reconnectProbability) {
	switch(type) {
  case nodes_first:
	  return new NodesFirst<Layout>(l);
  case wandering:
	  return new Wandering<Layout>(l);
  case all:
	  return new Batch<Layout>(l);
  case depth_first:
	  return new TraversalStep<Layout,DFS<Layout> >(l);
  case breadth_first:
	  return new TraversalStep<Layout,BFS<Layout> >(l);
  case create:
	  return new RandomDrawer<Layout>(l,N,reconnectProbability);
  case random_insert:
	  return new RandomTraversal<Layout>(l);
  default:
	  return 0;
	}
}
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
int main(int argc, char *args[]) {
	enum travmode traversal_mode = unspecified;
	enableReport(r_error,stderr);
	enableReport(r_cmdline,stdout);
	loops.sep = ',';
	bool forceRelayout = false;
	char *dotfile = 0;
	FILE *outfile[10];
	int i,generateN=-1;
	double reconnectProbability = 0.05;
	timer.Start();
	for(i = 0;i<10;++i) outfile[i] = 0;
	map<int,int> reports;
	for(i = 1; i<argc; ++i) {
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
	case 't': { // traversal
		if(traversal_mode!=unspecified) {
			report(r_error,"only one traversal mode allowed (-t or -c)\n");
			return 1;
		}
		pair<bool,enum travmode> val = findSwitchVal(g_traversals,g_ntraversals,args[i][2]);
		if(!val.first) {
			report(r_error,"-t%c not recognized. quitting.\n",args[i][2]);
			return 1;
		}
		traversal_mode = val.second;
		break;
			  }
	case 'c': // create
		if(traversal_mode!=unspecified) {
			report(r_error,"only one traversal mode allowed (-t or -c)\n");
			return 1;
		}
		traversal_mode = create;
		if(args[i][2])
			generateN = atoi(args[i]+2);
		else
			generateN = 200;
		if(generateN==0)
			generateN = 200;
		if(char *comma = strchr(args[i]+2,',')) {
			int prob = atoi(comma+1);
			reconnectProbability = prob/100.0;
		}
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
	case 'f': // force full relayout every step
		forceRelayout = true;
		break;
	case 'd': // dot-compatible coords
		g_transform  = &g_dotRatios;
        g_useDotDefaults = true;
		break;
	default:
		report(r_error,"switch -%c not recognized\n",args[i][1]);
	case 'h':
	case '?':
		report(r_cmdline,"dynagraph arguments:\n"
			"   -d use dot-compatible coordinates (position in points, node size in inches)\n"
			"   -i filename input .dot file\n"
			"   -ta choose traversal method\n");
		{
			int i;
			for(i = 0; i<g_ntraversals;++i)
				report(r_cmdline,"      %c %s\n",g_traversals[i].c,g_traversals[i].desc);
			report(r_cmdline,"   -cN create edges randomly, N steps; 95%% leaves\n"
				"   -raN report on a to stream N\n");
			for(i = 0;i<g_nreports;++i)
				report(r_cmdline,"      %c %s\n",g_reports[i].c,g_reports[i].desc);
		}
		report(r_cmdline,"   -oN filename write stream N to filename\n"
			"   -oL filename output layout steps to filename{step}.dot\n"
			"   -f restart server (get new batch layout) every step)\n");
		return 1;
		}
	}
#ifdef CATCH_XEP
	try {
#endif
		if(dotfile && traversal_mode==create) {
			report(r_error,"-i input file is not compatible with -c create\n");
			return 1;
		}
		if(!dotfile && traversal_mode!=create) {
			if(traversal_mode==unspecified) {
				traversal_mode = dynamic;
				//setvbuf(stdin,(char*)malloc(30000),_IOFBF,30000);
				setvbuf(stdin,0,_IONBF,0);
				setvbuf(stdout,0,_IONBF,0);
				setvbuf(stderr,0,_IONBF,0);
				//setvbuf(stdout,(char*)malloc(30000),_IOFBF,30000);
				/*
				setvbuf(stdin,0,_IOLBF,4196);
				setvbuf(stdout,0,_IOLBF,4196);
				*/
			}
			else {
				report(r_error,"specify a .dot file\n");
				return 1;
			}
		}
		if(traversal_mode==unspecified)
			traversal_mode = all;
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
		if(traversal_mode==dynamic) {
			while(1) {
				try {
					incr_yyparse(); 
					break; // end of stream
				}
				catch(IncrError ie) { // parser error
					fprintf(stdout,"message \"%s\"\n",ie.descrip.c_str());
				}
				catch(DGException2 dgx) {
					fprintf(stdout,"message \"%s: %s\"\n",dgx.exceptype,dgx.param);
					if(dgx.fatal)
						break;
				}
				catch(DGException dgx) {
					fprintf(stdout,"message \"exception: %s\"\n",dgx.exceptype);
					if(dgx.fatal)
						break;
				}
			}
			fprintf(stdout,"message \"dynagraph closing\"\n");
			return 0;
		}

		timer.Now(r_cmdline,"DynaDAG: laying out %s\n",dotfile?dotfile:"randomly generated graph");
		report(r_cmdline,"traversal: %s.  layout: %s\n",findDescription(g_traversals,g_ntraversals,traversal_mode).second,
			forceRelayout?"batch":"incremental");
		DynaDAGLayout layout,
			current(&layout);
		Server<DynaDAGLayout> &eng = *createLayoutServer<DynaDAGLayout>(&layout,&current);
		timer.Now(r_progress,"created engine.\n");
		DDChangeQueue Q(&layout,&current);
		if(dotfile) {
			FILE *f = fopen(dotfile,"r");
			if(!f) {
				report(r_error,"could not open file.\n");
				return 1;
			}
			Agraph_t *g = agread(f,0);
			fclose(f);
			if(!g) {
				report(r_error,"could not read graph.\n");
				return 1;
			}
			StrGraph *sg = ag2str(g);
			if(!sg) {
				report(r_error,"could not convert.\n");
				return 1;
			}
			timer.Now(r_progress,"read dot file...\n");
			applyStrGraph(g_transform,g_useDotDefaults,sg,&layout,&layout);
			timer.Now(r_progress,"translated...\n");
			/*
			// give nodes some area
			for(DynaDAGLayout::node_iter ni = layout.nodes().begin(); ni!=layout.nodes().end(); ++ni)
			init_node(*ni);
			*/
			timer.Now(r_progress,"added node boundaries.\n");
		}
		gd<Name>(&layout) = "unnamed";

		Stepper<DynaDAGLayout> *stepper=createStepper<DynaDAGLayout>(traversal_mode,&layout,generateN,reconnectProbability);
		double startLayout = timer.Now(r_progress,"Starting layout\n");
		try {
			while(!stepper->Done()) {
				loops.Start(r_timing);
				loops.Start(r_readability);
				loops.Start(r_stability);
				loops.Start(r_dynadag);
				loops.Start(r_crossopt);

				loops.Field(-1,"step",g_count);
				timer.Now(r_progress,"Step %d\n",g_count);
				if(forceRelayout) { // erase everything and start from scratch
					shush(true);
					assert(Q.Empty());
					for(DynaDAGLayout::node_iter ni = current.nodes().begin(); ni!=current.nodes().end(); ++ni)
						Q.DelNode(*ni); // edges inducted
					eng.Process(Q);
					assert(current.nodes().empty());
					Q.Okay(false);
					Q.insN = layout;
					for(DynaDAGLayout::graphedge_iter ei = layout.edges().begin(); ei!=layout.edges().end(); ++ei)
						Q.InsEdge(*ei);
					shush(false);
				}
				stepper->Step(Q);
				if(Q.Empty()) {
					loops.Cancel();
					break;
				}
				dumpQueue(Q);
				try {
					eng.Process(Q);
				}
				catch(DynaDAG::BackForth bf) {
					loops.Cancel();
					Q.Okay(true);
					layout.erase(bf.e);
					continue;
				}
				loops.Field(r_stability,"percent of nodes moved",double(Q.modN.nodes().size())/double(current.nodes().size()));
				stringsOut(g_transform,Q);
				Q.Okay(true);
				if(reportEnabled(r_readability)) {
					Bounds b = gd<GraphGeom>(&layout).bounds;
					if(!b.valid)
						b.l = b.t = b.r = b.b = 0.0;
					loops.Field(r_readability,"left side",b.l);
					loops.Field(r_readability,"top side",b.t);
					loops.Field(r_readability,"right side",b.r);
					loops.Field(r_readability,"bottom side",b.b);
					loops.Field(r_readability,"area",b.Width()*b.Height());

				}

				loops.Finish(r_timing);
				loops.Finish(r_readability);
				loops.Finish(r_stability);
				loops.Finish(r_dynadag);
				loops.Finish(r_crossopt);

				doOutdot(&layout);
			}
		}
		catch(DDChangeQueue::EndnodesNotInserted) {
			report(r_error,"bad traversal!\n");
			return 1;
		}
		double endLayout = timer.Elapsed(r_progress,startLayout,"start of layout\n");
		report(r_progress,"average step time: %8.4f\n",(endLayout-startLayout)/double(g_count));

		timer.Now(r_cmdline,"writing files...\n");
#ifdef CATCH_XEP
	}
	catch(GeomParseError pe) {
		fprintf(stderr,"geometry parse error: \"%s\" not expected\n",pe.val.c_str());
	}
	catch(Assertion sert) {
		fprintf(stderr,"assertion in file %s, line %d\n",sert.file,sert.line);
	}
	catch(DGException dgx) {
		fprintf(stderr,"dynagraph exception: %s\n",dgx.exceptype);
	}
	catch(...) {
		fprintf(stderr,"argh!  unknown exception!\n");
	}
#endif
	for(i=0;i<10;++i)
		if(outfile[i])
			fclose(outfile[i]);
	return 0;
}
