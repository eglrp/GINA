
set( EIGEN_CORE_INC_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/eigen-3.3.4" )

##################
# Set Properties #
##################
#####################################################################################################################################
set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY EIGEN_CORE_INC_DIRS ${EIGEN_CORE_INC_DIRS})
