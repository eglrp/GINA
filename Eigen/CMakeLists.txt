
set( EIGEN_CORE_INC_DIRS "${PROJECT_SOURCE_DIR}/Eigen/eigen-3.3.4" )

##################
# Set Properties #
##################
#####################################################################################################################################
set_property(DIRECTORY "${PROJECT_SOURCE_DIR}/Eigen" PROPERTY EIGEN_CORE_INC_DIRS ${EIGEN_CORE_INC_DIRS})
