#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoShapeHints.h>
#include<Inventor/nodes/SoDrawStyle.h>
#include<Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTransform.h>
#include "Mesh.h"


class Painter
{
public:
	SoSeparator* getShapeSep(Mesh* mesh);
	SoSeparator* get1PointSep(Mesh* obj, int pnt, int drawWhat, float deltaX, float deltaY, float scale, bool showNeighbours=false);
	SoSeparator* paintNeighbours(Mesh* mesh, int pnt);
	SoSeparator* DrawLines(Mesh* mesh,int source,int targ,int N, int* prev);
	vector<int> getPath(int* prev, int source, int targ, int N);
};
