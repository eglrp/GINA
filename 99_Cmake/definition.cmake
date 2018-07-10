

# Function to set the filter in visual studio
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

function(change_slash2backslash varName)
	if (WIN32)
		STRING(REGEX REPLACE "/" "\\\\"  ${varName} ${${varName}}) 
		SET(${varName} ${${varName}} PARENT_SCOPE)
	endif()
endfunction(change_slash2backslash)

function(change_slash2double_backslash varName)
	if (WIN32)
		STRING(REGEX REPLACE "/" "\\\\\\\\"  ${varName} ${${varName}}) 
		SET(${varName} ${${varName}} PARENT_SCOPE)
	endif()
endfunction(change_slash2double_backslash)