#include "common/createEngine.h"
#include "DynaDAGLayout.h"

// this system of engine creation is deprecated in favor of the Configurator model
// keeping these files intact in case anyone is using createEngine (LMK!)

namespace Dynagraph {

using DynaDAG::DynaDAGLayout;
using namespace std;

template<>
creators<DynaDAGLayout>::creator_map &creators<DynaDAGLayout>::getMap() {
	static creator_map s_map;
	return s_map;
}

} // namespace Dynagraph
