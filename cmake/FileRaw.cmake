function(ae2f_FileRaw_init prm_STRGEN prm_BINGEN_BUFFSZ)
	file(REMOVE_RECURSE ${ae2f_FileRaw_ROOT}/build)
	message("[ae2f_FileRaw_init]  ${CMAKE_GENERATOR}")

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
		WORKING_DIRECTORY ${ae2f_FileRaw_ROOT}
		COMMAND ${CMAKE_COMMAND}
		"-S" "." "-B" "./build"
		-DSTRGEN=${prm_STRGEN}
		-DBINGEN_BUFFSZ=${prm_BINGEN_BUFFSZ}
		${gen} ${cstd} ${cc}
		${ARGN}
  		RESULT_VARIABLE ConfOut
	)

 	if(NOT ConfOut EQUAL 0)
		message(FATAL_ERROR "[ae2f_FileRaw_init] Configuration failed. ${ConfOut}")
	endif()

	execute_process(
		WORKING_DIRECTORY ${ae2f_FileRaw_ROOT}
		COMMAND ${CMAKE_COMMAND} "--build" "build"
  		RESULT_VARIABLE BuildOut
		)
  
 	if(NOT BuildOut EQUAL 0)
		message(FATAL_ERROR "[ae2f_FileRaw_init] Build failed. ${BuildOut}")
	endif()

	message("[ae2f_FileRaw_init] Succeed.")
endfunction()


function(ae2f_FileRaw_Run_One inp_file_absolute out_file_absolute)
	file(GLOB_RECURSE cmd ${ae2f_FileRaw_ROOT}/build/bin/**)
	execute_process(
		COMMAND		${cmd}
		INPUT_FILE	${inp_file_absolute}
		OUTPUT_FILE	${out_file_absolute}
	)
endfunction()
