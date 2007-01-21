#include "SVG.h"

// SVG is currently used only for debug dumps.
// But it's a start ;-)

using namespace std;

namespace Dynagraph {
namespace SVG {

void header(ostream &out) {
	out << "<?xml version=\"1.0\" standalone=\"no\"?>" << endl <<
		"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\"" << endl <<
		"\"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">" << endl;
	out << "<svg width=\"100%%\" height=\"100%%\">" << endl;
}
void line(ostream &out,const Line &shape,Coord ul,Coord ofs) {
	if(shape.size()) {
		out << "<path d=\"M" << shape[0].x+ofs.x-ul.x << ' ' << ul.y-(shape[0].y+ofs.y) << ' ';
		switch(shape.degree) {
		case 1: out << "L";
			break;
		case 3:	out << "C";
			break;
		default:
			dgassert(0);
		}
		for(Line::const_iterator ci = shape.begin()+1;ci!=shape.end(); ++ci) {
			out << ci->x+ofs.x-ul.x << ' ' << ul.y-(ci->y+ofs.y);
			if(ci!=shape.end()-1)
				out << ' ';
		}
		out << '"' << endl <<
			"style=\"fill:none;stroke:rgb(0,0,0);stroke-width:0.1\"/>" << endl;
	}
}
void footer(ostream &out) {
	out << "</svg>" << endl;
}

} // namespace SVG
} // namespace Dynagraph
