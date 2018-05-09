cmake_minimum_required (VERSION 2.91.0)
project (PINA)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}")

# Function to set the filter in visual studio
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)


set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/PINAConfig.h.in"
  "${PROJECT_BINARY_DIR}/PINAConfig.h"
  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories ("${PROJECT_BINARY_DIR}")

#################
# Math Packages #
#################
#################################################################
add_subdirectory (10_MathPckgs)

	##########
	# ALGLIB #
	##########
	#################################################################
	get_property(ALGLIB_CORE_INC_DIRS DIRECTORY "${PROJECT_SOURCE_DIR}/10_MathPckgs/ALGLIB" PROPERTY ALGLIB_CORE_INC_DIRS)
	set (EXTRA_LIBS ${EXTRA_LIBS} ALGLIB)
	#message("Property for root directory root read from ALGLIB_CORE_INC_DIRS: " ${ALGLIB_CORE_INC_DIRS})

	############
	#Eigen Lib #
	############
	#################################################################
	get_property(EIGEN_CORE_INC_DIRS DIRECTORY "${PROJECT_SOURCE_DIR}/10_MathPckgs/Eigen" PROPERTY EIGEN_CORE_INC_DIRS)
	#message("Property for root directory root read from EIGEN_CORE_INC_DIRS: " ${EIGEN_CORE_INC_DIRS})

#########
# GPSTK #
#########
#################################################################

#add_subdirectory (20_GPSTk-master)
file( GLOB_RECURSE GPSTK_CORE_INC_FILES "20_GPSTk-master/core/lib/*.hpp" )
file( GLOB_RECURSE GPSTK_EXT_INC_FILES "20_GPSTk-master/ext/lib/*.h" "GPSTk-master/ext/lib/*.hpp" )
  
#----------------------------------------
# Define /core library include directories
#----------------------------------------

# initialize list of include directories
set( GPSTK_CORE_INC_DIRS "" )

foreach( _headerFile ${GPSTK_CORE_INC_FILES} )
    get_filename_component( _dir ${_headerFile} PATH )
    list( APPEND GPSTK_CORE_INC_DIRS ${_dir} )
endforeach()


list( REMOVE_DUPLICATES GPSTK_CORE_INC_DIRS )

set( GPSTK_EXT_INC_DIRS "" )

 foreach( _headerFile ${GPSTK_EXT_INC_FILES} )
   get_filename_component( _dir ${_headerFile} PATH )
   get_filename_component( _name ${_headerFile} NAME )
   list( APPEND GPSTK_EXT_INC_DIRS ${_dir} )
 endforeach()

  list( REMOVE_DUPLICATES GPSTK_EXT_INC_DIRS )

add_subdirectory (20_GPSTk-master)
set (EXTRA_LIBS ${EXTRA_LIBS} gpstk_extra)
set (GPSTK_LIBS ${GPSTK_LIBS} gpstk)

#PINAPArser
##############################################################
option( PINAParser "HELP: Build Pina Parser: SWITCH, Default = ON, Build the PINAPArser" ON )
add_subdirectory (68_PINAPArser) 
get_property(PINAPARSER_CORE_INC_DIRS DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/68_PINAParser" PROPERTY PINAPARSER_CORE_INC_DIRS)
# message("Property for root directory root read from GNSSSIM_INCLUDE_DIRS: " ${GNSSSIM_INCLUDE_DIRS})

#GNSSsimulator
##############################################################
add_subdirectory (50_GNSSsimulator)
get_property(GNSSSIM_INCLUDE_DIRS DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/50_GNSSSimulator" PROPERTY GNSSSIM_INCLUDE_DIRS)
# message("Property for root directory root read from GNSSSIM_INCLUDE_DIRS: " ${GNSSSIM_INCLUDE_DIRS})
 
#IMUSimulator & IMUSimulatorLibrary
##############################################################
option( IMUSimulator_Tests "HELP: IMUSimulatorTest: Default = OFF, Create tests projects of the IMUSimulator." OFF )
add_subdirectory (60_IMUSimulator)
set (EXTRA_LIBS ${EXTRA_LIBS} IMUSimulatorLibrary)

# GNSS Competence Team's Projects
##############################################################
option( GNSS_Competence_Team_Project_1 "HELP: BUILD_EXC1: SWITCH, Default = ON, Build the GNSS Competence Team - 1st exercise" OFF )
add_subdirectory (40_GNSS_Competence_Team) 

add_subdirectory (69_PINA)



