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

##########
# ALGLIB #
##########
#################################################################

file(GLOB_RECURSE ALGLIB_INCL "ALGLIB/src/*.h")
set( ALGLIB_CORE_INC_DIRS "" )

foreach( _headerFile ${ALGLIB_INCL} )
    get_filename_component( _dir ${_headerFile} PATH )
    list( APPEND ALGLIB_CORE_INC_DIRS ${_dir} )
endforeach()

list( REMOVE_DUPLICATES ALGLIB_CORE_INC_DIRS )

add_subdirectory (ALGLIB)
set (EXTRA_LIBS ${EXTRA_LIBS} ALGLIB)


#########
# GPSTK #
#########
#################################################################

#add_subdirectory (GPSTk-master)
file( GLOB_RECURSE GPSTK_CORE_INC_FILES "GPSTk-master/core/lib/*.hpp" )
file( GLOB_RECURSE GPSTK_EXT_INC_FILES "ext/lib/*.h" "ext/lib/*.hpp" )
  
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


add_subdirectory (GPSTk-master)
set (EXTRA_LIBS ${EXTRA_LIBS} gpstk)
set (GPSTK_LIBS ${GPSTK_LIBS} gpstk)

# Set header files and include directories to use eigen lib
##############################################################


 
#set( Eigen3_DIR "" )
#SET ( "$ENV{Eigen3_DIR}"  C:/Users/LUS2BP/Source/Repos/PINA/Eigen/eigen-3.3.4")
#set (EIGEN3_INCLUDE_DIR "C:/Users/LUS2BP/Source/Repos/PINA/Eigen/eigen-3.3.4")
 # C:/Users/LUS2BP/Source/Repos/PINA/Eigen/eigen-3.3.4


#SET( EIGEN3_INCLUDE_DIR "$ENV{EIGEN3_INCLUDE_DIR}" )
#set(EIGEN3_INCLUDE_DIR "C:/Users/LUS2BP/Source/Repos/PINA/Eigen/eigen-3.3.4/cmake")
#IF( NOT EIGEN3_INCLUDE_DIR )
#    MESSAGE( FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
#ENDIF()
#INCLUDE_DIRECTORIES ( "${EIGEN3_INCLUDE_DIR}" )

#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
#find_package(Eigen3 REQUIRED)
#include_directories(${EIGEN3_INCLUDE_DIR})
  
  
######################

#find_package(Eigen3 QUIET)
#if(NOT Eigen_FOUND)
#  include(${CMAKE_SOURCE_DIR}/FindEigen3.cmake)
#  set(Eigen_INCLUDE_DIR ${EIGEN3_INCLUDE_DIR})
#endif()
#include_directories(${Eigen_INCLUDE_DIR})


###################
#message("Eigen3_DIR = ${Eigen3_DIR}")
#set(Eigen3_DIR "C:/Users/LUS2BP/Source/Repos/PINA/Eigen/Build/eigen-3.3.4")
message("Eigen3_DIR = ${Eigen3_DIR}")
#find_package (Eigen3 3.3 REQUIRED NO_MODULE)

set( EIGEN_CORE_INC_DIRS_temp "" )
set( EIGEN_INC_FILES "" )

set( EIGEN_CORE_INC_DIRS "${PROJECT_SOURCE_DIR}/Eigen/eigen-3.3.4" )

file( GLOB_RECURSE EIGEN_CORE_INC_DIRS_temp "Eigen/eigen-3.3.4/Eigen/src/*.h" "Eigen/eigen_334/Eigen/src/*.hpp" )

foreach( _headerFile ${EIGEN_CORE_INC_DIRS_temp} )
    get_filename_component( _dir ${_headerFile} PATH )
    list( APPEND EIGEN_CORE_INC_DIRS_temp ${_dir} )
endforeach()

#list( APPEND EIGEN_INC_FILES ${EIGEN_CORE_INC_DIRS_temp} )

#add_subdirectory(Eigen)
message("Eigen3_DIR = ${Eigen3_DIR}")

###############################################################

#IMUSimulator & IMUSimulatorLibrary
#include_directories ("${PROJECT_SOURCE_DIR}/IMUSimulator/include")
add_subdirectory (IMUSimulator)
set (EXTRA_LIBS ${EXTRA_LIBS} IMUSimulatorLibrary)

add_subdirectory (GNSSsimulator)



option( BUILD_EXC1 "HELP: BUILD_EXC1: SWITCH, Default = ON, Build the GNSS Competence Team - 1st exercise" ON )
add_subdirectory (GNSS_Competence_Team) 

add_subdirectory (PINA)



