set(ae2f_MAC_KEYWORD "ae2f_MAC")
option(ae2f_MAC_BUILD "Convention variable. If on, macro function will be built." OFF)

macro(ae2f_Macro_init2 prm_CMT_REQUIRED prm_SZPARAM prm_SZTPARAM prm_NO_ROOT_BRACKET)
	message(STATUS "ae2f::Macro::init")

	set(__name a-${ae2f_MAC_KEYWORD}-${prm_SZPARAM}-${prm_SZTPARAM}-${prm_CMT_REQUIRED}-${prm_NO_ROOT_BRACKET})


	if(NOT DEFINED ae2f_Macro${__name}SZPRM)
		set(ae2f_Macro${__name}SZPRM 0)
	endif()

	if(NOT DEFINED ae2f_Macro${__name}SZTPRM)
		set(ae2f_Macro${__name}SZTPRM 0)
	endif()

	file(REMOVE_RECURSE ${ae2f_Preproc_BINROOT}/util/Macro/${__name})
	file(MAKE_DIRECTORY ${ae2f_Preproc_BINROOT}/util/Macro/${__name})
	execute_process(
		WORKING_DIRECTORY ${ae2f_Macro_ROOT}
		COMMAND ${CMAKE_COMMAND}
		"-S" "." "-B" "${ae2f_Preproc_BINROOT}/util/Macro/${__name}"
		-Dae2f_Macro_CMT_REQUIRED=${prm_CMT_REQUIRED}
		-Dae2f_Macro_SZPARAM=${prm_SZPARAM}
		-Dae2f_Macro_SZTPARAM=${prm_SZTPARAM}
		-Dae2f_MAC_KEYWORD=${ae2f_MAC_KEYWORD}
		-DNO_ROOT_BRACKET=${prm_NO_ROOT_BRACKET}
		${ae2f_PreProc_CSTD_PRM}
		${ae2f_PreProc_C_PRM}
		"-DCMAKE_BUILD_TYPE=Release"
		${gen}
		${ARGN}
		-Dae2f_submod=${ae2f_submod}
		-Dae2f_ProjRoot=${ae2f_ProjRoot}
		RESULT_VARIABLE ConfOut
		)

	if(NOT ConfOut EQUAL 0)
		message(FATAL_ERROR "[ae2f::Macro::init] Configuration failed. ${ConfOut}")
	endif()

	execute_process(
		WORKING_DIRECTORY ${ae2f_Preproc_BINROOT}/util/Macro/${__name}
		COMMAND ${CMAKE_COMMAND} "--build" "."
		RESULT_VARIABLE BuildOut
		)

	if(NOT BuildOut EQUAL 0)
		message(FATAL_ERROR "[ae2f::Macro::init] Build failed. ${BuildOut}")
	endif()

	unset(ae2f_macro_last_exe)
	file(GLOB_RECURSE ae2f_macro_last_exe ${ae2f_Preproc_BINROOT}/util/Macro/${__name}/bin/**)

	message(STATUS "[ae2f::Macro::init] configuration succeed ${ae2f_macro_last_exe}")

	if(${prm_SZPARAM} GREATER ${ae2f_Macro${__name}SZPRM})
		set(ae2f_Macro${__name}SZPRM ${prm_SZPARAM})
	endif()
	if(${prm_SZTPARAM} GREATER ${ae2f_Macro${__name}SZTPRM})
		set(ae2f_Macro${__name}SZTPRM ${prm_SZTPARAM})
	endif()
	message(STATUS "[ae2f_Macro_init] Succeed. ${ae2f_macro_last_exe}")
endmacro()

macro(ae2f_Macro_init prm_CMT_REQUIRED prm_SZPARAM prm_SZTPARAM)
	ae2f_Macro_init2(${prm_CMT_REQUIRED} ${prm_SZPARAM} ${prm_SZTPARAM} 0)
endmacro()

macro(ae2f_Macro_one prm_in_path_cmake prm_out_path_cmake)
	file(TO_NATIVE_PATH	${prm_in_path_cmake} prm_in)
	file(TO_NATIVE_PATH	${prm_out_path_cmake} prm_out)

	message(STATUS "[ae2f::Macro::one] ${prm_in} ${prm_out}")

	get_filename_component(out_dir "${prm_out}" DIRECTORY)
	file(MAKE_DIRECTORY ${out_dir})

	add_custom_command(
		OUTPUT		${prm_out}
		DEPENDS		${prm_in} ${easyredir_exe} easyredir_proj

		COMMAND 
		"${ae3f_easyredir_exe}"
		"${prm_in}" 
		"${prm_out}" 
		"${prm_out}.err" 0
		"${ae2f_macro_last_exe}"
		)

	add_custom_command(
		OUTPUT		${prm_out}.err
		DEPENDS		${prm_out} easyredir_proj ${easyredir_exe}
		COMMAND		"${ae3f_easyredir_exe}"
		"${prm_in}" "${prm_out}.err" "${prm_out}.err" 0
		"${CMAKE_COMMAND}" --version
		)
endmacro()


macro(ae2f_Macro_cvrt prm_in prm_dir prm_ext)
	get_filename_component(path_no_ext "${prm_in}" NAME_WE)
	ae2f_Macro_one(${prm_in} ${prm_dir}/${path_no_ext}${prm_ext})
endmacro()

macro(ae2f_Macro_cvrtdir prm_in_dir prm_in_glob prm_out_dir prm_out_ext)
	file(GLOB_RECURSE files "${prm_in_dir}/${prm_in_glob}")
	foreach(file ${files})
		ae2f_Macro_cvrt(${file} ${prm_out_dir} ${prm_out_ext})
	endforeach()
endmacro()

macro(ae2f_Macro_Lib_V prm_namespace prm_name prm_prefix
		prm_src_dir	prm_src_glob 
		prm_inc_dir	prm_inc_glob
		prm_inc_out_dir	prm_inc_out_ext

		prm_config_file	prm_include_dir
		)
	file(GLOB_RECURSE files-inc "${prm_inc_dir}/${prm_inc_glob}")
	file(GLOB_RECURSE files-src "${prm_src_dir}/${prm_src_glob}")
	file(GLOB_RECURSE orig-inc  "${prm_include_dir}/*")
	set(ofiles-inc "")

	foreach(file ${files-inc})
		get_filename_component(path_dir "${file}" DIRECTORY)
		file(RELATIVE_PATH path_dir_rel ${prm_inc_dir} ${path_dir})
		get_filename_component(path_no_ext "${file}" NAME_WE)
		ae2f_Macro_one(${file} ${prm_inc_out_dir}/${path_dir_rel}/${path_no_ext}${prm_inc_out_ext})
		set(ofiles-inc ${ofiles-inc} ${prm_inc_out_dir}/${path_dir_rel}/${path_no_ext}${prm_inc_out_ext})
	endforeach()

	ae2f_CoreLibTentConfigCustom(
		${prm_name} ${prm_prefix} ${prm_include_dir} 
		${prm_namespace} ${prm_config_file}
		${ARGN} ${files-src} ${ofiles-inc} ${orig-inc}
		)
endmacro()

macro(ae2f_Macro_Lib_V2_REL
		prm_namespace prm_name prm_prefix 
		prm_src_dir	prm_src_glob
		prm_inc_dir	prm_inc_glob
		prm_inc_out_dir_rel	prm_inc_out_ext

		prm_config_file	prm_include_dir

		prm_guard)

	set(OUTDIRROOT_ "util/MacroREL/${prm_namespace}/${prm_name}")
	set(OUTDIRROOT "${ae2f_Preproc_BINROOT}/${OUTDIRROOT_}")

	ae2f_Macro_Lib_V(
		${prm_namespace}	${prm_name} ${prm_prefix} 
		${prm_src_dir}		${prm_src_glob}
		${prm_inc_dir}		${prm_inc_glob}
		${OUTDIRROOT}/${prm_inc_out_dir_rel}		
		${prm_inc_out_ext}

		${prm_config_file}	${prm_include_dir}
		${ARGN}
		)


	if(NOT "${prm_prefix}" STREQUAL "INTERFACE")
		target_include_directories(
			${prm_namespace}-${prm_name} PUBLIC
			$<BUILD_INTERFACE:${OUTDIRROOT}/${prm_include_dir}/>  
			$<INSTALL_INTERFACE:${OUTDIRROOT_}/${prm_include_dir}/>
			)
	endif()

	target_include_directories(
		${prm_namespace}-${prm_name} INTERFACE
		$<BUILD_INTERFACE:${OUTDIRROOT}/${prm_include_dir}/>  
		$<INSTALL_INTERFACE:${OUTDIRROOT_}/${prm_include_dir}/>
		)

	if(NOT ${prm_guard})
		file(GLOB_RECURSE files-inc "${prm_inc_dir}/${prm_inc_glob}")

		add_executable(
			dvl-${prm_namespace}-${prm_name}
			${ae2f_Preproc_ROOT}/cmake/dummymain.c
			${files-inc}
			)

		target_link_libraries(
			dvl-${prm_namespace}-${prm_name}
			PUBLIC ${prm_namespace}::${prm_name}
			)
	endif()
endmacro()

macro(ae2f_Macro_Lib_V2 
		prm_namespace prm_name prm_prefix 
		prm_src_dir	prm_src_glob
		prm_inc_dir	prm_inc_glob
		prm_inc_out_dir	prm_inc_out_ext

		prm_config_file	prm_include_dir

		prm_guard)

	file(MAKE_DIRECTORY ${prm_inc_out_dir})

	ae2f_Macro_Lib_V(
		${prm_namespace}	${prm_name} ${prm_prefix} 
		${prm_src_dir}		${prm_src_glob}
		${prm_inc_dir}		${prm_inc_glob}
		${prm_inc_out_dir}	${prm_inc_out_ext}
		${prm_config_file}	${prm_include_dir}
		${ARGN}
		)

	if(NOT ${prm_guard})
		file(GLOB_RECURSE files-inc "${prm_inc_dir}/${prm_inc_glob}")
		# message(FATAL_ERROR ${files-inc})

		add_executable(
			dvl-${prm_namespace}-${prm_name}
			${ae2f_Preproc_ROOT}/cmake/dummymain.c
			${files-inc}
			)

		target_link_libraries(
			dvl-${prm_namespace}-${prm_name}
			PUBLIC ${prm_namespace}::${prm_name}
			)
	endif()
endmacro()

function(ae2f_Macro_Lib prm_namespace prm_name prm_prefix 
		prm_in_dir prm_in_glob 
		prm_out_dir prm_out_ext 
		prm_config_file prm_include_dir
		)
	ae2f_Macro_Lib_V(
		${prm_namespace} ${prm_name} ${prm_prefix} 
		${prm_in_dir} ${prm_in_glob} 
		${prm_in_dir} ${prm_in_glob} ${prm_out_dir} ${prm_out_ext}
		${prm_config_file} ${prm_include_dir}
		)
endfunction()

function(ae2f_Macro_autoname prm_in)
	get_filename_component(path_no_ext "${prm_in}" NAME_WE)
	get_filename_component(ext "${prm_in}" LAST_EXT)
	get_filename_component(dir "${prm_in}" DIRECTORY)

	ae2f_Macro_one(${prm_in}  "${dir}/${path_no_ext}.auto${ext}")
endfunction()
