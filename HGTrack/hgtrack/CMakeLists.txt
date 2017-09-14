# This is hgtrack/CMakeLists.txt

cmake_minimum_required (VERSION 2.8)

project (HGTrack)

SET (CMAKE_MODULE_PATH ${HGTrack_SOURCE_DIR})

SET (LIBRARY_OUTPUT_PATH ${HGTrack_BINARY_DIR}/bin)
SET (EXECUTABLE_OUTPUT_PATH ${HGTrack_BINARY_DIR}/bin)

# MCC: 
# Set to static link to C RunTime Lib (CRT) so no VC redist or libgcc.a will be needed.
# See http://stackoverflow.com/questions/14172856/cmake-compile-with-mt-instead-of-md
# This essentially change all "Multi-thread DLL (/MD)" to "Multi-thread (/MT)"
# in Properties -> C/C++ -> Code Generation -> Runtime Library
# See http://stackoverflow.com/questions/29462439/crt-library-type
if (MSVC)
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
  set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /MT")
  set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /MT")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")
  set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")

  # MCC: borrowed from SDL
  option(FORCE_STATIC_VCRT "Force /MT for static VC runtimes" ON)
  if(FORCE_STATIC_VCRT)
    foreach(flag_var
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
      if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
      endif()
    endforeach()
  endif()

  # Make sure /RTC1 is disabled, otherwise it will use functions from the CRT
  foreach(flag_var
      CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    string(REGEX REPLACE "/RTC(su|[1su])" "" ${flag_var} "${${flag_var}}")
  endforeach(flag_var)
endif()

# MCC:
MESSAGE (STATUS "BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}")   
IF (WIN32) 
  IF (NOT ${BUILD_SHARED_LIBS})
    # If building statically, add /FORCE:MULTIPLE linker flag
    # Add Linker flag /FORCE:MULTIPLE
    MESSAGE (STATUS "add linker flags /FORCE:MULTIPLE")
    SET (EXTRA_CMAKE_EXE_LINKER_FLAGS "${EXTRA_CMAKE_EXE_LINKER_FLAGS} /FORCE:MULTIPLE")
    SET (EXTRA_CMAKE_MODULE_LINKER_FLAGS "${EXTRA_CMAKE_MODULE_LINKER_FLAGS} /FORCE:MULTIPLE")
    SET (EXTRA_CMAKE_SHARED_LINKER_FLAGS "${EXTRA_CMAKE_SHARED_LINKER_FLAGS} /FORCE:MULTIPLE")
  ENDIF ()
ENDIF (WIN32)

# Options to add extra compiler and linker flags
#
# These options allow you to specify additional flags without
# affecting the default flags for a particular platform or build type.
# This is especially useful for adding extra warning flags.
set( EXTRA_CMAKE_C_FLAGS             CACHE STRING "Extra flags appended to CMAKE_C_FLAGS" )
set( EXTRA_CMAKE_CXX_FLAGS           CACHE STRING "Extra flags appended to CMAKE_CXX_FLAGS" )
set( EXTRA_CMAKE_EXE_LINKER_FLAGS    CACHE STRING "Extra flags appended to CMAKE_EXE_LINKER_FLAGS" )
set( EXTRA_CMAKE_MODULE_LINKER_FLAGS CACHE STRING "Extra flags appended to CMAKE_MODULE_LINKER_FLAGS" )
set( EXTRA_CMAKE_SHARED_LINKER_FLAGS CACHE STRING "Extra flags appended to CMAKE_SHARED_LINKER_FLAGS" )
set( CMAKE_C_FLAGS             "${CMAKE_C_FLAGS} ${EXTRA_CMAKE_C_FLAGS}" )
set( CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} ${EXTRA_CMAKE_CXX_FLAGS}" )
set( CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} ${EXTRA_CMAKE_EXE_LINKER_FLAGS}" )
set( CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${EXTRA_CMAKE_MODULE_LINKER_FLAGS}" )
set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${EXTRA_CMAKE_SHARED_LINKER_FLAGS}" )

    
MESSAGE (STATUS "BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}")   
IF (WIN32)  
  IF (NOT BUILD_SHARED_LIBS)
    # If building statically, add /FORCE:MULTIPLE linker flag
    # Add Linker flag /FORCE:MULTIPLE
    MESSAGE (STATUS "add linker flags /FORCE:MULTIPLE")
    SET (EXTRA_CMAKE_EXE_LINKER_FLAGS "${EXTRA_CMAKE_EXE_LINKER_FLAGS} /FORCE:MULTIPLE")
    SET (EXTRA_CMAKE_MODULE_LINKER_FLAGS "${EXTRA_CMAKE_MODULE_LINKER_FLAGS} /FORCE:MULTIPLE")
    SET (EXTRA_CMAKE_SHARED_LINKER_FLAGS "${EXTRA_CMAKE_SHARED_LINKER_FLAGS} /FORCE:MULTIPLE")
  ENDIF ()
ENDIF (WIN32)

# ---------------------------------------------------------------------------
# Options to add extra compiler and linker flags.
#
# These options allow you to specify additional flags without
# affecting the default flags for a particular platform or build type.
# This is especially useful for adding extra warning flags.

#SET( EXTRA_CMAKE_C_FLAGS             CACHE STRING "Extra flags appended to CMAKE_C_FLAGS" )
#SET( EXTRA_CMAKE_CXX_FLAGS           CACHE STRING "Extra flags appended to CMAKE_CXX_FLAGS" )
#SET( EXTRA_CMAKE_EXE_LINKER_FLAGS    CACHE STRING "Extra flags appended to CMAKE_EXE_LINKER_FLAGS" )
#SET( EXTRA_CMAKE_MODULE_LINKER_FLAGS CACHE STRING "Extra flags appended to CMAKE_MODULE_LINKER_FLAGS" )
#SET( EXTRA_CMAKE_SHARED_LINKER_FLAGS CACHE STRING "Extra flags appended to CMAKE_SHARED_LINKER_FLAGS" )

SET( CMAKE_C_FLAGS             "${CMAKE_C_FLAGS} ${EXTRA_CMAKE_C_FLAGS}" )
SET( CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} ${EXTRA_CMAKE_CXX_FLAGS}" )
SET( CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} ${EXTRA_CMAKE_EXE_LINKER_FLAGS}" )
SET( CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${EXTRA_CMAKE_MODULE_LINKER_FLAGS}" )
SET( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${EXTRA_CMAKE_SHARED_LINKER_FLAGS}" )

MESSAGE (STATUS "CMAKE_EXE_LINKER_FLAGS = ${CMAKE_EXE_LINKER_FLAGS}")
MESSAGE (STATUS "CMAKE_MODULE_LINKER_FLAGS = ${CMAKE_MODULE_LINKER_FLAGS}")
MESSAGE (STATUS "CMAKE_SHARED_LINKER_FLAGS = ${CMAKE_SHARED_LINKER_FLAGS}")

# ===== Find and configure OpenCV =====    
find_package (OpenCV2 REQUIRED)
# See http://stackoverflow.com/questions/24262081/cmake-relwithdebinfo-links-to-debug-libs
# This solve the RelWithDebInfo run-time error for OpenCV 2.4.13
set_target_properties(${OpenCV_LIBS} PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)


MESSAGE (STATUS "OPENCV2_FOUND = ${OPENCV2_FOUND}")
IF (OPENCV2_FOUND)
  MESSAGE (STATUS "OPENCV2_DIR = ${OPENCV2_DIR}")
  MESSAGE (STATUS "add OPENCV2_INCLUDE_DIRS = ${OPENCV2_INCLUDE_DIRS}")  
  INCLUDE_DIRECTORIES(${OPENCV2_INCLUDE_DIRS})
  MESSAGE (STATUS "add OPENCV2_LIB_DIR = ${OPENCV2_LIB_DIR}") 
  LINK_DIRECTORIES( ${OPENCV2_LIB_DIR} )
  MESSAGE (STATUS "  OpenCV_LIBS = ${OpenCV_LIBS}")  
  
  
  IF( OPENCV2_CUDA_FOUND )
    MESSAGE( STATUS "OpenCV2 CUDA Found" )
    # Workaround issue with OpenCV2 on Ubuntu which requires several
    # CUDA libs (e.g., cudart, cufft, nppc, nppi, npps), but does not
    # export absolute paths to them in the OpenCVConfig.cmake it
    # provides.
    IF( ${CUDA_CUDART_LIBRARY} )
      GET_FILENAME_COMPONENT( CUDA_LIB_DIR ${CUDA_CUDART_LIBRARY} DIRECTORY )
      MESSAGE( STATUS "Adding CUDA library dir: ${CUDA_LIB_DIR}" )
      LINK_DIRECTORIES( ${CUDA_LIB_DIR} )
    ENDIF()
  ELSE()
    MESSAGE( STATUS "OpenCV2 CUDA Not Found" )
  ENDIF()
ELSE()
  MESSAGE( STATUS "OpenCV2 Not Found" )
ENDIF()

INCLUDE_DIRECTORIES ( ${HGTrack_SOURCE_DIR} )
#LINK_DIRECTORIES( ${LIBRARY_OUTPUT_PATH} ) 
# ===== Graph Decompose =====
SUBDIRS (graph_decomp)

# ===== MultiTarget Tracker =====
SUBDIRS (util)
SUBDIRS (config)
SUBDIRS (clique)
SUBDIRS (opt)
SUBDIRS (math)
SUBDIRS (export)
SUBDIRS (feat)
SUBDIRS (spline_fit)
SUBDIRS (detector)
SUBDIRS (hgtracker)
SUBDIRS (mt_tracker)


