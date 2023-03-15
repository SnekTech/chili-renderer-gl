//
// Created by syf on 2023/3/10.
//

#ifndef CHILI_RENDERER_GL_CUBE_H
#define CHILI_RENDERER_GL_CUBE_H

#include <vector>
#include "../DataStructures/Vec3.h"
#include "DataStructures/IndexedList.h"

class Cube
{
public:
	explicit Cube(float size)
	{
		const float side = size / 2.0f;
		vertices.emplace_back( -side,-side,-side );
		vertices.emplace_back( side,-side,-side );
		vertices.emplace_back( -side,side,-side );
		vertices.emplace_back( side,side,-side );
		vertices.emplace_back( -side,-side,side );
		vertices.emplace_back( side,-side,side );
		vertices.emplace_back( -side,side,side );
		vertices.emplace_back( side,side,side );
	}
	[[nodiscard]]
	IndexedLineList GetLines() const
	{
		return {
			vertices, {
				0,1,  1,3,  3,2,  2,0,
				0,4,  1,5,  3,7,  2,6,
				4,5,  5,7,  7,6,  6,4
			}
		};
	}
    [[nodiscard]] IndexedTriangleList GetTriangles() const
    {
        return{
            vertices,{
                0,2,1, 2,3,1,
                1,3,5, 3,7,5,
                2,6,3, 3,6,7,
                4,5,7, 4,7,6,
                0,4,2, 2,4,6,
                0,1,4, 1,5,4 }
        };
    }
private:
	std::vector<Vec3> vertices;
};

#endif //CHILI_RENDERER_GL_CUBE_H
