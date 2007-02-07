#ifndef Dump_h
#define Dump_h

#include "DynaDAG.h"

namespace Dynagraph {
namespace DynaDAG {

void dumpModel(std::ostream &out,DDModel &model,bool nod,bool edg);

} // namespace DynaDAG
} // namespace Dynagraph
#endif // Dump_h
