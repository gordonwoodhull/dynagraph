#include <iostream>
#include "common/Dynagraph.h"
#include "incrface/createLayoutServer.h"

using namespace std;

void printChanges(ChangeQueue &queue) {
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

	ChangeQueue queue(&layout,&current);
	gd<StrAttrs>(&layout)["engines"] = "shapegen,fdp,voronoi,visspline";

	Server *server = createLayoutServer(&layout,&current);

	Layout::Node *m = layout.create_node(),*n = layout.create_node();
	gd<Name>(m) = "m";
	gd<Name>(n) = "n";
	gd<NodeGeom>(m).pos = Coord(10,10);
	gd<NodeGeom>(n).pos = Coord(15,10);
	queue.InsNode(n);
	queue.InsNode(m);

	server->Process(queue);
	cout << "step 1" << endl;
	printChanges(queue);
	queue.Okay(true);

	Layout::Edge *e = layout.create_edge(m,n).first;
	gd<Name>(e) = "e";
	queue.InsEdge(e);

	server->Process(queue);
	cout << "step 2" << endl;
	printChanges(queue);
	queue.Okay(true);

	gd<NodeGeom>(n).pos = Coord(5,5);
	queue.ModNode(n,DG_UPD_MOVE);

	server->Process(queue);
	cout << "step 3" << endl;
	printChanges(queue);
	queue.Okay(true);

	queue.DelNode(n);

	server->Process(queue);
	cout << "step 4" << endl;
	printChanges(queue);
	queue.Okay(true);

}