
# trajectory_tests
add_executable(trajectory_tests Trajectory_Tests.cpp)
target_link_libraries (trajectory_tests 	PUBLIC gpstk 
											PRIVATE ALGLIB
											IMUSimulator)
											
# strapdown_ecef_test
add_executable(strapdown_ecef_test strapdown_ecef_test.cpp)
target_link_libraries (strapdown_ecef_test 	PUBLIC gpstk 
											PRIVATE ALGLIB
											IMUSimulator)
											
											
											