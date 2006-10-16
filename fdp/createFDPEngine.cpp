#include "common/createEngine.h"
#include "FDPLayout.h"

// this system of engine creation is deprecated in favor of the Configurator model
// keeping these files intact in case anyone is using createEngine (LMK!)

namespace Dynagraph {

using FDP::FDPLayout;

template<>
creators<FDP::FDPLayout>::creator_map &creators<FDPLayout>::getMap() {
	static creator_map s_map;
	return s_map;
}

} // namespace Dynagraph
