cmake_minimum_required(VERSION 3.15)
project(chili-renderer-gl LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# glad
set(GLAD_DIR ./external/glad)
list(APPEND INCLUDE_DIR ${GLAD_DIR}/include)
file(GLOB BUTTERFLIES_SOURCES_C ${CMAKE_CURRENT_SOURCE_DIR} *.c glad/src/gl.c)

# GLFW
set(GLFW_DIR ./external/glfw)
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIR} glfw_binary EXCLUDE_FROM_ALL)

list(APPEND INCLUDE_DIR ${GLFW_DIR}/include)
list(APPEND INCLUDE_DIR ${GLFW_DIR}/deps)
list(APPEND INCLUDE_DIR src)

list(APPEND LINK_LIBS glfw)

list(APPEND MAIN_SOURCE ${GLAD_DIR}/src/gl.c src/main.cpp)
add_executable(main ${MAIN_SOURCE} src/Widgets/Shader.h src/HardwareRender/Surface.cpp src/HardwareRender/Surface.h src/HardwareRender/Colors.h src/HardwareRender/Graphics.cpp src/HardwareRender/Graphics.h src/Game.cpp src/Game.h src/Widgets/FrameTimer.cpp src/Widgets/FrameTimer.h src/DataStructures/Mat.h src/PreClipScreenTransformer.h src/DataStructures/IndexedList.h src/Shapes/Cube.h src/Widgets/Controller.h src/Widgets/Controller.cpp src/Scenes/Scene.h src/Scenes/SolidCubeScene.h src/Scenes/CubeOrderScene.h src/Shapes/ConcaveHexahedron.h src/Scenes/ConHexWireScene.h src/Scenes/ConHexScene.h src/Scenes/XMutualScene.h src/Shapes/XQuads.h src/DataStructures/TexVertex.h src/Scenes/TexCubeScene.h external/stb_image.h src/Shapes/CubeSkinned.h src/Scenes/CubeSkinnedScene.h)
target_include_directories(main PUBLIC ${INCLUDE_DIR})
target_link_libraries(main PUBLIC ${LINK_LIBS})
