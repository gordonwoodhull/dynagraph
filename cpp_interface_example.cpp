// example of using the Dynagraph libraries directly from C++

#include <iostream>
#include "incrface/IncrLangEvents.h"
#include "createConfiguration.h"

using namespace Dynagraph;
using namespace std;

int main() {
	// enable basic dynagraph report streams
	reports.enable(dgr::error, &std::cerr);
	reports.enable(dgr::cmdline, &std::cout);
	reports.enable(dgr::output, &std::cout);

    // the layouts are held in a graph called the DinoMachine in which
    // the nodes are layouts along with the handlers for those layouts
    
    // use incrface's callback, held in g_incrCallback, to create a handler
    // for the particular graph attributes that you want.  here we'll create
    // a dynadag layout with rotation and shape generation
    
    Name layoutName = "G";
    StrAttrs layoutAttrs;
    
    layoutAttrs["engine"] = "dynadag";
    layoutAttrs["coordtranslation"] = "true";
    layoutAttrs["shapegeneration"] = "true";
    
    EnginePair<GeneralLayout> engines = createCppConfiguration("G", layoutAttrs);
    ChangingGraph<GeneralLayout> *world = engines.world();
    
    engines.first->Open();
    GeneralLayout::Node *n = world->whole_.create_node("n"),
        *m = world->whole_.create_node("m");
    world->Q_.InsNode(n);
    world->Q_.InsNode(m);
    engines.first->Process();
    
    cout << "Before edge" << endl;
    cout << "n: " << gd<NodeGeom>(n).pos << endl;
    cout << "m: " << gd<NodeGeom>(m).pos << endl;
    
    GeneralLayout::Edge *e = world->whole_.create_edge(n,m, "e");
    world->Q_.InsEdge(e);
    engines.first->Process();

    cout << "After edge" << endl;
    cout << "n: " << gd<NodeGeom>(n).pos << endl;
    cout << "m: " << gd<NodeGeom>(m).pos << endl;
    cout << "e: " << gd<EdgeGeom>(e).pos << endl;
        
    return 0;
}