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
