file(GLOB GNSSSIM_SRC "src/*.cpp")
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
  

  # append ext include files needed to build library target


include_directories (include)
add_executable(GNSSSimulator ${GNSSSIM_SRC} ${GNSSSIM_INCL} )
target_link_libraries (GNSSSimulator	PUBLIC ${GPSTK_LIBS}
										PUBLIC gpstk 
										PUBLIC PINAParser)
target_include_directories(GNSSSimulator PUBLIC ${GNSSSim_INC_DIRS} ${GPSTK_CORE_INC_DIRS} ${GPSTK_EXT_INC_DIRS} ${PINAPARSER_CORE_INC_DIRS} )

# Create VS Filters	
set( GNSS_All_FILES "" )	
list( APPEND GNSS_All_FILES ${GNSSSIM_SRC})
list( APPEND GNSS_All_FILES ${GNSSSIM_INCL})					
assign_source_group(${GNSS_All_FILES})

##################
# Set Properties #
##################
#####################################################################################################################################
set( GNSSSIM_INCLUDE_DIRS "" )
set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY GNSSSIM_INCLUDE_DIRS ${GNSSSim_INC_DIRS})












