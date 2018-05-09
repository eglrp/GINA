
file(GLOB PINA_SRC src/*.cpp)
file(GLOB PINA_INCL "include/*.h" "include/*.hpp")

# PINA
add_executable (PINA ${PINA_SRC} ${PINA_INCL})

target_include_directories(PINA PUBLIC "${PROJECT_SOURCE_DIR}/60_IMUSimulator/include")

set( PINA_CORE_INC_DIRS "" )
list( APPEND PINA_CORE_INC_DIRS ${IMU_CORE_INC_DIRS} )

target_include_directories(PINA PUBLIC ${PINA_CORE_INC_DIRS} ${ALGLIB_CORE_INC_DIRS} ${GPSTK_CORE_INC_DIRS})

target_link_libraries (PINA gpstk IMUSimulator ALGLIB)

# add the install targets
install (TARGETS PINA DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
  DESTINATION include)
  
# Create VS Filters	
set( PINA_All_FILES "" )	
list( APPEND PINA_All_FILES ${PINA_SRC})
list( APPEND PINA_All_FILES ${PINA_INCL})					
assign_source_group(${PINA_All_FILES})