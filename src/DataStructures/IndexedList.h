//
// Created by syf on 2023/3/10.
//

#ifndef CHILI_RENDERER_GL_INDEXEDLIST_H
#define CHILI_RENDERER_GL_INDEXEDLIST_H

#include <utility>
#include <vector>
#include "DataStructures/Vec3.h"

struct IndexedLineList
{
	std::vector<Vec3> vertices;
	std::vector<unsigned int> indices;
};

template <typename T>
struct IndexedTriangleList
{
    IndexedTriangleList(std::vector<T> verticesIn, std::vector<size_t> indicesIn)
        : vertices(std::move(verticesIn)), indices(std::move(indicesIn))
        {
            assert(vertices.size() > 2);
            assert(indices.size() % 3 == 0);
            cullFlags.resize(indices.size() / 3, false);
        }

    std::vector<T> vertices;
    std::vector<size_t> indices;
    std::vector<bool> cullFlags;
};

#endif //CHILI_RENDERER_GL_INDEXEDLIST_H
