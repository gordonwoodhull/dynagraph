#include "common/createEngine.h"
#include "FDPLayout.h"

namespace Dynagraph {

using FDP::FDPLayout;

template<>
creators<FDP::FDPLayout>::creator_map &creators<FDPLayout>::getMap() {
	static creator_map s_map;
	return s_map;
}

} // namespace Dynagraph
