#include "common/createEngine.h"

#include "common/GeneralLayout.h"
#include "common/ColorByAge.h"
#include "common/LabelPlacer.h"
#include "common/ShapeGenerator.h"

#include "dynadag/DynaDAGLayout.h"
#include "dynadag/DynaDAG.h"
#include "dynadag/NSRanker.h"

#include "voronoi/voronoi.h"
#include "fdp/fdp.h"
#include "shortspline/ObAvSplinerEngine.h"

using namespace Dynagraph;

AddEngine<GeneralLayout, DynaDAG::NSRanker<GeneralLayout> > GLRanker("nsranker");
AddEngine<GeneralLayout, LabelPlacer<GeneralLayout> > GLLabels("labels");
AddEngine<GeneralLayout, ShapeGenerator<GeneralLayout> > GLShapes("shapegen");
AddEngine<GeneralLayout, ColorByAge<GeneralLayout> > GLColors("colorbyage");

using DynaDAG::DynaDAGLayout;
AddEngine<DynaDAGLayout,DynaDAG::DynaDAGServer> DDEngine("dynadag");
AddEngine<DynaDAGLayout,DynaDAG::NSRanker<DynaDAGLayout> > DDRanker("nsranker");
AddEngine<DynaDAGLayout,LabelPlacer<DynaDAGLayout> > DDLabels("labels");
AddEngine<DynaDAGLayout,ShapeGenerator<DynaDAGLayout> > DDShapes("shapegen");

using FDP::FDPLayout;
AddEngine<FDPLayout,FDP::FDPServer> FDPServer("fdp");
AddEngine<FDPLayout,Voronoi::VoronoiServer> FDPVoronoi("voronoi");
AddEngine<FDPLayout,ObAvSplinerEngine<FDPLayout> > FDPSplines("visspline");
AddEngine<FDPLayout,LabelPlacer<FDPLayout> > FDPLabels("labels");
AddEngine<FDPLayout,ShapeGenerator<FDPLayout> > FDPShapes("shapegen");

