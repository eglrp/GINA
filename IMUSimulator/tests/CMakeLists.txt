
# trajectory_test
add_executable(test_trajectory Trajectory_test.cpp)
target_link_libraries (test_trajectory 	PUBLIC gpstk 
										PRIVATE ALGLIB
										IMUSimulator)
											
# strapdown_ecef_test
add_executable(test_strapdown_ecef strapdown_ecef_test.cpp)
target_link_libraries (test_strapdown_ecef 	PUBLIC gpstk 
											PRIVATE ALGLIB
											IMUSimulator)
											
											
# IMUStore_test
add_executable(test_IMUStore IMUStore_test.cpp)
target_link_libraries (test_IMUStore 	PUBLIC gpstk 
										PRIVATE ALGLIB
										IMUSimulator)
										
										
# IMUSignalGenerator_test
add_executable(test_IMUSignalGenerator IMUSignalGenerator_test.cpp)
target_link_libraries (test_IMUSignalGenerator 	PUBLIC gpstk 
												PRIVATE ALGLIB
												IMUSimulator)
											
											
											