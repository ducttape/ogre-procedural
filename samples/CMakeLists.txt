find_package(OIS REQUIRED)

if(NOT OIS_FOUND)
	message(SEND_ERROR "Failed to find OIS.")
endif()


# if win32, copy the Ogre DLLs and media over
if(WIN32)
	copy_release(OgreMain.dll)
	copy_debug(OgreMain_d.dll)

	copy_release(RenderSystem_Direct3D9.dll)
	copy_debug(RenderSystem_Direct3D9_d.dll)

	copy_release(RenderSystem_GL.dll)
	copy_debug(RenderSystem_GL_d.dll)

	copy_release(OIS.dll)
	copy_debug(OIS_d.dll)

	set(OgreProcedural_PLUGIN_DIR_REL ".")
	set(OgreProcedural_PLUGIN_DIR_DBG ".")

	set(OGRE_PLUGIN_DIR_REL ".")
	set(OGRE_PLUGIN_DIR_DBG ".")

endif(WIN32)

# if Linux, copy the Ogre configuration files over
if(UNIX AND NOT APPLE)

	set(OgreProcedural_PLUGIN_DIR_REL ".")
	set(OgreProcedural_PLUGIN_DIR_DBG ".")
	set(OgreProcedural_COMMENT_RENDERSYSTEM_D3D9 "#")

	#comes from findOgre.cmake (don't know why it doesn't work there...)
	get_filename_component(OGRE_PLUGIN_DIR_REL ${OGRE_RenderSystem_GL_LIBRARY_REL} PATH)
	get_filename_component(OGRE_PLUGIN_DIR_DBG ${OGRE_RenderSystem_GL_LIBRARY_DBG} PATH)

endif(UNIX AND NOT APPLE)

copy_release_debug(SdkTrays.zip ${OgreProcedural_SOURCE_DIR}/samples/media)
copy_release_debug(old_fence.jpg ${OgreProcedural_SOURCE_DIR}/samples/media)
copy_release_debug(red_brick.jpg ${OgreProcedural_SOURCE_DIR}/samples/media)
copy_release_debug(Examples.material ${OgreProcedural_SOURCE_DIR}/samples/media)

configure_release_dir(resources.cfg ${OgreProcedural_TEMPLATES_DIR})
configure_debug_dir(resources_d.cfg ${OgreProcedural_TEMPLATES_DIR})

configure_release_dir(plugins.cfg ${OgreProcedural_TEMPLATES_DIR})
configure_debug_dir(plugins_d.cfg ${OgreProcedural_TEMPLATES_DIR})

add_subdirectory(primitives)

add_subdirectory(extrusion)

# Install sample sources
if (OgreProcedural_INSTALL_SAMPLES_SOURCE)
  if (WIN32 OR APPLE)
    set(OgreProcedural_SAMPLES_DIR Samples)
  elseif (UNIX)
    set(OgreProcedural_SAMPLES_DIR share/OgreProcedural/Samples)
  endif ()
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${OgreProcedural_SAMPLES_DIR}
    REGEX "^CMakeLists.txt$" EXCLUDE
    PATTERN "Makefile.am" EXCLUDE
    PATTERN "Media" EXCLUDE
    PATTERN "bin" EXCLUDE
    PATTERN "setup" EXCLUDE
	PATTERN "obj" EXCLUDE
	PATTERN "scripts" EXCLUDE
  )
  # install a new CMakeLists.txt file to allow building of samples
  configure_file(${OgreProcedural_TEMPLATES_DIR}/SDK_CMakeLists.txt.in ${CMAKE_CURRENT_BINARY_DIR}/../CMakeLists.txt @ONLY)
  configure_file(${OgreProcedural_TEMPLATES_DIR}/SDK_Samples_CMakeLists.txt.in ${CMAKE_CURRENT_BINARY_DIR}/CMakeLists.txt @ONLY)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/../CMakeLists.txt DESTINATION ${OgreProcedural_SAMPLES_DIR}/../)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CMakeLists.txt DESTINATION ${OgreProcedural_SAMPLES_DIR})
endif ()
