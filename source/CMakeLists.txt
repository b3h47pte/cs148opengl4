cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project("cs148-opengl4")

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

# CPP Flags
set(CXX_FLAGS "-Wall -std=c++14")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Common Files necessary for all assignments
include_directories("./")
file(GLOB_RECURSE COMMON_SOURCES "common/*.cpp")
file(GLOB_RECURSE COMMON_HEADERS "common/*.h")

# GLM Headers
include_directories("./external/glm")

# GLEW Headers
include_directories("./external/glew/include")

# OpenGL Shenanigans
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# Add Files based on the assignment
if (NOT ASSIGNMENT OR ASSIGNMENT GREATER 3)
    message(STATUS "Assignment NOT SET or INVALID, default to assignment 1")
    set(ASSIGNMENT 1)
endif()

message(STATUS "Compiling: Assignment ${ASSIGNMENT}")
add_definitions("-DASSIGNMENT=${ASSIGNMENT}")

file(GLOB_RECURSE ASSIGNMENT_SOURCES "./assignment${ASSIGNMENT}/*.cpp")
file(GLOB_RECURSE ASSIGNMENT_HEADERS "./assignment${ASSIGNMENT}/*.h")

# Shaders. Need to copy shaders to the executables working directory. 
file(GLOB_RECURSE SHADERS "./shaders/*.vert" "./shaders/*.frag")

set(SHADER_TARGET_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
if (${CMAKE_GENERATOR} STREQUAL "Unix Makefiles")
    set(SHADER_TARGET_DIRECTORY ${SHADER_TARGET_DIRECTORY}/../)
endif()

add_custom_target(shaderHandler ALL 
    python ${CMAKE_CURRENT_SOURCE_DIR}/scripts/dirCopy.py ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${SHADER_TARGET_DIRECTORY}/shaders
    DEPENDS ${SHADERS}
    SOURCES ${SHADERS})

# Note that file globbing is generally not recommended. It is usually better to explicitly list files; however,
# I am assuming that most students are unfamiliar with CMake and thus will not be able to update the CMakeLists.txt
# file appropriately. Hence, globbing is necessary for me to generate the Makefiles/whatever again should I ever
# want to run their code.
add_definitions(${CXX_FLAGS})
add_executable(cs148opengl4 main.cpp ${COMMON_SOURCES} ${COMMON_HEADERS}
    ${ASSIGNMENT_SOURCES} ${ASSIGNMENT_HEADERS})
add_dependencies(cs148opengl4 shaderHandler)

# OpenGL
target_link_libraries(cs148opengl4 ${OPENGL_LIBRARIES})

# SDL2 Library
if (APPLE)
    target_link_libraries(cs148opengl4 "${CMAKE_SOURCE_DIR}/external/SDL2/osx/SDL2.framework")

    # Need to code sign for use with XCode!
    add_custom_command(TARGET cs148opengl4 POST_BUILD
        COMMAND codesign -f -s - "${CMAKE_SOURCE_DIR}/external/SDL2/osx/SDL2.framework")
endif()

# GLEW Library
if (APPLE)
    target_link_libraries(cs148opengl4 "${CMAKE_SOURCE_DIR}/external/glew/distrib/osx/lib/libGLEW.a")
endif()
