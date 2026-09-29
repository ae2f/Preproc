
# configure a cmake binary project from ${srcdir} to ${tardir}
# ${prefix}_exe
# ${prefix}_proj_cfg
# ${prefix}_proj
# ${prefix}_dep
function(ae2fpreproctent2 prefix name srcdir tardir)

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

		set(gen "-G${CMAKE_GENERATOR}")
	else()
		set(gen "")
	endif()

	set(exe "${tardir}/bin/Release/${name}${CMAKE_EXECUTABLE_SUFFIX}")
	set(${prefix}_exe ${exe} PARENT_SCOPE)
	set(${prefix}_dep ${prefix}_proj ${exe} PARENT_SCOPE)
	message(STATUS "preproctent: ${exe} ${${prefix}_exe}")

	if(TARGET ${prefix}_proj)
		return()
	endif()

	add_custom_command(
		DEPENDS		${srcdir}/CMakeLists.txt
		OUTPUT		${tardir}/CMakeCache.txt
		WORKING_DIRECTORY ${srcdir}
		COMMAND
		${CMAKE_COMMAND}
		"-S" "."
		"-B" "${tardir}"
		"-DCMAKE_BUILD_TYPE=Release"
		"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${tardir}/bin"
		"-Dae2f_TEST=OFF"
		"-Dae2f_CXX=OFF"
		"${prm_c_compiler}"
		"${prm_cc_compiler}"
		"${prm_c_std}"
		"${prm_cc_std}"
		-Dae2f_ProjRoot=${ae2f_PreProc_ProjRoot}
		-Dae2f_submod=${ae2f_PreProc_submod}
		-Dae2f_host=${ae2f_PreProc_host}
		"-DCMAKE_TOOLCHAIN_FILE=${ae2f_Preproc_ROOT}/cmake/unify.cmake"
		${ARGN}
		)

	add_custom_target(${prefix}_proj_cfg DEPENDS ${tardir}/CMakeCache.txt)

	add_custom_command(
		OUTPUT			${exe}
		WORKING_DIRECTORY	${tardir}
		COMMAND			${CMAKE_COMMAND} --build . --config Release
		DEPENDS			${tardir}/CMakeCache.txt ${prefix}_proj_cfg
		)
	add_custom_target(${prefix}_proj DEPENDS ${exe} ${prefix}_proj_cfg)
endfunction()

macro(ae2fpreproctent name srcdir tardir)
	ae2fpreproctent2(${name} ${name} ${srcdir} ${tardir} ${ARGN})
endmacro()

macro(ae2fpreproctent_rel name srcdir tardir_rel)
	ae2fpreproctent(${name} "${srcdir}" "${ae2f_Preproc_BINROOT}/util/${tardir_rel}" ${ARGN})
endmacro()

macro(ae2fpreproctent_rel2 prefix name srcdir tardir_rel)
	ae2fpreproctent2(${prefix} ${name} "${srcdir}" "${ae2f_Preproc_BINROOT}/util/${tardir_rel}" ${ARGN})
endmacro()
