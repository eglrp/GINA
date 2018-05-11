
file( GLOB_RECURSE PINAPARSER_CORE_SRC_FILES "src/*.cpp" "src/*.c" )
file( GLOB_RECURSE PINAPARSER_CORE_INC_FILES "include/*.h" "include/*.hpp" )
file( GLOB_RECURSE PINAPARSER_TEST_SRC_FILES "tests/*.cpp" "tests/*.c" )
file( GLOB_RECURSE PINAPARSER_TEST_INC_FILES "tests/*.h" "tests/*.hpp" )

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

# Test
foreach( _headerFile ${PINAPARSER_TEST_INC_FILES} )
    get_filename_component( _dir ${_headerFile} PATH )
    list( APPEND PINAPARSER_CORE_INC_DIRS ${_dir} )
endforeach()

list( REMOVE_DUPLICATES PINAPARSER_CORE_INC_DIRS )

# Add every directory containing a header file
# to the project(IMUSimulator) include_directories


# define src and include files needed to build library target
set( PINAPARSER_TEST_SRC "" )
set( PINAPARSER_TEST_INC "" )

list( APPEND PINAPARSER_TEST_SRC ${PINAPARSER_CORE_SRC_FILES} )
list( APPEND PINAPARSER_TEST_SRC ${PINAPARSER_TEST_SRC_FILES} )
list( APPEND PINAPARSER_TEST_INC ${PINAPARSER_CORE_INC_FILES} )
list( APPEND PINAPARSER_TEST_INC ${PINAPARSER_TEST_INC_FILES} )

#set(PINAPARSER_LIB_CORE_SRC_FILES ${PINAPARSER_TEST_SRC_FILES})
#list(REMOVE_ITEM PINAPARSER_LIB_CORE_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/PINAParser.cpp)

#message("${CMAKE_CURRENT_SOURCE_DIR}/PINAParser.cpp: " ${CMAKE_CURRENT_SOURCE_DIR}/PINAParser.cpp)
#message("PINAPARSER_LIB_CORE_SRC_FILES}): " ${PINAPARSER_LIB_CORE_SRC_FILES})

include_directories( ${PINAPARSER_CORE_INC_DIRS} ${GPSTK_CORE_INC_DIRS})

# add the Test
if(test_PINAParser)
	add_executable(test_PINAParser ${PINAPARSER_SRC_FILES} ${PINAPARSER_TEST_SRC} ${PINAPARSER_INC_FILES})
	target_link_libraries (test_PINAParser PUBLIC gpstk)		
	target_include_directories(test_PINAParser PUBLIC ${PINAPARSER_CORE_INC_DIRS} ${PINAPARSER_TEST_INC} ${GPSTK_CORE_INC_DIRS})	
endif()

# PINAPArser library build target
add_library(PINAParser	SHARED 	${PINAPARSER_LIB_CORE_SRC_FILES}
								${PINAPARSER_INC_FILES})
target_link_libraries (PINAParser PUBLIC gpstk)		
target_include_directories(test_PINAParser PUBLIC ${PINAPARSER_CORE_INC_DIRS} ${GPSTK_CORE_INC_DIRS})	

# add the executable
#add_executable(PINAParser ${PINAPARSER_SRC_FILES} ${PINAPARSER_INC_FILES})
#target_link_libraries (PINAParser PUBLIC gpstk)							

# Create VS Filters	
set( PINAPARSER_All_FILES "" )	
list( APPEND PINAPARSER_All_FILES ${PINAPARSER_SRC_FILES})
list( APPEND PINAPARSER_All_FILES ${PINAPARSER_TEST_SRC})
list( APPEND PINAPARSER_All_FILES ${PINAPARSER_TEST_INC})
list( APPEND PINAPARSER_All_FILES ${PINAPARSER_INC_FILES})					
assign_source_group(${PINAPARSER_All_FILES})

##################
# Set Properties #
##################
#####################################################################################################################################
set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY PINAPARSER_CORE_INC_DIRS ${PINAPARSER_CORE_INC_DIRS})






















