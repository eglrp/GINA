file(GLOB SRC "src/*.cpp" "src/*.c" )
file(GLOB INCL "incl/*.h" "incl/*.hpp")



# Define ext library include directories
  set( INC_DIRS "" )

  foreach( _headerFile ${INCL} )
    get_filename_component( _dir ${_headerFile} PATH )
    get_filename_component( _name ${_headerFile} NAME )
    list( APPEND INC_DIRS ${_dir} )
  endforeach()

  list( REMOVE_DUPLICATES INC_DIRS )
  
  
  
  # define src and include files needed to build library target
set( SRC_FILES "" )
set( INC_FILES "" )
list( APPEND SRC_FILES ${SRC} )
list( APPEND INC_FILES ${INCL} )


include_directories( ${INC_DIRS} ${GPSTK_CORE_INC_DIRS} ${ALGLIB_CORE_INC_DIRS})

add_executable (Exc1_Schreiber ${SRC_FILES} ${INC_FILES})
target_link_libraries (Exc1_Schreiber gpstk ALGLIB gpstk)