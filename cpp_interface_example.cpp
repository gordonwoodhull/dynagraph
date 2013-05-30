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
   
  try {
    Name layoutName = "G";
    StrAttrs layoutAttrs;
    
    layoutAttrs["engine"] = "dynadag";
    layoutAttrs["coordtranslation"] = "true";
    layoutAttrs["shapegeneration"] = "true";
    
    EnginePair<GeneralLayout> engines = createCppConfiguration("G", layoutAttrs);
    if(!engines.first) {
      cerr << "failed to create engine with attrs " << layoutAttrs << endl;
      return 1;
    }

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
  }
  catch(Assertion sert) {
    LOCK_REPORT(dgr::incrface);
    reports[dgr::incrface] << "message \"(exception) Assertion: " << sert.expr << "; " << sert.file << ", " << sert.line << '"' << std::endl;
    reports[dgr::error] << "(exception) Assertion: " << sert.expr << "; " << sert.file << ", " << sert.line << std::endl;
    exit(23);
  }
  catch(DGException2 dgx) {
    LOCK_REPORT(dgr::incrface);
    reports[dgr::incrface] << "message \"(exception) " << dgx.exceptype << ": " << dgx.param << '"' << std::endl;
    reports[dgr::error] << "(exception) " << dgx.exceptype << ": " << dgx.param << std::endl;
    exit(23);
  }
  catch(DGException dgx) {
    LOCK_REPORT(dgr::incrface);
    reports[dgr::incrface] << "message \"(exception) " << dgx.exceptype << '"' << std::endl;
    reports[dgr::error] << "(exception) " << dgx.exceptype << std::endl;
    exit(23);
  }
  catch(...) {
    LOCK_REPORT(dgr::incrface);
    reports[dgr::incrface] << "message \"(exception) unknown exception\"" << std::endl;
    reports[dgr::error] << "(exception) unknown exception" << std::endl;
    exit(23);
  }

  return 0;
}
