file(GLOB GNSSSIM_SRC src/*.cpp)
file(GLOB GNSSSIM_INCL "include/*.h" "include/*.hpp")



  # Define ext library include directories
  set( GNSSSim_INC_DIRS "" )

  foreach( _headerFile ${GNSSSIM_INCL} )
    get_filename_component( _dir ${_headerFile} PATH )
    get_filename_component( _name ${_headerFile} NAME )
    list( APPEND GNSSSim_INC_DIRS ${_dir} )
  endforeach()

  list( REMOVE_DUPLICATES GNSSSim_INC_DIRS )

  # Add every directory containing a header file
  # to the project(gpstk) include_directories
  include_directories( ${GNSSSim_INC_DIRS} ${GPSTK_CORE_INC_DIRS} ${GPSTK_EXT_INC_DIRS})

  # append ext include files needed to build library target

#Add GNSSim root directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}) 
#set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

include_directories (include)
add_executable(GNSSSimulator ${GNSSSIM_SRC} ${GNSSSIM_INCL} )
target_link_libraries (GNSSSimulator  ${GPSTK_LIBS})