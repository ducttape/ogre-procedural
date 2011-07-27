cmake_minimum_required(VERSION 2.6)

include(PrecompiledHeader)
include(ProceduralConfigTargets)

set( HDRS
	include/OgreProceduralPlatform.h
	include/OgreProcedural.h
	include/OgreProceduralMeshGenerator.h
	include/OgreProceduralBoxGenerator.h
	include/OgreProceduralCapsuleGenerator.h
	include/OgreProceduralConeGenerator.h
	include/OgreProceduralCylinderGenerator.h
	include/OgreProceduralIcoSphereGenerator.h
	include/OgreProceduralPlaneGenerator.h
	include/OgreProceduralSphereGenerator.h
	include/OgreProceduralRoot.h
	include/OgreProceduralRoundedBoxGenerator.h
	include/OgreProceduralTorusGenerator.h
	include/OgreProceduralTorusKnotGenerator.h
	include/OgreProceduralTubeGenerator.h
	include/OgreProceduralUtils.h
	include/OgreProceduralExtruder.h
	include/OgreProceduralLathe.h
	include/OgreProceduralShape.h
	include/OgreProceduralShapeGenerators.h
	include/OgreProceduralPath.h
	include/OgreProceduralPathGenerators.h
	include/OgreProceduralTrack.h
	include/OgreProceduralTriangulator.h
	include/OgreProceduralTriangleBuffer.h
	include/OgreProceduralStableHeaders.h
	include/OgreProceduralMultiShape.h
	include/OgreProceduralGeometryHelpers.h
	)

set( SRCS
		src/OgreProceduralBoxGenerator.cpp
		src/OgreProceduralCapsuleGenerator.cpp
		src/OgreProceduralConeGenerator.cpp
		src/OgreProceduralCylinderGenerator.cpp
		src/OgreProceduralIcoSphereGenerator.cpp
		src/OgreProceduralPlaneGenerator.cpp
		src/OgreProceduralRoot.cpp
		src/OgreProceduralRoundedBoxGenerator.cpp
		src/OgreProceduralSphereGenerator.cpp
		src/OgreProceduralTorusGenerator.cpp
		src/OgreProceduralTorusKnotGenerator.cpp
		src/OgreProceduralTubeGenerator.cpp
		src/OgreProceduralUtils.cpp
		src/OgreProceduralShape.cpp
		src/OgreProceduralShapeGenerators.cpp
		src/OgreProceduralPath.cpp
		src/OgreProceduralPathGenerators.cpp
		src/OgreProceduralTrack.cpp
		src/OgreProceduralExtruder.cpp
		src/OgreProceduralLathe.cpp
		src/OgreProceduralTriangulator.cpp
		src/OgreProceduralPrecompiledHeaders.cpp
		src/OgreProceduralMultiShape.cpp
		src/OgreProceduralGeometryHelpers.cpp
	)

include_directories( ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
)
add_definitions(-DOgreProcedural_NONCLIENT_BUILD -Wall -Werror -Wno-unused-parameter -Wno-unused-but-set-parameter -pedantic -std=c++0x)

add_library(OgreProcedural STATIC ${HDRS} ${SRCS})

set_target_properties(OgreProcedural PROPERTIES DEBUG_POSTFIX _d)

if (UNIX)
    set_target_properties(OgreProcedural PROPERTIES PREFIX "lib")
else (UNIX)
    set_target_properties(OgreProcedural PROPERTIES PREFIX "")
endif (UNIX)

target_link_libraries(OgreProcedural ${OGRE_LIBRARIES} ${OIS_LIBRARIES})

#install(TARGETS OgreProcedural
#		BUNDLE DESTINATION "bin"
#		RUNTIME DESTINATION "bin"
#		LIBRARY DESTINATION "lib"
#		ARCHIVE DESTINATION "lib"
#		FRAMEWORK DESTINATION "lib")

procedural_install_target(OgreProcedural)

install(FILES ${HDRS} DESTINATION include/OgreProcedural)

# specify a precompiled header to use
use_precompiled_header(OgreProcedural
  "${CMAKE_CURRENT_SOURCE_DIR}/include/ProceduralStableHeaders.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/ProceduralPrecompiledHeaders.cpp"
)

#set_target_properties(OgreProcedural PROPERTIES
# 	ARCHIVE_OUTPUT_DIRECTORY ${PROCEDURAL_BINARY_DIR}/lib
#    LIBRARY_OUTPUT_DIRECTORY ${PROCEDURAL_BINARY_DIR}/lib
#    RUNTIME_OUTPUT_DIRECTORY ${PROCEDURAL_BINARY_DIR}/bin
#  )
#install(TARGETS OgreProcedural
	#BUNDLE DESTINATION ${PROCEDURAL_BINARY_DIR}/bin CONFIGURATIONS All
	#RUNTIME DESTINATION ${PROCEDURAL_BINARY_DIR}/bin CONFIGURATIONS All
	#LIBRARY DESTINATION ${PROCEDURAL_BINARY_DIR}/bin CONFIGURATIONS All
	#ARCHIVE DESTINATION ${PROCEDURAL_BINARY_DIR}/bin CONFIGURATIONS All
	#FRAMEWORK DESTINATION ${PROCEDURAL_BINARY_DIR}/bin CONFIGURATIONS All
	#)
