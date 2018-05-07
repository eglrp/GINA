# This sets up variables contining GNU standard installation locations.
include( GNUInstallDirs )

# For now, override to use 'lib' for the library install dir.
set( CMAKE_INSTALL_LIBDIR lib )

# Set a filename for collecting exported targets.
set( EXPORT_TARGETS_FILENAME "ALGLIBTargets" )


file(GLOB_RECURSE ALGLIB_SOURCES "src/*.cpp")
file(GLOB_RECURSE ALGLIB_INCL "src/*.h")

set( ALGLIB_CORE_INC_DIRS "" )

foreach( _headerFile ${ALGLIB_INCL} )
    get_filename_component( _dir ${_headerFile} PATH )
    list( APPEND ALGLIB_CORE_INC_DIRS ${_dir} )
endforeach()

list( REMOVE_DUPLICATES ALGLIB_CORE_INC_DIRS )

#message("Property read from root ALGLIB_CORE_INC_DIRS. From sub: " ${ALGLIB_CORE_INC_DIRS})

# Add every directory containing a header file
# to the project(ALGLIB) include_directories
include_directories( ${ALGLIB_CORE_INC_DIRS} )


add_library(ALGLIB STATIC ${ALGLIB_SOURCES}
						   ${ALGLIB_INCL})

# ALGLIB library install target
install( TARGETS ALGLIB DESTINATION "${CMAKE_INSTALL_LIBDIR}" EXPORT "${EXPORT_TARGETS_FILENAME}" )

# ALGLIB header file install target
install( FILES ${ALGLIB_INCL} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" )

##################
# Set Properties #
##################
#####################################################################################################################################
set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY ALGLIB_CORE_INC_DIRS ${ALGLIB_CORE_INC_DIRS})







