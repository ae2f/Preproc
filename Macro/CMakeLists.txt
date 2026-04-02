cmake_minimum_required(VERSION 3.10)
project(Macro_BIN LANGUAGES C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/pdb)

add_executable(Macro main.c)

set(ae2f_Macro_CMT_REQUIRED 1)
set(ae2f_Macro_SZPARAM      50)
set(ae2f_Macro_SZTPARAM     300)
set(ae2f_MAC_KEYWORD        ae2f_MAC)

configure_file(
	${PROJECT_SOURCE_DIR}/main.h.in
	${PROJECT_SOURCE_DIR}/.main.h
	)


# Set this source directory as the tree root for project.
set(ae2f_ProjRoot ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Tree root for project")
set(ae2f_BinRoot ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "Tree root for project")
set(ae2f_submod  .submod CACHE STRING "Relative path to submodule (subdirectory)")
set(ae2f_CoreScript_Wh ${ae2f_ProjRoot}/${ae2f_submod}/ae2f/CoreScript)

# Execute git to fetch CoreScript when repository does not exist
if(NOT EXISTS ${ae2f_CoreScript_Wh})
	execute_process(
		COMMAND git clone https://talkischeap.work.gd/ae2f/CoreScript
		${ae2f_CoreScript_Wh}
		)
endif()

# add subdirectory
if(NOT TARGET ae2f::CoreScript)
	add_subdirectory(${ae2f_CoreScript_Wh} ${ae2f_submod}/ae2f/CoreScript)
endif()

ae2f_UltraErrWERROR(Macro PRIVATE)
