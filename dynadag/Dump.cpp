#include "DynaDAG.h"
#include "common/SVG.h"

namespace Dynagraph {
namespace DynaDAG {

void dumpModel(std::ostream &out,DDModel &model,bool nod,bool edg) {
	const int SC=4;
	SVG::header(out);
	Line line;
	line.degree = 1;
	line.resize(2);
	for(DDModel::graphedge_iter ei = model.edges().begin(); ei!=model.edges().end(); ++ei)
		if(gd<DDEdge>(*ei).amNodePart()&&nod || gd<DDEdge>(*ei).amEdgePart()&&edg) {
			line[0].x = gd<DDNode>((*ei)->tail).order*SC;
			line[0].y = gd<DDNode>((*ei)->tail).rank*SC;
			line[1].x = gd<DDNode>((*ei)->head).order*SC;
			line[1].y = gd<DDNode>((*ei)->head).rank*SC;
			SVG::line(out,line,Coord(0,0));
		}
	SVG::footer(out);
}

} // namespace Dynagraph
} // namespace DynaDAG
