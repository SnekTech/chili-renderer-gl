cmake_minimum_required(VERSION 3.15)
project(chili-renderer-gl LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
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

list(APPEND LINK_LIBS glfw)

list(APPEND MAIN_SOURCE ${GLAD_DIR}/src/gl.c src/main.cpp)
add_executable(main ${MAIN_SOURCE} src/Shader.h src/Surface.cpp src/Surface.h src/Colors.h src/Graphics.cpp src/Graphics.h src/Game.cpp src/Game.h src/FrameTimer.cpp src/FrameTimer.h src/Mat2.h src/Mat3.h)
target_include_directories(main PUBLIC ${INCLUDE_DIR})
target_link_libraries(main PUBLIC ${LINK_LIBS})
