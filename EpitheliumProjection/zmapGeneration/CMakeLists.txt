# define cmake minimum requirement
cmake_minimum_required ( VERSION 2.8 )

# project info
project(zmapGeneration) 

## source files #####################################################################
# set sources and headers files


set( source
main.cpp 
)

set( header
../include/itkImageProjectionFromTopologicalMapFilter.h
../include/itkImageProjectionFromTopologicalMapFilter.hxx
../include/itkMultiscaleVolumeToTopologicalDepthMapFilter.h
../include/itkMultiscaleVolumeToTopologicalDepthMapFilter.hxx
../include/itkVolumeDataToTopologicalDepthMapFilter.h
../include/itkVolumeDataToTopologicalDepthMapFilter.hxx
../include/LocalPeakDetection1D.h
../include/LocalPeakDetection1D.hxx
)

set( external
../external/itkVarianceImageFilter.h
../external/itkVarianceImageFilter.hxx
../external/itkFilterWatcher.h
)

## executable #######################################################################
# define executable and link libraries

add_executable( zmapGeneration ${source} ${header} ${external} )


target_link_libraries( zmapGeneration ${ITK_LIBRARIES} ${Boost_LIBRARIES} )



set_target_properties(zmapGeneration PROPERTIES
                      
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug/
                      
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release/
)


