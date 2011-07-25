###############################################################
# Install documentation
# Build API docs, if doxygen is available
###############################################################

# Set docs target directory
if (WIN32 OR APPLE)
  set(OgreProcedural_DOCS_PATH "Docs")
elseif (UNIX)
  set(OgreProcedural_DOCS_PATH "share/OgreProcedural/docs")
endif ()
  
if (OgreProcedural_INSTALL_DOCS)
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${OgreProcedural_DOCS_PATH}
    PATTERN "src" EXCLUDE
  )
endif ()

# Build and install API documentation if doxygen is available
if(DOXYGEN_FOUND)
	configure_file(${OgreProcedural_TEMPLATES_DIR}/html.cfg.in ${OgreProcedural_BINARY_DIR}/html.cfg)
	add_custom_target(doc COMMAND 
    ${DOXYGEN_EXECUTABLE} ${OgreProcedural_BINARY_DIR}/html.cfg 
    WORKING_DIRECTORY ${OgreProcedural_SOURCE_DIR}/Docs/src/ 
    COMMENT "Building documentation" VERBATIM
  )
	file(WRITE ${OgreProcedural_BINARY_DIR}/api/temp.txt "This file should be deleted before installation. It is only here to force the creation of the api folder")
	file(REMOVE ${OgreProcedural_BINARY_DIR}/api/temp.txt)
	if (OgreProcedural_INSTALL_DOCS)
		install(DIRECTORY ${OgreProcedural_BINARY_DIR}/api DESTINATION ${OgreProcedural_DOCS_PATH})
	endif()
endif()
