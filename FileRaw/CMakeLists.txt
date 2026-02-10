cmake_minimum_required(VERSION 3.10)
project(FileRaw_BIN LANGUAGES C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/pdb)

option(STRGEN "" OFF)
set(BINGEN_BUFFSZ 50)

add_executable(FileRaw main.c)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/main.h.in
	${CMAKE_CURRENT_SOURCE_DIR}/.main.h
	)
