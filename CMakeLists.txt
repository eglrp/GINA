cmake_minimum_required (VERSION 2.91.0)

include(${CMAKE_CURRENT_SOURCE_DIR}/99_CMake/definition.cmake) 
project (GINA)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}")



# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# Set directory definitions in the GINAConfig.h
set (ROOT_SRC_DIR "${CMAKE_SOURCE_DIR}")
change_slash2double_backslash(ROOT_SRC_DIR)
set (GINA_PARSER_TrajectoryData "${CMAKE_SOURCE_DIR}/68_GINAParser/include/TrajectoryParser/TrajectoryData.hpp")
change_slash2backslash(GINA_PARSER_TrajectoryData)
set (GINA_PARSER_TrajectoryStream "${CMAKE_SOURCE_DIR}/68_GINAParser/include/TrajectoryParser/TrajectoryStream.hpp") 
change_slash2backslash(GINA_PARSER_TrajectoryStream)
set (GINA_PARSER_IMUData "${CMAKE_SOURCE_DIR}/68_GINAParser/include/IMUParser/IMUData.hpp")
change_slash2backslash(GINA_PARSER_IMUData)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/GINAConfig.h.in"
  "${PROJECT_SOURCE_DIR}/99_CMake/GINAConfig.h"
  )

# add the binary tree to the search path for include files
# so that we will find GINAConfig.h
include_directories ("${PROJECT_BINARY_DIR}")
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/99_CMake")

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
file( GLOB_RECURSE GPSTK_EXT_INC_FILES "20_GPSTk-master/ext/lib/*.h" "20_GPSTk-master/ext/lib/*.hpp" )
  
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

#GINAPArser
##############################################################
option( test_GINAParser "HELP: Build test of GINA Parser: SWITCH, Default = ON, Build the GINAPArser" OFF )
add_subdirectory (68_GINAPArser) 
get_property(GINAPARSER_CORE_INC_DIRS DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/68_GINAParser" PROPERTY GINAPARSER_CORE_INC_DIRS)
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
option( GNSS_Competence_Team_Project_2 "HELP: BUILD_EXC1: SWITCH, Default = ON, Build the GNSS Competence Team - 2nd exercise" OFF )
add_subdirectory (40_GNSS_Competence_Team) 

add_subdirectory (69_GINA)



