//
// Created by syf on 2023/3/10.
//

#ifndef CHILI_RENDERER_GL_INDEXEDLINELIST_H
#define CHILI_RENDERER_GL_INDEXEDLINELIST_H

#include <vector>
#include "DataStructures/Vec3.h"

struct IndexedLineList
{
	std::vector<Vec3> vertices;
	std::vector<unsigned int> indices;
};

#endif //CHILI_RENDERER_GL_INDEXEDLINELIST_H
