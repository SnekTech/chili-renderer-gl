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
add_executable(main ${MAIN_SOURCE} src/Widgets/Shader.h src/Render/Surface.cpp src/Render/Surface.h src/Render/Colors.h src/Render/Graphics.cpp src/Render/Graphics.h src/Game.cpp src/Game.h src/Widgets/FrameTimer.cpp src/Widgets/FrameTimer.h src/DataStructures/Mat.h src/Render/NDCScreenTransformer.h src/DataStructures/IndexedList.h src/Widgets/Controller.h src/Widgets/Controller.cpp src/Scenes/Scene.h external/stb_image.h src/Shapes/Cube.h src/Scenes/CubeSkinnedScene.h src/Render/Pipeline.h src/DataStructures/Triangle.h src/Effects/TextureEffect.h src/Effects/VertexColorEffect.h src/Scenes/CubeVertexColorScene.h src/Scenes/CubeSolidScene.h src/Effects/SolidEffect.h src/Scenes/DoubleCubeScene.h src/Render/ZBuffer.h src/Effects/DefaultVertexShader.h src/Shapes/Plane.h src/Scenes/VertexWaveScene.h src/Effects/WaveVertexTextureEffect.h src/Effects/VertexPositionColorEffect.h src/Scenes/CubeVertexPositionColorScene.h src/Scenes/CubeSolidGeometryScene.h src/Effects/SolidGeometryEffect.h src/Scenes/CubeFlagIndependentScene.h src/Widgets/Keyboard.cpp src/Widgets/Keyboard.h external/tiny_obj_loader.cpp)
target_include_directories(main PUBLIC ${INCLUDE_DIR})
target_link_libraries(main PUBLIC ${LINK_LIBS})
