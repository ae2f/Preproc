cmake_minimum_required(VERSION 3.30)
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

file(
	WRITE ${PROJECT_SOURCE_DIR}/main.auto.h
	"#define DIRLEN			${DIRLEN} \n"
	"#define PATHLEN		${PATHLEN} \n"
	"#define STACKLEN		${STACKLEN} \n"
	"#define INC_IGNORE_NFOUND	${INC_IGNORE_NFOUND} \n"
	"#define INC_IGNORE_SMASH	${INC_IGNORE_SMASH} \n"
	"#define INC_REPT_CHECK		${INC_REPT_CHECK}"
)
