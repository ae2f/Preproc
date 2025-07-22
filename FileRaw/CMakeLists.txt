cmake_minimum_required(VERSION 3.30)
project(FileRaw_BIN)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/pdb)

set(STRGEN ON)
set(BINGEN_BUFFSZ 50)

add_executable(FileRaw main.c)
file(
	WRITE ${PROJECT_SOURCE_DIR}/main.auto.h
	"#define STRGEN ${STRGEN}\n"
	"#define BINGEN_BUFFSZ ${BINGEN_BUFFSZ}\n"
	"#define OFF	0\n"
	"#define ON	1"
)
