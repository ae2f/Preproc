cmake_minimum_required(VERSION 3.10)
project(Inc_BIN)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/pdb)

add_executable(Inc main.c)

set(DIRLEN	150)
set(PATHLEN	150)
set(STACKLEN	50)

option(INC_IGNORE_SMASH		"" ON)
option(INC_IGNORE_NFOUND	"" ON)
option(INC_REPT_CHECK		"" ON)
option(INC_LEAVE_NFOUND		"" OFF)
option(NDEBUG			"" ON)

configure_file(
	${PROJECT_SOURCE_DIR}/main.h.in
	${PROJECT_SOURCE_DIR}/.main.h
	)
