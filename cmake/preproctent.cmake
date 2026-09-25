
# configure a cmake binary project from ${srcdir} to ${tardir}
# ${outprefix}_exe
function(ae2fpreproctent outprefix srcdir tardir name)
	if(NOT ae2f_PreProc_CMAKE_C_CC_COMPILER STREQUAL "")
		set(prm_cc_compiler "-DCMAKE_CXX_COMPILER=${ae2f_PreProc_CMAKE_CXX_COMPILER}")
	else()
		set(prm_cc_compiler "")
	endif()

	if(NOT ae2f_PreProc_CMAKE_C_C_COMPILER STREQUAL "")
		set(prm_c_compiler "-DCMAKE_C_COMPILER=${ae2f_PreProc_CMAKE_C_COMPILER}")
	else()
		set(prm_c_compiler ae2f_PreProc_C_C_PRM "")
	endif()

	if(NOT ae2f_PreProc_CMAKE_C_STANDARD STREQUAL "")
		set(prm_c_std "-DCMAKE_C_STANDARD=${ae2f_PreProc_CMAKE_C_STANDARD}")
	else()
		set(prm_c_std "")
	endif()

	if(NOT ae2f_PreProc_CMAKE_CXX_STANDARD STREQUAL "")
		set(prm_cc_std "-DCMAKE_CXX_STANDARD=${ae2f_PreProc_CMAKE_CXX_STANDARD}")
	else()
		set(prm_cc_std "")
	endif()

	if(NOT ae2f_PreProc_CMAKE_GENERATOR STREQUAL "")
		set(gen "-G${ae2f_PreProc_CMAKE_GENERATOR}")
	elseif(DEFINED CMAKE_GENERATOR)
		set(gen "-G${CMAKE_GENERATOR}")
	else()
		set(gen "")
	endif()


	set(exe ${tardir}/bin/${name}${CMAKE_EXECUTABLE_SUFFIX})
	set(${outprefix}_exe ${exe} PARENT_SCOPE)
	message(STATUS "preproctent: ${exe} ${${outprefix}_exe}")

	file(REMOVE_RECURSE ${tardir})
	file(MAKE_DIRECTORY ${tardir})

	execute_process(
		WORKING_DIRECTORY ${srcdir}
		COMMAND
		${CMAKE_COMMAND}
		"-S" "."
		"-B" "${tardir}"
		"-DCMAKE_BUILD_TYPE=Release"
		"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=bin"
		"-DCMAKE_PDB_OUTPUT_DIRECTORY=pdb"
		"-Dae2f_TEST=OFF"
		"-Dae2f_CXX=OFF"
		"${prm_c_compiler}"
		"${prm_cc_compiler}"
		"${prm_c_std}"
		"${prm_cc_std}"
		-Dae2f_ProjRoot=${ae2f_PreProc_ProjRoot}
		-Dae2f_submod=${ae2f_PreProc_submod}
		-Dae2f_host=${ae2f_PreProc_host}
		RESULT_VARIABLE code
		)

	if(NOT code EQUAL 0)
		mssage(FATAL_ERROR "[X] configure failed. code: ${code}")
		return()
	endif()

	if(NOT EXISTS ${tardir}/CMakeCache.txt)
		message(FATAL_ERROR "[X] no ${tardir}/CMakeCache.txt")
	endif()

	execute_process(
		WORKING_DIRECTORY ${tardir}
		COMMAND		${CMAKE_COMMAND} --build .
		)
endfunction()

macro(ae2fpreproctent_rel outprefix srcdir tardir_rel name)
	ae2fpreproctent(${outprefix} ${srcdir} "${ae2f_Preproc_BINROOT}/util/${tardir_rel}" ${name})
endmacro()
