include(FindPkgMacros)
include(PreprocessorUtils)

findpkg_begin(PROCEDURAL)

getenv_path(PROCEDURAL_HOME)
getenv_path(PROGRAMFILES)
getenv_path(PROCEDURAL_SOURCE)

# construct search paths from environmental hints and
# OS specific guesses
if (WIN32)
  set(PROCEDURAL_PREFIX_GUESSES
    ${ENV_PROGRAMFILES}/PROCEDURAL
    C:/ProceduralSDK
  )
elseif (UNIX)
  set(PROCEDURAL_PREFIX_GUESSES
    /opt/procedural
    /opt/PROCEDURAL
    /usr/lib${LIB_SUFFIX}/procedural
    /usr/lib${LIB_SUFFIX}/PROCEDURAL
    /usr/local/lib${LIB_SUFFIX}/procedural
    /usr/local/lib${LIB_SUFFIX}/PROCEDURAL
    $ENV{HOME}/procedural
    $ENV{HOME}/PROCEDURAL
  )
endif ()
set(PROCEDURAL_PREFIX_PATH
  ${PROCEDURAL_HOME} ${PROCEDURAL_SDK} ${ENV_PROCEDURAL_HOME} ${ENV_PROCEDURAL_SDK}
  ${PROCEDURAL_PREFIX_GUESSES}
)
create_search_paths(PROCEDURAL)

# If both PROCEDURAL_BUILD and PROCEDURAL_SOURCE are set, prepare to find Ogre in a build dir
set(PROCEDURAL_PREFIX_SOURCE ${PROCEDURAL_SOURCE} ${ENV_PROCEDURAL_SOURCE})
set(PROCEDURAL_PREFIX_BUILD ${PROCEDURAL_BUILD} ${ENV_PROCEDURAL_BUILD})
set(PROCEDURAL_PREFIX_DEPENDENCIES_DIR ${PROCEDURAL_DEPENDENCIES_DIR} ${ENV_PROCEDURAL_DEPENDENCIES_DIR})
if (PROCEDURAL_PREFIX_SOURCE AND PROCEDURAL_PREFIX_BUILD)
  foreach(dir ${PROCEDURAL_PREFIX_SOURCE})
    set(PROCEDURAL_INC_SEARCH_PATH ${dir}/library/include ${PROCEDURAL_INC_SEARCH_PATH})
    set(PROCEDURAL_LIB_SEARCH_PATH ${dir}/lib ${PROCEDURAL_LIB_SEARCH_PATH})
  endforeach(dir)
  foreach(dir ${PROCEDURAL_PREFIX_BUILD})
    set(PROCEDURAL_INC_SEARCH_PATH ${dir}/include ${PROCEDURAL_INC_SEARCH_PATH})
    set(PROCEDURAL_LIB_SEARCH_PATH ${dir}/lib ${PROCEDURAL_LIB_SEARCH_PATH})
    set(PROCEDURAL_BIN_SEARCH_PATH ${dir}/bin ${PROCEDURAL_BIN_SEARCH_PATH})
  endforeach(dir)
else()
  set(PROCEDURAL_PREFIX_SOURCE "NOTFOUND")
  set(PROCEDURAL_PREFIX_BUILD "NOTFOUND")
endif ()

set(PROCEDURAL_PREFIX_SOURCE ${PROCEDURAL_SOURCE} ${ENV_PROCEDURAL_SOURCE})
set(PROCEDURAL_PREFIX_BUILD ${PROCEDURAL_BUILD} ${ENV_PROCEDURAL_BUILD})


set(PROCEDURAL_LIBRARY_NAMES "OgreProcedural")
get_debug_names(PROCEDURAL_LIBRARY_NAMES)

find_path(PROCEDURAL_INCLUDE_DIR NAMES Procedural.h HINTS ${PROCEDURAL_CONFIG_INCLUDE_DIR} ${PROCEDURAL_INC_SEARCH_PATH} ${PROCEDURAL_FRAMEWORK_INCLUDES} ${PROCEDURAL_PKGC_INCLUDE_DIRS} PATH_SUFFIXES "PROCEDURAL")

find_library(PROCEDURAL_LIBRARY_REL NAMES ${PROCEDURAL_LIBRARY_NAMES} HINTS ${PROCEDURAL_INC_SEARCH_PATH} ${PROCEDURAL_FRAMEWORK_INCLUDES} ${PROCEDURAL_PKGC_INCLUDE_DIRS} PATH_SUFFIXES "" "release" "relwithdebinfo" "minsizerel")
find_library(PROCEDURAL_LIBRARY_DBG NAMES ${PROCEDURAL_LIBRARY_NAMES_DBG} HINTS ${PROCEDURAL_CONFIG_INCLUDE_DIR} ${PROCEDURAL_INC_SEARCH_PATH} ${PROCEDURAL_FRAMEWORK_INCLUDES} ${PROCEDURAL_PKGC_INCLUDE_DIRS} PATH_SUFFIXES "" "debug")
make_library_set(PROCEDURAL_LIBRARY)

findpkg_finish(PROCEDURAL)