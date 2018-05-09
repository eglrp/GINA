
file( GLOB_RECURSE PINAPARSER_CORE_SRC_FILES "src/*.cpp" "src/*.c" )
file( GLOB_RECURSE PINAPARSER_CORE_INC_FILES "include/*.h" "include/*.hpp" )

#----------------------------------------
# Define /core library include directories
#----------------------------------------

# initialize list of include directories
set( PINAPARSER_CORE_INC_DIRS "" )

foreach( _headerFile ${PINAPARSER_CORE_INC_FILES} )
    get_filename_component( _dir ${_headerFile} PATH )
    list( APPEND PINAPARSER_CORE_INC_DIRS ${_dir} )
endforeach()

list( REMOVE_DUPLICATES PINAPARSER_CORE_INC_DIRS )

# Add every directory containing a header file
# to the project(IMUSimulator) include_directories
include_directories( ${PINAPARSER_CORE_INC_DIRS} ${GPSTK_CORE_INC_DIRS})

# define src and include files needed to build library target
set( PINAPARSER_SRC_FILES "" )
set( PINAPARSER_INC_FILES "" )

list( APPEND PINAPARSER_SRC_FILES ${PINAPARSER_CORE_SRC_FILES} )
list( APPEND PINAPARSER_INC_FILES ${PINAPARSER_CORE_INC_FILES} )

# add the executable
add_executable(PINAParser ${PINAPARSER_SRC_FILES} ${PINAPARSER_INC_FILES})
target_link_libraries (PINAParser PUBLIC gpstk)							

# Create VS Filters	
set( PINAPARSER_All_FILES "" )	
list( APPEND PINAPARSER_All_FILES ${PINAPARSER_SRC_FILES})
list( APPEND PINAPARSER_All_FILES ${PINAPARSER_INC_FILES})					
assign_source_group(${PINAPARSER_All_FILES})

##################
# Set Properties #
##################
#####################################################################################################################################
set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY PINAPARSER_CORE_INC_DIRS ${PINAPARSER_CORE_INC_DIRS})






















