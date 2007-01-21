#include "Geometry.h"
#include <iostream>

// SVG is currently used only for debug dumps.
// But it's a start ;-)

namespace Dynagraph {
namespace SVG {

void header(std::ostream &out);
void line(std::ostream &out,const Line &shape,Coord ul,Coord ofs = Coord(0,0));
void footer(std::ostream &out);

} // namespace SVG
} // namespace Dynagraph
