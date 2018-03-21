cmake_minimum_required (VERSION 2.6)
project (PINA)

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
  "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
  "${PROJECT_BINARY_DIR}/TutorialConfig.h"
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

###############################################################

#IMUSimulator & IMUSimulatorLibrary
#include_directories ("${PROJECT_SOURCE_DIR}/IMUSimulator/include")
add_subdirectory (IMUSimulator)
set (EXTRA_LIBS ${EXTRA_LIBS} IMUSimulatorLibrary)

add_subdirectory (GNSSsimulator)


option( BUILD_EXC1 "HELP: BUILD_EXC1: SWITCH, Default = ON, Build the GNSS Competence Team - 1st exercise" ON )
add_subdirectory (GNSS_Competence_Team) 

add_subdirectory (PINA)



