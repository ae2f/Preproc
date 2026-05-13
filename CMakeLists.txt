cmake_minimum_required(VERSION 3.10)

project(Preproc VERSION 1.1.4 LANGUAGES C)

set(ae2f_PreProc_CMAKE_C_COMPILER "${CMAKE_C_COMPILER}" CACHE STRING "Sub-agent C compiler")
set(ae2f_PreProc_CMAKE_CXX_COMPILER "${CMAKE_CXX_COMPILER}" CACHE STRING "Sub-agent C++ compiler")

set(ae2f_PreProc_CMAKE_C_STANDARD "${CMAKE_C_STANDARD}" CACHE STRING "Sub-agent C version")
set(ae2f_PreProc_CMAKE_CXX_STANDARD "${CMAKE_CXX_STANDARD}" CACHE STRING "Sub-agent C++ version")

set(ae2f_PreProc_CMAKE_GENERATOR "${CMAKE_GENERATOR}" CACHE STRING "Sub-agent Generator")

if(NOT ae2f_PreProc_CMAKE_C_CC_COMPILER STREQUAL "")
	set(ae2f_PreProc_C_CC_PRM "-DCMAKE_CXX_COMPILER=${ae2f_PreProc_CMAKE_CXX_COMPILER}")
else()
	set(ae2f_PreProc_C_CC_PRM "")
endif()

if(NOT ae2f_PreProc_CMAKE_C_C_COMPILER STREQUAL "")
	set(ae2f_PreProc_C_C_PRM "-DCMAKE_C_COMPILER=${ae2f_PreProc_CMAKE_C_COMPILER}")
else()
	set(ae2f_PreProc_C_C_PRM "")
endif()

set(ae2f_PreProc_C_PRM ${ae2f_PreProc_C_CC_PRM} ${ae2f_PreProc_C_C_PRM} "-Dae2f_TEST=OFF" "-Dae2f_CXX=OFF" CACHE STRING "Sub-agent parameter")

message(STATUS "\n"
	"Cache Memory Used: \n"
	"ae2f_PreProc_CMAKE_C_COMPILER: ${ae2f_PreProc_CMAKE_C_COMPILER}\n"
	"ae2f_PreProc_CMAKE_CXX_COMPILER: ${ae2f_PreProc_CMAKE_CXX_COMPILER}\n"
	"ae2f_PreProc_CMAKE_C_STANDARD: ${ae2f_PreProc_CMAKE_C_STANDARD}\n"
	"ae2f_PreProc_CMAKE_GENERATOR: ${ae2f_PreProc_CMAKE_GENERATOR}\n"
	)

if(NOT ae2f_PreProc_CMAKE_C_STANDARD STREQUAL "")
	set(ae2f_PreProc_CSTD_PRM "-DCMAKE_C_STANDARD=${ae2f_PreProc_CMAKE_C_STANDARD}")
else()
	set(ae2f_PreProc_CSTD_PRM "")
endif()

if(NOT ae2f_PreProc_CMAKE_GENERATOR STREQUAL "")
	set(gen "-G${ae2f_PreProc_CMAKE_GENERATOR}")
elseif(DEFINED CMAKE_GENERATOR)
	set(gen "-G${CMAKE_GENERATOR}")
else()
	set(gen "")
endif()


message(STATUS "\n"
	"Generated: \n"
	"ae2f_PreProc_C_PRM: ${ae2f_PreProc_C_PRM}	\n"
	"ae2f_PreProc_CSTD_PRM: ${ae2f_PreProc_CSTD_PRM}	\n"
	)

set(ae2f_Macro_ROOT "${PROJECT_SOURCE_DIR}/Macro/" CACHE STRING "${PROJECT_SOURCE_DIR}/Macro")
set(ae2f_Inc_ROOT "${PROJECT_SOURCE_DIR}/Inc/" CACHE STRING "${PROJECT_SOURCE_DIR}/Inc")
set(ae2f_FileRaw_ROOT "${PROJECT_SOURCE_DIR}/FileRaw/" CACHE STRING "${PROJECT_SOURCE_DIR}/FileRaw")
set(ae2f_Preproc_ROOT ${PROJECT_SOURCE_DIR}/ CACHE STRING "")
set(ae2f_Preproc_BINROOT ${PROJECT_BINARY_DIR}/ CACHE STRING "")

option(ae2f_TEST "" ON)

# Set this source directory as the tree root for project.
set(ae2f_ProjRoot ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Tree root for project")
set(ae2f_submod  .submod CACHE STRING "Relative path to submodule (subdirectory)")
set(ae2f_CoreScript_Wh ${ae2f_ProjRoot}/${ae2f_submod}/ae2f/CoreScript/)
set(ae3f_easyredir_Wh ${ae2f_ProjRoot}/${ae2f_submod}/ae3f/easyredir/)

# Execute git to fetch CoreScript when repository does not exist
if(NOT EXISTS ${ae2f_CoreScript_Wh})
	execute_process(
		COMMAND git clone https://talkischeap.work.gd/ae2f/CoreScript
		${ae2f_CoreScript_Wh}
		)
endif()

# Execute git to fetch CoreScript when repository does not exist
if(NOT EXISTS ${ae3f_easyredir_Wh})
	execute_process(
		COMMAND git clone https://talkischeap.work.gd/ae3f/easyredir
		${ae3f_easyredir_Wh}
		)
endif()

message(STATUS "sub-configuring ${ae3f_easyredir_Wh}")
execute_process(
	COMMAND ${CMAKE_COMMAND}
	"-S" "${ae3f_easyredir_Wh}"
	"-B" "${PROJECT_BINARY_DIR}/util/easyredir"
	"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=bin"
	"-DCMAKE_PDB_OUTPUT_DIRECTORY=pdb"
	"-Dae2f_ProjRoot=${ae2f_ProjRoot}"
	"-Dae2f_submod=${ae2f_submod}"
	${ae2f_PreProc_CSTD_PRM}
	${ae2f_PreProc_C_PRM}
	"-DCMAKE_BUILD_TYPE=Release"
	${gen}
	)

execute_process(
	COMMAND 
	${CMAKE_COMMAND} "--build" "${PROJECT_BINARY_DIR}/util/easyredir"
	)

file(GLOB_RECURSE _ae3f_easyredir_exe ${PROJECT_BINARY_DIR}/util/easyredir/bin/**)
set(ae3f_easyredir_exe ${_ae3f_easyredir_exe} CACHE STRING "executable of easyredir")
message(STATUS "ae2f::preproc::cfg easyredir exe ready: ${ae3f_easyredir_exe}")

# add subdirectory
add_subdirectory(${ae2f_CoreScript_Wh} ${ae2f_submod}/ae2f/CoreScript)

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
