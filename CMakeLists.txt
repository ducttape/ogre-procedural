cmake_minimum_required(VERSION 2.6)

project(OgreProcedural)

set(CMAKE_MODULE_PATH
  "${OgreProcedural_SOURCE_DIR}/CMake"
  "${OgreProcedural_SOURCE_DIR}/CMake/Utils"
  "${OgreProcedural_SOURCE_DIR}/CMake/Packages"
)

set(OgreProcedural_VERSION_DASH_SEPARATED "0.2")

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

set(CMAKE_DEBUG_POSTFIX "_d")

if (WIN32 OR APPLE)
  if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX
	  "${OgreProcedural_BINARY_DIR}/sdk" CACHE PATH "OgreProcedural install prefix" FORCE
    )
  endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
endif(WIN32 OR APPLE)

find_package(OGRE REQUIRED)

find_package(Doxygen)

option(OgreProcedural_BUILD_SAMPLES "Build OgreProcedural samples." TRUE)
option(OgreProcedural_BUILD_TESTS "Build OgreProcedural test cases." FALSE)
option(OgreProcedural_INSTALL_DOCS "Install documentation." FALSE)

# Specify build paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OgreProcedural_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OgreProcedural_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OgreProcedural_BINARY_DIR}/bin")

set(OgreProcedural_TEMPLATES_DIR "${OgreProcedural_SOURCE_DIR}/CMake/Templates")
set(OgreProcedural_WORK_DIR ${OgreProcedural_BINARY_DIR})

# Configure version file for use by scripts
configure_file("${OgreProcedural_TEMPLATES_DIR}/version.txt.in" "${OgreProcedural_BINARY_DIR}/version.txt" @ONLY)

include_directories("${OgreProcedural_SOURCE_DIR}/library/include")

add_subdirectory(library)

# definitions for samples
set(OgreProcedural_LIBRARIES OgreProcedural)

#useful macros for samples and tests
include(ProceduralMacros)

if (OgreProcedural_BUILD_SAMPLES)
	add_subdirectory(samples)
endif()

if (OgreProcedural_BUILD_TESTS)
	add_subdirectory(tests)
endif()

# Install documentation
add_subdirectory(docs)

# Install CMake modules
add_subdirectory(CMake)
