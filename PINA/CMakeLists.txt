
# PINA
add_executable (PINA MainPINA.cpp)

target_include_directories(PINA PUBLIC "${PROJECT_SOURCE_DIR}/IMUSimulator/include")

set( PINA_CORE_INC_DIRS "" )
list( APPEND PINA_CORE_INC_DIRS ${IMU_CORE_INC_DIRS} )

target_include_directories(PINA PUBLIC ${PINA_CORE_INC_DIRS} ${ALGLIB_CORE_INC_DIRS} ${GPSTK_CORE_INC_DIRS})

target_link_libraries (PINA gpstk IMUSimulator ALGLIB gpstk)

# add the install targets
install (TARGETS PINA DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
  DESTINATION include)