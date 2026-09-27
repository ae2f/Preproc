cmake_minimum_required(VERSION 3.10)

project(Preproc VERSION 1.1.4 LANGUAGES C)

option(ae2f_TEST "" ON)

set(ae2f_CoreScript_Wh ${ae2f_ProjRoot}/${ae2f_submod}/ae2f/CoreScript)
set(ae3f_easyredir_Wh ${ae2f_ProjRoot}/${ae2f_submod}/ae3f/easyredir)

set(ae2f_ProjRoot ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Tree root for project")
set(ae2f_BinRoot ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "Tree root for project")
set(ae2f_submod  .submod CACHE STRING "Relative path to submodule (subdirectory)")
set(ae2f_host "http://talkischeap.work.gd/" CACHE STRING "Default git host for fetching.")

set(ae2f_PreProc_CMAKE_C_COMPILER "${CMAKE_C_COMPILER}" CACHE STRING "Sub-agent C compiler")
set(ae2f_PreProc_CMAKE_CXX_COMPILER "${CMAKE_CXX_COMPILER}" CACHE STRING "Sub-agent C++ compiler")
set(ae2f_PreProc_CMAKE_C_STANDARD "${CMAKE_C_STANDARD}" CACHE STRING "Sub-agent C version")
set(ae2f_PreProc_CMAKE_CXX_STANDARD "${CMAKE_CXX_STANDARD}" CACHE STRING "Sub-agent C++ version")
set(ae2f_PreProc_CMAKE_GENERATOR "${CMAKE_GENERATOR}" CACHE STRING "Sub-agent Generator")
set(ae2f_PreProc_ProjRoot "${ae2f_ProjRoot}" CACHE STRING "unique project root")
set(ae2f_PreProc_BinRoot "${ae2f_BinRoot}" CACHE STRING "unique binary root")
set(ae2f_PreProc_submod  "${ae2f_submod}" CACHE STRING "Relative path to submodule (subdirectory)")
set(ae2f_PreProc_host "${ae2f_host}" CACHE STRING "Default git host for fetching.")

message(STATUS "\n"
	"[ae2f::Preproc] \n"
	"ae2f_PreProc_CMAKE_C_COMPILER: ${ae2f_PreProc_CMAKE_C_COMPILER}\n"
	"ae2f_PreProc_CMAKE_CXX_COMPILER: ${ae2f_PreProc_CMAKE_CXX_COMPILER}\n"
	"ae2f_PreProc_CMAKE_C_STANDARD: ${ae2f_PreProc_CMAKE_C_STANDARD}\n"
	"ae2f_PreProc_CMAKE_GENERATOR: ${ae2f_PreProc_CMAKE_GENERATOR}\n"
	"ae2f_PreProc_ProjRoot: ${ae2f_PreProc_ProjRoot}\n"
	"ae2f_PreProc_BinRoot: ${ae2f_PreProc_BinRoot}\n"
	"ae2f_PreProc_submod: ${ae2f_PreProc_submod}\n"
	"ae2f_PreProc_host: ${ae2f_PreProc_host}\n"
	)

include(cmake/preproctent.cmake)

set(ae2f_Macro_ROOT "${PROJECT_SOURCE_DIR}/Macro" CACHE STRING "${PROJECT_SOURCE_DIR}/Macro")
set(ae2f_Inc_ROOT "${PROJECT_SOURCE_DIR}/Inc" CACHE STRING "${PROJECT_SOURCE_DIR}/Inc")
set(ae2f_FileRaw_ROOT "${PROJECT_SOURCE_DIR}/FileRaw" CACHE STRING "${PROJECT_SOURCE_DIR}/FileRaw")
set(ae2f_Preproc_ROOT ${PROJECT_SOURCE_DIR}/ CACHE STRING "")
set(ae2f_Preproc_BINROO ${PROJECT_BINARY_DIR}/ CACHE STRING "")

# Execute git to fetch CoreScript when repository does not exist
if(NOT EXISTS ${ae2f_CoreScript_Wh})
	execute_process(
		COMMAND git clone ${ae2f_host}/ae2f/CoreScript
		${ae2f_CoreScript_Wh}
		)
endif()

# Execute git to fetch CoreScript when repository does not exist
if(NOT EXISTS ${ae3f_easyredir_Wh})
	execute_process(
		COMMAND git clone ${ae2f_host}/ae3f/easyredir
		${ae3f_easyredir_Wh}
		)
endif()

add_subdirectory(${ae2f_CoreScript_Wh} ${ae2f_ProjRoot}/${ae2f_submod}/ae2f/CoreScript)
add_subdirectory(${ae3f_easyredir_Wh} ${ae2f_ProjRoot}/${ae2f_submod}/ae3f/easyredir)

message(STATUS "sub-configuring ${ae3f_easyredir_Wh}")
ae2fpreproctent_rel(
	easyredir
	${ae3f_easyredir_Wh}
	ae3f/easyredir
	)

set(ae3f_easyredir_exe ${easyredir_exe} CACHE STRING "executable of easyredir")
message(STATUS "ae2f::preproc::cfg easyredir exe ready: ${ae3f_easyredir_exe}")

if(NOT TARGET ae2f::Preproc)
	ae2f_CoreLibTentConfigCustom(
		Preproc INTERFACE inc ae2f config/Macro.cmake.in
		)
endif()

include(cmake/Macro.cmake)
include(cmake/Inc.cmake)
include(cmake/FileRaw.cmake)

if(${ae2f_TEST})
	ae2f_Macro_init(1 100 100)
	ae2f_Inc_init(50 50 50 ON ON ON)
	ae2f_FileRaw_init(ON 30)
	ae2f_FileRaw_init(OFF 30)
endif()
