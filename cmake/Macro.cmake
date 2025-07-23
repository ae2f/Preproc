set(ae2f_MAC_KEYWORD "ae2f_MAC")
option(ae2f_MAC_BUILD "Convention variable. If on, macro function will be built." OFF)

include(${PROJECT_SOURCE_DIR}/cmake/Core.cmake)

function(ae2f_Macro_init prm_CMT_REQUIRED prm_SZPARAM prm_SZTPARAM)
	file(REMOVE_RECURSE ${ae2f_Macro_ROOT}/build)
  	message("[ae2f_Macro_init]  ${CMAKE_GENERATOR}")

	if(DEFINED CMAKE_C_STANDARD)
		set(cstd "-DCMAKE_C_STANDARD=${CMAKE_C_STANDARD}")
	else()
		set(cstd "")
	endif()

	if(DEFINED CMAKE_C_COMPILER)
		set(cc "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
	else()
		set(cc "")
	endif()

	if(DEFINED CMAKE_GENERATOR)
		set(gen "-G${CMAKE_GENERATOR}")
	else()
		set(gen "")
	endif()

	execute_process(
		WORKING_DIRECTORY ${ae2f_Macro_ROOT}
		COMMAND ${CMAKE_COMMAND} 
		"-S" "." "-B" "./build"
		-Dae2f_Macro_CMT_REQUIRED=${prm_CMT_REQUIRED}
		-Dae2f_Macro_SZPARAM=${prm_SZPARAM}
		-Dae2f_Macro_SZTPARAM=${prm_SZTPARAM}
        	-Dae2f_MAC_KEYWORD=${ae2f_MAC_KEYWORD}
		${gen} ${cstd} ${cc}
		${ARGN}
  		RESULT_VARIABLE ConfOut
	)

 	if(NOT ConfOut EQUAL 0)
		message(FATAL_ERROR "[ae2f_Macro_init] Configuration failed. ${ConfOut}")
	endif()

	execute_process(
		WORKING_DIRECTORY ${ae2f_Macro_ROOT}
		COMMAND ${CMAKE_COMMAND} "--build" "build"
  		RESULT_VARIABLE BuildOut
		)
  
 	if(NOT BuildOut EQUAL 0)
		message(FATAL_ERROR "[ae2f_Macro_init] Build failed. ${BuildOut}")
	endif()

 	message("[ae2f_Macro_init] Succeed.")
endfunction()

function(ae2f_Macro_one prm_in prm_out)
	message("[ae2f_Macro_one] ${prm_in} ${prm_out}")
	message("[ae2f_Macro_one] ROOT ${ae2f_Macro_ROOT}")	

	file(GLOB_RECURSE macrocmd ${ae2f_Macro_ROOT}/build/bin/**)

	execute_process(
		COMMAND ${macrocmd}
		INPUT_FILE ${prm_in}
		OUTPUT_FILE ${prm_out}
		RESULT_VARIABLE MacroOut
	)

	if(MacroOut EQUAL 0)
		message("[ae2f_Macro] succeed.")
	endif()

	if(MacroOut EQUAL 1)
		message(FATAL_ERROR "[ae2f_Macro] Output operation has failed.")
	endif()

	if(MacroOut EQUAL 2)
		message(FATAL_ERROR "[ae2f_Macro] Unexpected token.")
	endif()

	if(MacroOut EQUAL 3)
		message(FATAL_ERROR "[ae2f_Macro] Buffer overrun has occurred.")
	endif()
endfunction()


function(ae2f_Macro_cvrt prm_in prm_dir prm_ext)
	get_filename_component(path_no_ext "${prm_in}" NAME_WE)
	ae2f_Macro_one(${prm_in} ${prm_dir}/${path_no_ext}${prm_ext})
endfunction()

function(ae2f_Macro_cvrtdir prm_in_dir prm_in_glob prm_out_dir prm_out_ext)
	file(GLOB_RECURSE files "${prm_in_dir}/${prm_in_glob}")
	foreach(file ${files})
		ae2f_Macro_cvrt(${file} ${prm_out_dir} ${prm_out_ext})
	endforeach()
endfunction()

function(ae2f_Macro_Lib_V prm_namespace prm_name prm_prefix
		prm_src_dir	prm_src_glob 
		prm_inc_dir	prm_inc_glob
		prm_inc_out_dir	prm_inc_out_ext

		prm_config_file	prm_include_dir
)
	file(GLOB_RECURSE files-inc "${prm_inc_dir}/${prm_inc_glob}")
	file(GLOB_RECURSE files-src "${prm_src_dir}/${prm_src_glob}")

	foreach(file ${files-inc})
		ae2f_Macro_cvrt(${file} ${prm_inc_out_dir} ${prm_inc_out_ext})
	endforeach()

	file(GLOB_RECURSE ofiles-inc "${prm_inc_out_dir}/*${prm_inc_out_ext}")

	if(ae2f_MAC_BUILD)
		ae2f_CoreLibTentConfigCustom(
			${prm_name} ${prm_prefix} ${prm_include_dir} 
			${prm_namespace} ${prm_config_file}
			${ARGN} ${files-src} ${ofiles-inc}
		)
	else()
		ae2f_CoreLibTentConfigCustom(
			${prm_name} INTERFACE ${prm_include_dir} 
			${prm_namespace} ${prm_config_file} 
			${ofiles-inc}
			${ARGN}
		)
	endif()
endfunction()

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
