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
			l0g("testing cpp features...");
		}
	private:
		template<typename T>
		void l0g(const T& value)
		{
			std::cout << value << std::endl;
		}
	};
}

#endif //CHILI_RENDERER_GL_CPPFEATURETESTER_H
