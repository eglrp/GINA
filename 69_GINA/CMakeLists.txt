
file(GLOB GINA_SRC src/*.cpp)
file(GLOB GINA_INCL "include/*.h" "include/*.hpp")

# GINA
add_executable (GINA ${GINA_SRC} ${GINA_INCL})

target_include_directories(GINA PUBLIC "${PROJECT_SOURCE_DIR}/60_IMUSimulator/include")

set( GINA_CORE_INC_DIRS "" )
list( APPEND GINA_CORE_INC_DIRS ${IMU_CORE_INC_DIRS} )

target_include_directories(GINA PUBLIC ${GINA_CORE_INC_DIRS} ${ALGLIB_CORE_INC_DIRS} ${GPSTK_CORE_INC_DIRS})

target_link_libraries (GINA gpstk IMUSimulator ALGLIB)

# add the install targets
install (TARGETS GINA DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
  DESTINATION include)
  
# Create VS Filters	
set( GINA_All_FILES "" )	
list( APPEND GINA_All_FILES ${GINA_SRC})
list( APPEND GINA_All_FILES ${GINA_INCL})					
assign_source_group(${GINA_All_FILES})