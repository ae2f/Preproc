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
