#include "common/createEngine.h"
#include "DynaDAGLayout.h"

namespace Dynagraph {

using DynaDAG::DynaDAGLayout;
using namespace std;

template<>
creators<DynaDAGLayout>::creator_map &creators<DynaDAGLayout>::getMap() {
	static creator_map s_map;
	return s_map;
}

} // namespace Dynagraph
