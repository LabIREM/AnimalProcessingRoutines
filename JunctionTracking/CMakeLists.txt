# define cmake minimum requirement
cmake_minimum_required(VERSION 2.8)

# project information 
project(JunctionTracking)

## Global Libraries ###########################################################################
# Set and define required libraries for all projects

# ITK Libraries
find_package(ITK REQUIRED)
if(ITK_FOUND)
	include(${ITK_USE_FILE})
endif()

## source files #####################################################################
# set sources and headers files

include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/header)

set(header 
${CMAKE_SOURCE_DIR}/header/border.h
${CMAKE_SOURCE_DIR}/header/bordertracking.h
${CMAKE_SOURCE_DIR}/header/cell.h
${CMAKE_SOURCE_DIR}/header/intensityV.h
${CMAKE_SOURCE_DIR}/header/pixel.h
${CMAKE_SOURCE_DIR}/header/skelpix.h
${CMAKE_SOURCE_DIR}/header/tools.h
)

set(source 
${CMAKE_SOURCE_DIR}/source/border.cpp 
${CMAKE_SOURCE_DIR}/source/bordertracking.cxx
${CMAKE_SOURCE_DIR}/source/cell.cpp 
${CMAKE_SOURCE_DIR}/source/intensityV.cpp 
${CMAKE_SOURCE_DIR}/source/skelpix.cpp 
${CMAKE_SOURCE_DIR}/source/pixel.cpp 
${CMAKE_SOURCE_DIR}/source/tools.cpp 
)

## executable #######################################################################
# define executable and link libraries

add_executable(JunctionTracking ${header} ${source})

target_link_libraries(JunctionTracking ${ITK_LIBRARIES})

set_target_properties(JunctionTracking PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug/
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release/
)


