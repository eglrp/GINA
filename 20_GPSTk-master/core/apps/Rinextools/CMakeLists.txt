# apps/Rinextools/CMakeLists.txt

if( BUILD_EXT )
	add_executable(RinDump RinDump.cpp)
	target_link_libraries(RinDump gpstk)
	install (TARGETS RinDump DESTINATION "${CMAKE_INSTALL_BINDIR}")
endif()
