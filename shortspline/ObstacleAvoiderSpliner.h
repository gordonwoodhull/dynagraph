extern "C" {
#include "pathplan.h"
}

struct ObstacleAvoiderSpliner {
	std::vector<Ppoly_t*> obs;
	std::vector<Ppoly_t> polys;
	std::vector<Line> polydat;
	vconfig_t	*vconfig;

	ObstacleAvoiderSpliner(Layout *layout);
	void FindSpline(Coord a,Coord b,Line &ret);
	void make_barriers(int pp, int qp, Pedge_t **barriers, int *n_barriers);
};
struct ClockwiseShapes : DGException {
	ClockwiseShapes() : DGException("node shapes must be counter-clockwise") {}
};
