cmake_minimum_required(VERSION 3.15)
project(BuildingGenerator)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/build)

# Set the library output directory (if you have libraries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/build)

# Set the archive output directory (for static libraries)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/build)

# Set the CMake temporary build directory
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/out/build)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE HDR_FILES src/*.hpp)
file(GLOB_RECURSE SHADERS shaders/*.glsl)
set(GLAD_SRC external/glad/src/glad.c)

add_executable(BuildingGenerator
	src/main.cpp
	${HDR_FILES}
	${SHADERS}
	${GLAD_SRC}
 "src/shape.cpp" "src/shader.cpp" "src/mesh.cpp" "src/camera.cpp")

target_include_directories(BuildingGenerator PRIVATE
	external/glad/include
	external/glfw/include
	external/glm
)

link_directories(${CMAKE_SOURCE_DIR}/external/glfw/lib-vc2022)

target_link_libraries(BuildingGenerator ${GLFW3_LIBRARY} OpenGL::GL)