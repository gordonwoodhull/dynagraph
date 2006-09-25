#include "common/createEngine.h"
#include "common/GeneralLayout.h"

// this system of engine creation is deprecated in favor of the Configurator model
// keeping these files intact in case anyone is using createEngine (LMK!)

namespace Dynagraph {

template<>
creators<GeneralLayout>::creator_map &creators<GeneralLayout>::getMap() {
	static creator_map s_map;
	return s_map;
}

} // namespace Dynagraph
