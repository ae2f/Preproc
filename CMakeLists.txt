cmake_minimum_required(VERSION 3.20)

project(Preproc VERSION 1.1.4)

if(DEFINED ae2f_PreProc_C_COMPILER)
	set(ae2f_PreProc_C_PRM "-DCMAKE_C_COMPILER=${ae2f_PreProc_C_COMPILER}")
else()
	set(ae2f_PreProc_C_PRM "")
endif()

set(ae2f_Macro_ROOT "${PROJECT_SOURCE_DIR}/Macro" CACHE STRING "${PROJECT_SOURCE_DIR}/Macro")
set(ae2f_Inc_ROOT "${PROJECT_SOURCE_DIR}/Inc" CACHE STRING "${PROJECT_SOURCE_DIR}/Inc")
set(ae2f_FileRaw_ROOT "${PROJECT_SOURCE_DIR}/FileRaw" CACHE STRING "${PROJECT_SOURCE_DIR}/FileRaw")

option(ae2f_TEST "" ON)

# Set this source directory as the tree root for project.
set(ae2f_ProjRoot ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Tree root for project")
set(ae2f_submod  .submod CACHE STRING "Relative path to submodule (subdirectory)")
set(ae2f_CoreScript_Wh ${ae2f_ProjRoot}/${ae2f_submod}/ae2f/CoreScript)

# Execute git to fetch CoreScript when repository does not exist
if(NOT EXISTS ${ae2f_CoreScript_Wh})
	execute_process(
		COMMAND git clone https://codeberg.org/ae2f/CoreScript
		${ae2f_CoreScript_Wh}
		)
endif()

# add subdirectory
if(NOT TARGET ae2f::CoreScript)
	add_subdirectory(${ae2f_CoreScript_Wh} ${ae2f_submod}/ae2f/CoreScript)
endif()

include(cmake/Macro.cmake)
include(cmake/Inc.cmake)
include(cmake/FileRaw.cmake)


if(NOT TARGET ae2f::Preproc)
	configure_file(${PROJECT_SOURCE_DIR}/Macro.h.in ${PROJECT_SOURCE_DIR}/inc/ae2f/Macro.h)
	ae2f_CoreLibTentConfigCustom(
		Preproc INTERFACE inc ae2f config/Macro.cmake.in
		"inc/ae2f/Macro.h" 
	)
endif()

if(${ae2f_TEST})
	ae2f_Macro_init(1 100 100)
	ae2f_Inc_init(50 50 50 ON ON ON)
	ae2f_FileRaw_init(ON 30)
	ae2f_FileRaw_init(OFF 30)
endif()
