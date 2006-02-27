#include "common/Dynagraph.h"
#include "incrface/DynaView.h"
#include "incrface/incrparse.h"
#include <iostream>

// This example shows the many ways to use Dynagraph via the incrface library
// If it looks complicated, note that probably only one of these ways
// will be relevant to your application

// The cleaner way to handle lifetimes of graphs is to let incrface do it
// (of course! "let me do that for you" is the curse of computer science, religion, and politics...)
// but many clients will want control, so the library uses create and destroy callbacks
// this example demonstrates both paradigms; 
// #define CREATE_YOURSELF 
// selects the messier but more powerful client-control way

using namespace std;

// determines how to interpret string coordinates.  specify not to change them:
Transform *g_transform = new Transform(Coord(1,1),Coord(1,1));
// whether to use the same resolution,separation,defaultsize as dot
bool g_useDotDefaults = false;

struct EgzamplView : DynaView {
	void IncrHappened() { 
		// obviously the output is not essential
		// often this method will need to map Layout nodes & edges to application objects
		// sorry, there is no better way than to use a map<>
		for(Layout::node_iter ni = Q.insN.nodes().begin(); ni!=Q.insN.nodes().end(); ++ni)
			cout << "node " << gd<Name>(*ni) << " inserted at " << gd<NodeGeom>(*ni).pos << endl;
		for(Layout::graphedge_iter ei = Q.insE.edges().begin(); ei!=Q.insE.edges().end(); ++ei)
			cout << "edge " << gd<Name>(*ei) << " inserted at " << gd<EdgeGeom>(*ei).pos << endl;
		for(Layout::node_iter ni = Q.modN.nodes().begin(); ni!=Q.modN.nodes().end(); ++ni)
			cout << "node " << gd<Name>(*ni) << " moved to " << gd<NodeGeom>(*ni).pos << endl;
		for(Layout::graphedge_iter ei = Q.modE.edges().begin(); ei!=Q.modE.edges().end(); ++ei)
			cout << "edge " << gd<Name>(*ei) << " moved to " << gd<EdgeGeom>(*ei).pos << endl;
		for(Layout::node_iter ni = Q.delN.nodes().begin(); ni!=Q.delN.nodes().end(); ++ni)
			cout << "node " << gd<Name>(*ni) << " deleted" << endl;
		for(Layout::graphedge_iter ei = Q.delE.edges().begin(); ei!=Q.delE.edges().end(); ++ei)
			cout << "edge " << gd<Name>(*ei) << " deleted" << endl;
		// IMPORTANT: Okay must be called within IncrHappened
		Q.Okay(true);
	}
	void IncrNewNode(Layout::Node *n) {}
	void IncrNewEdge(Layout::Edge *e) {}
	EgzamplView(Name name) : DynaView(name,g_transform,g_useDotDefaults) {
#ifdef CREATE_YOURSELF 
		// in DIY model, constructor always gets called before incrface "open graph" command
		incr_set_handler(gd<Name>(&layout),this);
#endif
		// this flag is necessary if the application is going to call incr_ev_open_graph
		// or createServer and then also process an "open graph" incrface command
		// as this example does and e.g. when receiving from graphsearch
        m_allowOneReopen = true;
	}
};
// incrface needs g_incrCallback to be filled; a global with initializer in the 
// constructor is a simple way.
struct IncrCalledBack : IncrCallbacks {
    IncrCalledBack() {
        g_incrCallback = this;
    }
    IncrLangEvents *incr_cb_create_handler(Name name,const StrAttrs &attrs) {
        DinoMachine::Node *dn = g_dinoMachine.ndict[name];
#ifdef CREATE_YOURSELF
		// must be created before opened
		// "open graph" with a new name is an error
		return dn?gd<DinoMachNode>(dn).handler:0;
#else
		// must not already be open
		return dn?0:new EgzamplView(name);
#endif
	}
	void incr_cb_destroy_handler(IncrLangEvents *h) {
#ifdef CREATE_YOURSELF
		// only you may destroy what you create.  (does not apply to real life.)
#else
		delete h;
#endif
	}
    // no fulfils in this example (or anywhere yet)
    void incr_cb_fulfil_graph(DString name,StrGraph *sg) {}
    void incr_cb_fulfil_node(DString graphname,DString nodename,const StrAttrs &attrs) {}
    void incr_cb_fulfil_edge(DString graphname,DString edgename,const StrAttrs &attrs) {}
    void incr_cb_message(const char *msg) {}
} g_incrCalledBack;

void main() {
	IncrLangEvents *view;
	StrAttrs attrs;
#ifdef CREATE_YOURSELF
	view = new EgzamplView("Z");
#else
	view = g_incrCalledBack.incr_cb_create_handler("Z",attrs);
#endif
	attrs["engines"] = "shapegen,dynadag,labels";
	attrs["resolution"] = "0.1,0.1";
	view->incr_ev_open_graph("Z",attrs);
	attrs.clear();

	// now we have access to THREE APIs!  
	// ONE: access using strings through IncrLangEvents
	// this is pretty much just like using the incrface language
	cout << "step 1" << endl;
	attrs["shape"] = "box";
	attrs["width"] = attrs["height"] = "1";
	view->incr_ev_ins_node("m",attrs,false);

	// TWO: access using C++ through DynaView.  
	// this is the most powerful, direct, and efficient way
	// (but that's not always what's important)
	cout << "step 2" << endl;
	DynaView *dynaview = static_cast<DynaView*>(view);
	// (modify)
	Layout::Node *m = dynaview->getNode("m",false).first;
	gd<NodeGeom>(m).pos = Coord(5,5);
	dynaview->Q.ModNode(m,DG_UPD_MOVE);
	dynaview->maybe_go();
	// (create node)
	cout << "step 2b" << endl;
	Layout::Node *n = dynaview->getNode("n",true).first;
	gd<NodeGeom>(n).pos = Coord(10,10);
	dynaview->Q.InsNode(n);
	dynaview->maybe_go();
	// (create edge)
	cout << "step 2c" << endl;
	Layout::Edge *e = dynaview->getEdge("e",m,n,true).first;
	dynaview->Q.InsEdge(e);
	dynaview->maybe_go();

	// THREE: the incrface language parser
	cout << "step 3" << endl;
	const char *incr_commands = 
		"open graph Z\n" // this is okay because the EgzampleView constructor set m_allowOneReopen
		"insert node Z o [shape=hexagon,height=2,width=2]\n"
		"insert edge Z f o n\n";
	// does anyone please know how to create a FILE* on memory?
	FILE *f = tmpfile();
	fwrite(incr_commands,1,strlen(incr_commands),f);
	fseek(f,0,SEEK_SET);
	incr_yyin = f;
	while(1) {
		try {
			incr_yyparse();
		}
		catch(DVException dvx) { // DynaView exceptions are recoverable
			fprintf(stdout,"message \"%s %s\"\n",dvx.exceptype,dvx.param);;
		}
		catch(IncrGraphNotOpen gnotop) { // graph not open too
			fprintf(stdout,"message \"%s %s\"\n",gnotop.exceptype,gnotop.param);;
		}
		catch(IncrError ie) { // parser errors are not
			fprintf(stdout,"message \"%s\"\n",ie.descrip.c_str());
			return;
		}
		catch(DGException dgx) { // probably an internal error
			fprintf(stdout,"message \"exception: %s\"\n",dgx.exceptype);
			return;
		}
		incr_yyin = stdin; // pass the mike to you
	}
}