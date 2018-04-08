# ext/CMakeLists.txt

if( BUILD_EXT_EXAMPLE )
	add_subdirectory( apps )
endif()

if( TEST_SWITCH )
    add_subdirectory( tests )
endif()



