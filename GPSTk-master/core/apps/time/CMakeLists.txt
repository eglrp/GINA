# apps/time/CMakeLists.txt


if( BUILD_EXT )
	add_executable(timeconvert timeconvert.cpp)
	target_link_libraries(timeconvert gpstk)
	install (TARGETS timeconvert DESTINATION "${CMAKE_INSTALL_BINDIR}")
endif()


