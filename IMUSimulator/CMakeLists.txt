
file( GLOB_RECURSE IMU_CORE_SRC_FILES "src/*.cpp" "src/*.c" )
file( GLOB_RECURSE IMU_CORE_INC_FILES "include/*.h" "include/*.hpp" )

#----------------------------------------
# Define /core library include directories
#----------------------------------------

# initialize list of include directories
set( IMU_CORE_INC_DIRS "" )

foreach( _headerFile ${IMU_CORE_INC_FILES} )
    get_filename_component( _dir ${_headerFile} PATH )
    list( APPEND IMU_CORE_INC_DIRS ${_dir} )
endforeach()



list( REMOVE_DUPLICATES IMU_CORE_INC_DIRS )

# Add every directory containing a header file
# to the project(IMUSimulator) include_directories
include_directories( ${IMU_CORE_INC_DIRS} ${ALGLIB_CORE_INC_DIRS} ${GPSTK_CORE_INC_DIRS} ${EIGEN_CORE_INC_DIRS})


# define src and include files needed to build library target
set( IMU_SRC_FILES "" )
set( IMU_INC_FILES "" )

list( APPEND IMU_SRC_FILES ${IMU_CORE_SRC_FILES} )
list( APPEND IMU_INC_FILES ${IMU_CORE_INC_FILES} )
list( APPEND IMU_INC_FILES ${EIGEN_INC_FILES} )

# IMUSimulator library build target
add_library(IMUSimulator	STATIC 	${IMU_SRC_FILES}
									${IMU_INC_FILES})


# add the executable
add_executable(IMUSimulatorProject ${IMU_SRC_FILES} ${IMU_INC_FILES})
target_link_libraries (IMUSimulatorProject 	PUBLIC gpstk 
											PRIVATE ALGLIB)
									

# Create VS Filters	
set( IMU_All_FILES "" )	
list( APPEND IMU_All_FILES ${IMU_SRC_FILES})
list( APPEND IMU_All_FILES ${IMU_INC_FILES})					
assign_source_group(${IMU_All_FILES})


if(IMUSimulator_Tests)
	add_subdirectory(tests)
endif()
