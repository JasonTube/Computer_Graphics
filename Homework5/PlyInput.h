#ifndef _PLY_INPUT_H
#define _PLY_INPUT_H

#include "Geometry.h"

extern bool LoadSimplePlyFile(const char *fileName, 
							  FPoint3 *&vertices, int &nVertices, 
							  int *&indices, int &nIndices);

#endif	// _PLY_INPUT_H