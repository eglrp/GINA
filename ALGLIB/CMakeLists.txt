# This sets up variables contining GNU standard installation locations.
include( GNUInstallDirs )

# For now, override to use 'lib' for the library install dir.
set( CMAKE_INSTALL_LIBDIR lib )

# Set a filename for collecting exported targets.
set( EXPORT_TARGETS_FILENAME "ALGLIBTargets" )


file(GLOB_RECURSE ALGLIB_SOURCES "src/*.cpp")
#file(GLOB_RECURSE ALGLIB_INCL "src/*.h")


# Add every directory containing a header file
# to the project(ALGLIB) include_directories
include_directories( ${ALGLIB_CORE_INC_DIRS} )


add_library(ALGLIB STATIC ${ALGLIB_SOURCES}
						   ${ALGLIB_INCL})

# ALGLIB library install target
install( TARGETS ALGLIB DESTINATION "${CMAKE_INSTALL_LIBDIR}" EXPORT "${EXPORT_TARGETS_FILENAME}" )

# ALGLIB header file install target
install( FILES ${ALGLIB_INCL} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" )
