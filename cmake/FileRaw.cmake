macro(ae2f_FileRaw_init prm_STRGEN prm_BINGEN_BUFFSZ)
	message(STATUS "ae2f::FileRaw::init")

	if(DEFINED ae2f_PreProc_CMAKE_C_STANDARD)
		set(cstd "-DCMAKE_C_STANDARD=${ae2f_preProc_CMAKE_C_STANDARD}")
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

	execute_process(
		WORKING_DIRECTORY ${ae2f_FileRaw_ROOT}
		COMMAND 
		${CMAKE_COMMAND} "-S" "." "-B" ${ae2f_Preproc_BINROOT}/util/FileRaw/A${prm_STRGEN}
		-DSTRGEN=${prm_STRGEN}
		-DBINGEN_BUFFSZ=${prm_BINGEN_BUFFSZ}
		${gen} ${cstd} ${cc}
		${ae2f_PreProc_C_PRM}
		${ARGN}
		RESULT_VARIABLE ConfOut
		)

	if(NOT ConfOut EQUAL 0)
		message(FATAL_ERROR "[ae2f::FileRaw::init] Configuration failed. ${ConfOut}")
	endif()

	execute_process(
		WORKING_DIRECTORY ${ae2f_FileRaw_ROOT}
		COMMAND ${CMAKE_COMMAND} "--build" 
		${ae2f_Preproc_BINROOT}/util/FileRaw/A${prm_STRGEN}
		RESULT_VARIABLE BuildOut
		)

	if(NOT BuildOut EQUAL 0)
		message(FATAL_ERROR "[ae2f::FileRaw::init] Build failed. ${BuildOut}")
	endif()

	file(GLOB_RECURSE ae2f_fileraw_last_exe ${ae2f_Preproc_BINROOT}/util/FileRaw/A${prm_STRGEN}/bin/**)
	message(STATUS "[ae2f::FileRaw::init] Succeed.")
endmacro()


function(ae2f_FileRaw_Run_One inp_file_absolute out_file_absolute)
	message(STATUS "cfg::ae2f::FileRaw ${inp_file_absolute} ${out_file_absolute}")
	add_custom_command(
		OUTPUT		${out_file_absolute}
		MAIN_DEPENDENCY	${inp_file_absolute}

		COMMAND 
		${ae3f_easyredir_exe}
		${inp_file_absolute} ${out_file_absolute} ${out_file_absolute}.err 0
		${ae2f_fileraw_last_exe}
		COMMENT "ae2f::FileRaw ${inp_file_absolute} ${out_file_absolute}"
		VERBATIM
		)

	add_custom_command(
		OUTPUT		${out_file_absolute}.err
		MAIN_DEPENDENCY	${inp_file_absolute}
		COMMAND	${CMAKE_COMMAND} --version
		VERBATIM
		)
endfunction()

function(ae2f_FileRaw_Run_One2 inp_file_absolute out_file_absolute keygen)
	message(STATUS "cfg::ae2f::FileRaw2 ${inp_file_absolute} ${out_file_absolute}")
	add_custom_command(
		OUTPUT		${out_file_absolute}
		MAIN_DEPENDENCY	${inp_file_absolute}

		COMMAND 
		${ae3f_easyredir_exe}
		${inp_file_absolute} ${out_file_absolute} "\"\"" 0
		${ae2f_fileraw_last_exe}
		${keygen}

		COMMENT "ae2f::FileRaw2 ${inp_file_absolute} ${out_file_absolute}"
		VERBATIM
		)
endfunction()
