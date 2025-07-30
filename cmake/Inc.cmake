function(ae2f_Inc_init prm_DIRLEN prm_PATHLEN prm_STACKLEN prm_INC_IGNORE_STACKSMASH prm_INC_IGNORE_NFOUND prm_INC_REPT_CHECK)
	file(REMOVE_RECURSE ${ae2f_Inc_ROOT}/build)
	message("[ae2f_Inc_init]  ${CMAKE_GENERATOR}")

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
		WORKING_DIRECTORY ${ae2f_Inc_ROOT}
		COMMAND ${CMAKE_COMMAND} 
		"-S" "." "-B" "./build"
		${gen} ${cstd} ${cc}
		-DDIRLEN=${prm_DIRLEN}
		-DPATHLEN=${prm_PATHLEN}
		-DSTACKLEN=${prm_STACKLEN}
		-DINC_IGNORE_SMASH=${prm_INC_IGNORE_STACKSMASH}
		-DINC_IGNORE_NFOUND=${prm_INC_IGNORE_NFOUND}
		-DINC_REPT_CHECK=${prm_INC_REPT_CHECK}
		${ARGN}
  		RESULT_VARIABLE ConfOut
	)

 	if(NOT ConfOut EQUAL 0)
		message(FATAL_ERROR "[ae2f_Inc_init] Configuration failed. ${ConfOut}")
	endif()

	execute_process(
		WORKING_DIRECTORY ${ae2f_Inc_ROOT}
		COMMAND ${CMAKE_COMMAND} "--build" "build"
  		RESULT_VARIABLE BuildOut
		)
  
 	if(NOT BuildOut EQUAL 0)
		message(FATAL_ERROR "[ae2f_Inc_init] Build failed. ${BuildOut}")
	endif()

	message("[ae2f_Inc_init] Succeed.")
endfunction()

function(ae2f_Inc_Run_One inp_file_absolute out_file_absolute)
	file(GLOB_RECURSE cmd ${ae2f_Macro_ROOT}/build/bin/**)
	get_filename_component(dir "${inp_file_absolute}" DIRECTORY)

	execute_process(
		COMMAND		${cmd} ${dir} ${ARGN}
		INPUT_FILE	${inp_file_absolute}
		OUTPUT_FILE	${out_file_absolute}
	)
endfunction()
