//
// Created by syf on 2023/3/11.
//

#ifndef CHILI_RENDERER_GL_CPPFEATURETESTER_H
#define CHILI_RENDERER_GL_CPPFEATURETESTER_H

#include <iostream>
#include <memory>

namespace Widgets {
	class CppFeatureTester
	{
	public:
		CppFeatureTester()
		{
			auto pMatrix = std::make_unique<int[]>(9);
			pMatrix[3] = 42;
			log(pMatrix[3]);
		}
	private:
		template<typename T>
		void log(const T& value)
		{
			std::cout << value << std::endl;
		}
	};
}

#endif //CHILI_RENDERER_GL_CPPFEATURETESTER_H
