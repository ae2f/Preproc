# -DINC_LEAVE_NFOUND
#	When this flag is set ON, on ignoring nfound of path, it will write the include again.
macro(ae2f_Inc_init prm_DIRLEN prm_PATHLEN prm_STACKLEN prm_INC_IGNORE_STACKSMASH prm_INC_IGNORE_NFOUND prm_INC_REPT_CHECK)
	message(STATUS "[ae2f::Inc::init]")

	if(DEFINED ae2f_PreProc_CMAKE_C_STANDARD)
		set(cstd "-DCMAKE_C_STANDARD=${ae2f_PreProc_CMAKE_C_STANDARD}")
	else()
		set(cstd "")
	endif()

	if(DEFINED ae2f_PreProc_CMAKE_C_COMPILER)
		set(cc "-DCMAKE_C_COMPILER=${ae2f_PreProc_CMAKE_C_COMPILER}")
	else()
		set(cc "")
	endif()

	if(DEFINED ae2f_PreProc_CMAKE_GENERATOR)
		set(gen "-G${ae2f_PreProc_CMAKE_GENERATOR}")
	else()
		set(gen "")
	endif()

	set(__name ${prm_DIRLEN}${prm_PATHLEN}${prm_STACKLEN}${prm_INC_IGNORE_NFOUND}${prm_INC_IGNORE_STACKSMASH}${prm_INC_REPT_CHECK})

	execute_process(
		WORKING_DIRECTORY ${ae2f_Inc_ROOT}
		COMMAND ${CMAKE_COMMAND}
		"-S" "." "-B" ${ae2f_Preproc_BINROOT}/util/Inc/${__name}
		${gen} ${cstd} ${cc}
		-DDIRLEN=${prm_DIRLEN}
		-DPATHLEN=${prm_PATHLEN}
		-DSTACKLEN=${prm_STACKLEN}
		-DINC_IGNORE_SMASH=${prm_INC_IGNORE_STACKSMASH}
		-DINC_IGNORE_NFOUND=${prm_INC_IGNORE_NFOUND}
		-DINC_REPT_CHECK=${prm_INC_REPT_CHECK}
		${ae2f_PreProc_C_PRM}
		${ARGN}
  		RESULT_VARIABLE ConfOut
	)

 	if(NOT ConfOut EQUAL 0)
		message(FATAL_ERROR "[ae2f_Inc_init] Configuration failed. ${ConfOut}")
	endif()

	execute_process(
		WORKING_DIRECTORY ${ae2f_Inc_ROOT}
		COMMAND ${CMAKE_COMMAND} "--build" ${ae2f_Preproc_BINROOT}/util/Inc/${__name}
  		RESULT_VARIABLE BuildOut
		)

	file(GLOB_RECURSE ae2f_inc_last_exe ${ae2f_Preproc_BINROOT}/util/Inc/${__name}/bin/**)
  
 	if(NOT BuildOut EQUAL 0)
		message(FATAL_ERROR "[ae2f_Inc_init] Build failed. ${BuildOut}")
	endif()



	message("[ae2f_Inc_init] Succeed.")
endmacro()

function(ae2f_Inc_Run_One inp_file_absolute out_file_absolute)
	get_filename_component(dir "${inp_file_absolute}" DIRECTORY)

	add_custom_command(
		OUTPUT			${out_file_absolute}
		MAIN_DEPENDENCY		${inp_file_absolute}
		WORKING_DIRECTORY	${dir}

		COMMAND 
		${ae3f_easyredir_exe}
		${inp_file_absolute} ${out_file_absolute} ${out_file_absolute}.err 0
		${ae2f_inc_last_exe} ${dir} ${ARGN}

		COMMENT "ae2f::Inc ${dir} ${ARGN}"
		VERBATIM
		)

	add_custom_command(
		OUTPUT			${out_file_absolute}.err
		MAIN_DEPENDENCY		${inp_file_absolute}

		COMMAND ${ae3f_easyredir_exe} ${inp_file_absolute} ${out_file_absolute}.err
		${out_file_absolute}.err 0
		${CMAKE_COMMAND} --version
		VERBATIM
		)
endfunction()
