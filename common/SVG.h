#ifndef SVG_h
#define SVG_h

#include "Geometry.h"
#include <iostream>

// SVG is currently used only for debug dumps.
// But it's a start ;-)

namespace Dynagraph {
namespace SVG {
    
void header(std::ostream &out);
void line(std::ostream &out,const Line &shape,Coord ul,Coord ofs = Coord(0,0), std::string const& color = "rgb(0,0,0)");
inline void line(std::ostream &out,const Line &shape,Coord ul,std::string const& color) {
    line(out,shape,ul,Coord(0,0),color);
}
void footer(std::ostream &out);

} // namespace SVG
} // namespace Dynagraph
#endif // SVG_h
