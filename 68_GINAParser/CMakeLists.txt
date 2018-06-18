
file( GLOB_RECURSE GINAPARSER_CORE_SRC_FILES "src/*.cpp" "src/*.c" )
file( GLOB_RECURSE GINAPARSER_CORE_INC_FILES "include/*.h" "include/*.hpp" )
file( GLOB_RECURSE GINAPARSER_TEST_SRC_FILES "tests/*.cpp" "tests/*.c" )
file( GLOB_RECURSE GINAPARSER_TEST_INC_FILES "tests/*.h" "tests/*.hpp" )

#----------------------------------------
# Define /core library include directories
#----------------------------------------

# initialize list of include directories
set( GINAPARSER_CORE_INC_DIRS "" )

foreach( _headerFile ${GINAPARSER_CORE_INC_FILES} )
    get_filename_component( _dir ${_headerFile} PATH )
    list( APPEND GINAPARSER_CORE_INC_DIRS ${_dir} )
endforeach()

list( REMOVE_DUPLICATES GINAPARSER_CORE_INC_DIRS )

# Add every directory containing a header file
# to the project(IMUSimulator) include_directories
include_directories( ${GINAPARSER_CORE_INC_DIRS} ${GPSTK_CORE_INC_DIRS})

# define src and include files needed to build library target
set( GINAPARSER_SRC_FILES "" )
set( GINAPARSER_INC_FILES "" )
set( GINAPARSER_TEST_INC_DIRS "" )

list( APPEND GINAPARSER_SRC_FILES ${GINAPARSER_CORE_SRC_FILES} )
list( APPEND GINAPARSER_INC_FILES ${GINAPARSER_CORE_INC_FILES} )

# Test
foreach( _headerFile ${GINAPARSER_TEST_INC_FILES} )
    get_filename_component( _dir ${_headerFile} PATH )
    list( APPEND GINAPARSER_TEST_INC_DIRS ${_dir} )
endforeach()

list( REMOVE_DUPLICATES GINAPARSER_TEST_INC_DIRS )

# Add every directory containing a header file
# to the project(IMUSimulator) include_directories


# define src and include files needed to build library target
set( GINAPARSER_TEST_SRC "" )
set( GINAPARSER_TEST_INC "" )

list( APPEND GINAPARSER_TEST_SRC ${GINAPARSER_CORE_SRC_FILES} )
list( APPEND GINAPARSER_TEST_SRC ${GINAPARSER_TEST_SRC_FILES} )
list( APPEND GINAPARSER_TEST_INC ${GINAPARSER_CORE_INC_FILES} )
list( APPEND GINAPARSER_TEST_INC ${GINAPARSER_TEST_INC_FILES} )

#set(GINAPARSER_LIB_CORE_SRC_FILES ${GINAPARSER_TEST_SRC_FILES})
#list(REMOVE_ITEM GINAPARSER_LIB_CORE_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/GINAParser.cpp)

#message("${CMAKE_CURRENT_SOURCE_DIR}/GINAParser.cpp: " ${CMAKE_CURRENT_SOURCE_DIR}/GINAParser.cpp)
#message("GINAPARSER_LIB_CORE_SRC_FILES}): " ${GINAPARSER_LIB_CORE_SRC_FILES})

include_directories( ${GINAPARSER_CORE_INC_DIRS} ${GPSTK_CORE_INC_DIRS})

# add the Test
if(test_GINAParser)
	add_executable(test_GINAParser ${GINAPARSER_SRC_FILES} ${GINAPARSER_TEST_SRC} ${GINAPARSER_INC_FILES})
	target_link_libraries (test_GINAParser PUBLIC gpstk)		
	target_include_directories(test_GINAParser PUBLIC ${GINAPARSER_CORE_INC_DIRS} ${GINAPARSER_TEST_INC_DIRS} ${GINAPARSER_TEST_INC} ${GPSTK_CORE_INC_DIRS})	
endif()

# GINAPArser library build target
add_library(GINAParser	STATIC 	${GINAPARSER_SRC_FILES}
								${GINAPARSER_INC_FILES})
target_link_libraries (GINAParser PUBLIC gpstk)		
target_include_directories(GINAParser PUBLIC ${GINAPARSER_CORE_INC_DIRS} ${GPSTK_CORE_INC_DIRS})	

# add the executable
#add_executable(GINAParserproj ${GINAPARSER_SRC_FILES} ${GINAPARSER_INC_FILES})
#target_link_libraries (GINAParserproj PUBLIC gpstk)							

# Create VS Filters	
set( GINAPARSER_All_FILES "" )	
list( APPEND GINAPARSER_All_FILES ${GINAPARSER_SRC_FILES})
list( APPEND GINAPARSER_All_FILES ${GINAPARSER_TEST_SRC})
list( APPEND GINAPARSER_All_FILES ${GINAPARSER_TEST_INC})
list( APPEND GINAPARSER_All_FILES ${GINAPARSER_INC_FILES})					
assign_source_group(${GINAPARSER_All_FILES})

##################
# Set Properties #
##################
#####################################################################################################################################
set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY GINAPARSER_CORE_INC_DIRS ${GINAPARSER_CORE_INC_DIRS})






















