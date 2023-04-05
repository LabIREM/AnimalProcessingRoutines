# define cmake minimum requirement
cmake_minimum_required ( VERSION 2.8 )


# project information
project(Projection)



## Global Libraries ###########################################################################

# Set and define required libraries for all projects



# ITK Libraries

find_package(ITK REQUIRED)
if(ITK_FOUND)
	include(${ITK_USE_FILE})
endif()



# Boost Libraries

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs

set(Boost_USE_MULTITHREADED      ON)

set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.64 COMPONENTS filesystem system program_options REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    
	include_directories(${Boost_LIBRARY_DIRS})
endif() 



##############################################################################################



#
# Add Subdirectories ########################################################################

# List and calls all subdirectories for cmake call propagation 



include_directories(include)

include_directories(external)

add_subdirectory(zmapGeneration)

add_subdirectory(zmapProjection)

