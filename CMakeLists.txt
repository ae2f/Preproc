cmake_minimum_required(VERSION 3.20)

project(Preproc VERSION 1.1.4)

set(ae2f_Macro_ROOT "${PROJECT_SOURCE_DIR}/Macro" CACHE STRING "${PROJECT_SOURCE_DIR}/Macro")
set(ae2f_Inc_ROOT "${PROJECT_SOURCE_DIR}/Inc" CACHE STRING "${PROJECT_SOURCE_DIR}/Inc")
set(ae2f_FileRaw_ROOT "${PROJECT_SOURCE_DIR}/FileRaw" CACHE STRING "${PROJECT_SOURCE_DIR}/FileRaw")

option(ae2f_TEST "" ON)

include(cmake/Core.cmake)
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
