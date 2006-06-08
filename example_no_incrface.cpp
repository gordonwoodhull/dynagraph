#include <iostream>
#include "common/ChangeQueue.h"
#include "fdp/FDPLayout.h"
#include "incrface/createEngine.h"

using namespace std;
using namespace Dynagraph;

typedef FDP::FDPLayout Layout;

void printChanges(ChangeQueue<Layout> &queue) {
	for(Layout::node_iter ni = queue.insN.nodes().begin(); ni!=queue.insN.nodes().end(); ++ni)
		cout << "node " << gd<Name>(*ni) << " inserted at " << gd<NodeGeom>(*ni).pos << endl;
	for(Layout::graphedge_iter ei = queue.insE.edges().begin(); ei!=queue.insE.edges().end(); ++ei)
		cout << "edge " << gd<Name>(*ei) << " inserted at " << gd<EdgeGeom>(*ei).pos << endl;
	for(Layout::node_iter ni = queue.modN.nodes().begin(); ni!=queue.modN.nodes().end(); ++ni)
		cout << "node " << gd<Name>(*ni) << " moved to " << gd<NodeGeom>(*ni).pos << endl;
	for(Layout::graphedge_iter ei = queue.modE.edges().begin(); ei!=queue.modE.edges().end(); ++ei)
		cout << "edge " << gd<Name>(*ei) << " moved to " << gd<EdgeGeom>(*ei).pos << endl;
	for(Layout::node_iter ni = queue.delN.nodes().begin(); ni!=queue.delN.nodes().end(); ++ni)
		cout << "node " << gd<Name>(*ni) << " deleted" << endl;
	for(Layout::graphedge_iter ei = queue.delE.edges().begin(); ei!=queue.delE.edges().end(); ++ei)
		cout << "edge " << gd<Name>(*ei) << " deleted" << endl;
}
void main() {
	Layout layout,current(&layout);

	gd<GraphGeom>(&layout).resolution = Coord(0.1,0.1);
	gd<GraphGeom>(&layout).separation = Coord(0.5,0.5);

	ChangeQueue<Layout> queue(&layout,&current);
	DString engines = "shapegen,fdp,voronoi,visspline";
	gd<StrAttrs>(&layout)["engines"] = engines;

	ChangeProcessor<Layout> *server = createEngine(engines,&layout,&current).first;

	Layout::Node *m = layout.create_node("m"),*n = layout.create_node("n");
	gd<NodeGeom>(m).pos = Coord(10,10);
	gd<NodeGeom>(n).pos = Coord(15,10);
	gd<PolyDef>(m).exterior_box = gd<PolyDef>(n).exterior_box = Coord(1,1);
	queue.InsNode(n);
	queue.InsNode(m);

	cout << "step 1" << endl;
	server->Process();
	printChanges(queue);
	queue.Execute(true);
	ModifyFlags(queue) = 0;

	Layout::Edge *e = layout.create_edge(m,n,"e").first;
	queue.InsEdge(e);

	cout << "step 2" << endl;
	server->Process();
	printChanges(queue);
	queue.Execute(true);
	ModifyFlags(queue) = 0;

	gd<NodeGeom>(n).pos = Coord(5,5);
	ModifyNode(queue,n,DG_UPD_MOVE);

	cout << "step 3" << endl;
	server->Process();
	printChanges(queue);
	queue.Execute(true);
	ModifyFlags(queue) = 0;

	queue.DelNode(n);

	cout << "step 4" << endl;
	server->Process();
	printChanges(queue);
	queue.Execute(true);
	ModifyFlags(queue) = 0;
}