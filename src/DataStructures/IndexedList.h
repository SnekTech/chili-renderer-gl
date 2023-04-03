//
// Created by syf on 2023/3/10.
//

#ifndef CHILI_RENDERER_GL_INDEXEDLIST_H
#define CHILI_RENDERER_GL_INDEXEDLIST_H

#include <utility>
#include <vector>
#include "DataStructures/Vec3.h"
#include "../external/tiny_obj_loader.h"
#include <fstream>
#include <sstream>

struct IndexedLineList
{
    std::vector<Vec3> vertices;
    std::vector<unsigned int> indices;
};

template<typename T>
struct IndexedTriangleList
{
    IndexedTriangleList() = default;

    IndexedTriangleList(std::vector<T> verticesIn, std::vector<size_t> indicesIn)
        : vertices(std::move(verticesIn)), indices(std::move(indicesIn))
    {
        assert(vertices.size() > 2);
        assert(indices.size() % 3 == 0);
    }

    static IndexedTriangleList<T> Load(const std::string& filename)
    {
        IndexedTriangleList<T> tl;

        bool isCCW;
        {
            std::ifstream file(filename);
            std::string firstLine;
            std::getline(file, firstLine);
            for (auto& c: firstLine)
            {
                c = (char)std::tolower(c);
            }

            isCCW = firstLine.find("ccw") != std::string::npos;
        }

        using namespace tinyobj;
        attrib_t attrib;
        std::vector<shape_t> shapes;
        std::string err;

        const bool loadSuccessful = LoadObj(&attrib, &shapes, nullptr, &err, filename.c_str());


        // check for errors
        if (!err.empty() && err.substr(0, 4) != "WARN")
        {
            throw std::runtime_error(("LoadObj returned error:" + err + " File:" + filename).c_str());
        }
        if (!loadSuccessful)
        {
            throw std::runtime_error(("LoadObj returned false  File:" + filename).c_str());
        }
        if (shapes.empty())
        {
            throw std::runtime_error(("LoadObj object file had no shapes  File:" + filename).c_str());
        }

        tl.vertices.reserve(attrib.vertices.size() / 3u);
        for (int i = 0; i < attrib.vertices.size(); i += 3)
        {
            tl.vertices.emplace_back(Vec3{
                attrib.vertices[i + 0],
                attrib.vertices[i + 1],
                attrib.vertices[i + 2],
            });
        }

        // extract index data
        const auto& mesh = shapes[0].mesh;
        tl.indices.reserve(mesh.indices.size());
        for (size_t f = 0; f < mesh.num_face_vertices.size(); f++)
        {
            if (mesh.num_face_vertices[f] != 3u)
            {
                std::stringstream ss;
                ss << "LoadObj error face #" << f << " has "
                   << mesh.num_face_vertices[f] << " vertices";
                throw std::runtime_error(ss.str().c_str());
            }

            for (size_t vn = 0; vn < 3u; vn++)
            {
                const auto idx = mesh.indices[f * 3u + vn];
                tl.indices.push_back(size_t(idx.vertex_index));
            }

            // reverse winding if file marked as CCW
            if (isCCW)
            {
                std::swap(tl.indices.back(), *std::prev(tl.indices.end(), 2));
            }

        }

        return tl;
    }

    std::vector<T> vertices;
    std::vector<size_t> indices;
};

#endif //CHILI_RENDERER_GL_INDEXEDLIST_H
