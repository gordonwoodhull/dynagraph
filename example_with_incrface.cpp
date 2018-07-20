#include "common/ChangeQueue.h"
#include "common/Transform.h"
#include "incrface/createEngine.h"
#include "incrface/incrparse.h"
#include "incrface/IncrViewWatcher.h"
#include <iostream>

// This example shows the many ways to use Dynagraph via the incrface library
// Probably only one of these ways will be relevant to your application

// The cleaner way to handle lifetimes of graphs is to let incrface do it
// but many clients will want control, so the library uses create and destroy callbacks
// this example demonstrates both paradigms;
// #define CREATE_YOURSELF
// selects the messier but more powerful client-control way

using namespace std;
using namespace Dynagraph;

// determines how to interpret string coordinates.  specify not to change them:
Transform *g_transform = new Transform(Coord(1,1),Coord(1,1));
// whether to use the same resolution,separation,defaultsize as dot
bool g_useDotDefaults = false;

template<typename Layout>
struct ExampleResponse : LinkedChangeProcessor<Layout>,IncrViewWatcher<Layout> {
    void Process() {
        // often this method will need to map Layout nodes & edges to application objects
        // sorry, there is no better way than to use a map<>
        for(Layout::node_iter ni = this->world_->Q_.insN.nodes().begin(); ni!=this->world_->Q_.insN.nodes().end(); ++ni)
            reports[dgr::output] << "node " << gd<Name>(*ni) << " inserted at " << gd<NodeGeom>(*ni).pos << endl;
        for(Layout::graphedge_iter ei = this->world_->Q_.insE.edges().begin(); ei!=this->world_->Q_.insE.edges().end(); ++ei)
            reports[dgr::output] << "edge " << gd<Name>(*ei) << " inserted at " << gd<EdgeGeom>(*ei).pos << endl;
        for(Layout::node_iter ni = this->world_->Q_.modN.nodes().begin(); ni!=this->world_->Q_.modN.nodes().end(); ++ni)
            reports[dgr::output] << "node " << gd<Name>(*ni) << " moved to " << gd<NodeGeom>(*ni).pos << endl;
        for(Layout::graphedge_iter ei = this->world_->Q_.modE.edges().begin(); ei!=this->world_->Q_.modE.edges().end(); ++ei)
            reports[dgr::output] << "edge " << gd<Name>(*ei) << " moved to " << gd<EdgeGeom>(*ei).pos << endl;
        for(Layout::node_iter ni = this->world_->Q_.delN.nodes().begin(); ni!=this->world_->Q_.delN.nodes().end(); ++ni)
            reports[dgr::output] << "node " << gd<Name>(*ni) << " deleted" << endl;
        for(Layout::graphedge_iter ei = this->world_->Q_.delE.edges().begin(); ei!=this->world_->Q_.delE.edges().end(); ++ei)
            reports[dgr::output] << "edge " << gd<Name>(*ei) << " deleted" << endl;
        // Being at the end of the processing chain, this must clear the queue
        this->world_->Q_.Execute(true);
    }
    // IncrViewWatcher callbacks, not used in this example
    void IncrOpen(ChangeQueue<Graph> &Q) {}
    void IncrClose(ChangeQueue<Graph> &Q) {}
    void FulfilGraph(Graph *g) {}
    void FulfilNode(typename Graph::Node *n) {}
    void FulfilEdge(typename Graph::Edge *e) {}
    ExampleResponse(Name name) : DynaView(name,g_transform,g_useDotDefaults) {
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
    void incr_cb_create_handler(Name name,const StrAttrs &attrs) {
#ifdef CREATE_YOURSELF
        // must be created before opened
        // "open graph" with a new name is an error
        dgassert(false);
#else
        incr_set_handler(name,new ExampleResponse(name))
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
    // enable basic dynagraph report streams
    reports.enable(dgr::error,&cerr);
    reports.enable(dgr::cmdline);
    reports.enable(dgr::output);

    IncrLangEvents *view;
    StrAttrs attrs;
#ifdef CREATE_YOURSELF
    view = new ExampleResponse("Z");
#else
    g_incrCalledBack.incr_cb_create_handler("Z",attrs);
    view = incr_get_handler("Z");
#endif
    attrs["engines"] = "shapegen,dynadag,labels";
    attrs["resolution"] = "0.1,0.1";
    view->incr_ev_open_graph("Z",attrs);
    attrs.clear();

    // now we have access to THREE APIs!
    // ONE: access using strings through IncrLangEvents
    // this is pretty much just like using the incrface language
    reports[dgr::output] << "step 1" << endl;
    attrs["shape"] = "box";
    attrs["width"] = attrs["height"] = "1";
    view->incr_ev_ins_node("m",attrs,false);

    // TWO: access using C++ through DynaView.
    // this is the most powerful, direct, and efficient way
    // (but that's not always what's important)
    reports[dgr::output] << "step 2" << endl;
    DynaView *dynaview = static_cast<DynaView*>(view);
    // (modify)
    Layout::Node *m = dynaview->getNode("m",false).first;
    gd<NodeGeom>(m).pos = Coord(5,5);
    ModifyNode(dynaview->Q,m,DG_UPD_MOVE);
    dynaview->maybe_go();
    // (create node)
    reports[dgr::output] << "step 2b" << endl;
    Layout::Node *n = dynaview->getNode("n",true).first;
    gd<NodeGeom>(n).pos = Coord(10,10);
    dynaview->Q.InsNode(n);
    dynaview->maybe_go();
    // (create edge)
    reports[dgr::output] << "step 2c" << endl;
    Layout::Edge *e = dynaview->getEdge("e",m,n,true).first;
    dynaview->Q.InsEdge(e);
    dynaview->maybe_go();

    // THREE: the incrface language parser
    reports[dgr::output] << "step 3" << endl;
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
        catch(Assertion sert) {
            reports[dgr::output] << "message \"(exception) Assertion: " << sert.expr << "; " << sert.file << ", " << sert.line << '"' << endl;
            if(g_xeptFatal)
                throw;
            if(sert.fatal)
                exit(23);
        }
        catch(DGException2 dgx) {
            reports[dgr::output] << "message \"(exception) " << dgx.exceptype << ": " << dgx.param << endl;
            if(g_xeptFatal)
                throw;
            if(dgx.fatal)
                exit(23);
        }
        catch(DGException dgx) {
            reports[dgr::output] << "message \"(exception) " << dgx.exceptype << endl;
            if(g_xeptFatal)
                throw;
            if(dgx.fatal)
                exit(23);
        }
        catch(...) {
            reports[dgr::output] << "message \"(exception) unknown exception\"" << endl;
            exit(23);
        }
        incr_yyin = stdin; // pass the mike to you
    }
}
