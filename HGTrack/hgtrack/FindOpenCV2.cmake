# Find the OpenCV version 2 library. Directory structures between 
# version 1 and 2 are different
#
# Usage:
#  
#   FIND_PACKAGE( OPENCV_2 REQUIRED )
#   TARGET_LINK_LIBRARIES( TARGET_NAME ${OPENCV_2_LIBS} )
#
# Variables set:
#
#   OPENCV2_FOUND			: If false, don't try to use OpenCV
#   OPENCV2_DIR				: Root directory 
#   OPENCV2_INCLUDE_DIRS	: OpenCV include directories
#   OPENCV2_LIBS			: OpenCV libraries to link against
#   OPENCV2_VERSION			: version string e.g., 2.3.0
#   OPENCV2_VERSION_MAJOR	: if version string is 2.3.0, then major version is 2
#   OPENCV2_VERSION_MINOR	: if version string is 2.3.0, then minor version is 3
#   OPENCV2_VERSION_PATCH	: if version string is 2.3.0, then the patch is 0


# History: 
#   23 Aug 2011. Initial version for OpenCV 2.6, windows x86 -- TP Tian
#

# Note that findpackage opencv2 should be placed at the root folder of cmake
# directory so that the values will be propagated to child scope.

IF (${OPENCV2_FOUND} MATCHES "YES")
  #MESSAGE ( "FindOpenCV2.cmake run twice!")
  RETURN()
  MESSAGE ( "after return --- you should not see this!!")
ENDIF ()

# SET (OPENCV2_FOUND "NO")
# MARK_AS_ADVANCED(OPENCV2_FOUND)
# SET (OPENCV2_CUDA_FOUND "NO")
# MARK_AS_ADVANCED(OPENCV2_CUDA_FOUND)

IF( ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    FIND_PATH (OPENCV2_DIR build/include/opencv2/opencv.hpp)

    # Easy case where we will reuse the cmake's config file in the build directory
    IF (OPENCV2_DIR AND EXISTS ${OPENCV2_DIR}/OpenCVConfig.cmake)
        SET (OPENCV2_FOUND "YES")
        # MESSAGE (STATUS "OPENCV2: Using config file ${OPENCV2_DIR}/OpenCVConfig.cmake")

        ADD_DEFINITIONS(-D HAS_OPENCV2)	  

        INCLUDE(${OPENCV2_DIR}/OpenCVConfig.cmake)

        SET( OPENCV2_LIBS           ${OpenCV_LIBS} )
        SET( OPENCV2_LIB_DIR        ${OpenCV_LIB_DIR} )
        SET( OPENCV2_INCLUDE_DIRS   ${OpenCV_INCLUDE_DIRS} CACHE PATH "OPENCV2_INCLUDE_DIRS")
        SET( OPENCV2_COMPUTE_CAPABILITIES ${OpenCV_COMPUTE_CAPABILITIES} )
        SET( OPENCV2_VERSION        ${OpenCV_VERSION} )
        SET( OPENCV2_VERSION_MAJOR  ${OpenCV_VERSION_MAJOR} )
        SET( OPENCV2_VERSION_MINOR  ${OpenCV_VERSION_MINOR} )
        SET( OPENCV2_VERSION_PATCH  ${OpenCV_VERSION_PATCH} )
        IF (OpenCV_CUDA_VERSION)
            SET( OPENCV2_CUDA_FOUND	  "YES")	
        ENDIF ()

        MARK_AS_ADVANCED(FORCE OPENCV2_INCLUDE_DIRS)
    ENDIF()
ENDIF( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )

IF(  APPLE)
    FIND_PATH (OPENCV2_DIR build/include/opencv2/opencv.hpp)

    # Easy case where we will reuse the cmake's config file in the build directory
    IF (OPENCV2_DIR AND EXISTS ${OPENCV2_DIR}/OpenCVConfig.cmake)
        SET (OPENCV2_FOUND "YES")
        # MESSAGE (STATUS "OPENCV2: Using config file ${OPENCV2_DIR}/OpenCVConfig.cmake")

        ADD_DEFINITIONS(-D HAS_OPENCV2)	  

        INCLUDE(${OPENCV2_DIR}/OpenCVConfig.cmake)

        SET( OPENCV2_LIBS           ${OpenCV_LIBS} )
        SET( OPENCV2_LIB_DIR        ${OpenCV_LIB_DIR} )
        SET( OPENCV2_INCLUDE_DIRS   ${OpenCV_INCLUDE_DIRS} CACHE PATH "OPENCV2_INCLUDE_DIRS")
        SET( OPENCV2_COMPUTE_CAPABILITIES ${OpenCV_COMPUTE_CAPABILITIES} )
        SET( OPENCV2_VERSION        ${OpenCV_VERSION} )
        SET( OPENCV2_VERSION_MAJOR  ${OpenCV_VERSION_MAJOR} )
        SET( OPENCV2_VERSION_MINOR  ${OpenCV_VERSION_MINOR} )
        SET( OPENCV2_VERSION_PATCH  ${OpenCV_VERSION_PATCH} )
        IF (OpenCV_CUDA_VERSION)
            SET( OPENCV2_CUDA_FOUND	  "YES")	
        ENDIF ()

        MARK_AS_ADVANCED(FORCE OPENCV2_INCLUDE_DIRS)
    ENDIF()
ENDIF( APPLE )

IF( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )

    # MESSAGE( STATUS "Please set the OPENCV2_DIR to be the install directory of your own OpenCV build, such as C:/opencv_build/install" )
    # MESSAGE( STATUS "or set the OPENCV2_DIR to be the build directory of OpenCV src zip, such as C:/opencv/build" )

    FIND_PATH (OPENCV2_DIR build/include/opencv2/opencv.hpp)

    # Easy case where we will reuse the cmake's config file in the build directory
    IF (OPENCV2_DIR AND EXISTS ${OPENCV2_DIR}/OpenCVConfig.cmake)
        SET (OPENCV2_FOUND "YES")
        MESSAGE(STATUS "FindOpenCV2: OPENCV2_FOUND = ${OPENCV2_FOUND}" )
        MARK_AS_ADVANCED(OPENCV2_FOUND)

        ADD_DEFINITIONS(-D HAS_OPENCV2)

        MESSAGE (STATUS "Including ${OPENCV2_DIR}/OpenCVConfig.cmake")
        INCLUDE(${OPENCV2_DIR}/OpenCVConfig.cmake)
        
        # Ming: this cause CMake error.
        # OpenCV does not have RelWithDebInfo, map OpenCV's release to RelWithDebInfo
        #SET_TARGET_PROPERTIES(${OpenCV_LIBS} PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)

        MESSAGE (STATUS "FindOpenCV2.cmake: OpenCV_LIBS = ${OpenCV_LIBS}")
        MESSAGE (STATUS "FindOpenCV2.cmake: OpenCV_LIB_DIR = ${OpenCV_LIB_DIR}")

        # Set GEVXL specific vars
        SET( OPENCV2_LIBS           ${OpenCV_LIBS} )
        SET( OPENCV2_LIB_DIR        ${OpenCV_LIB_DIR} )

        SET( OPENCV2_INCLUDE_DIRS   ${OpenCV_INCLUDE_DIRS} CACHE PATH "OPENCV2_INCLUDE_DIRS")
        #SET( OPENCV2_INCLUDE_DIRS   ${OpenCV_INCLUDE_DIRS} )
        SET( OPENCV2_COMPUTE_CAPABILITIES ${OpenCV_COMPUTE_CAPABILITIES} )
        SET( OPENCV2_VERSION        ${OpenCV_VERSION} )
        SET( OPENCV2_VERSION_MAJOR  ${OpenCV_VERSION_MAJOR} )
        SET( OPENCV2_VERSION_MINOR  ${OpenCV_VERSION_MINOR} )
        SET( OPENCV2_VERSION_PATCH  ${OpenCV_VERSION_PATCH} )	 
        IF (OpenCV_CUDA_VERSION)
            SET( OPENCV2_CUDA_FOUND	  "YES")	
        ENDIF ()

        MARK_AS_ADVANCED(FORCE OPENCV2_INCLUDE_DIRS)


    ENDIF()

    # Hard case where we have to traverse the directory to figure out the locations
    IF (OPENCV2_DIR AND NOT EXISTS ${OPENCV2_DIR}/OpenCVConfig.cmake)
        #MESSAGE (STATUS "OPENCV2: can not find the OpenCVConfig.cmake, so perform manual setting of OPENCV2_INCLUDE_DIR")
        #~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        # Include directories
        #~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        FIND_PATH ( OPENCV2_INCLUDE_DIR
            NAMES opencv2/opencv.hpp
            PATHS ${OPENCV2_DIR}/build/include ${OPENCV2_DIR}/include)

        IF (OPENCV2_INCLUDE_DIR)
            MESSAGE(STATUS "OpenCVConfig.cmake not found: OPENCV2_INCLUDE_DIR = ${OPENCV2_INCLUDE_DIR}" )
            INCLUDE_DIRECTORIES(${OPENCV2_INCLUDE_DIR})
        ENDIF ()


        #~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        # Version
        #~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        IF (OPENCV2_INCLUDE_DIR)
            SET (OPENCV2_FOUND "YES")
            MESSAGE(STATUS "OPENCV2_FOUND = ${OPENCV2_FOUND}" )
            MARK_AS_ADVANCED(OPENCV2_FOUND)

            ADD_DEFINITIONS(-D HAS_OPENCV2)

            FILE( STRINGS ${OPENCV2_INCLUDE_DIR}/opencv2/core/version.hpp OPENCV2_VERSION_MAJOR REGEX "#define CV_MAJOR_VERSION.*")
            FILE( STRINGS ${OPENCV2_INCLUDE_DIR}/opencv2/core/version.hpp OPENCV2_VERSION_MINOR REGEX "#define CV_MINOR_VERSION.*")
            FILE( STRINGS ${OPENCV2_INCLUDE_DIR}/opencv2/core/version.hpp OPENCV2_VERSION_PATCH REGEX "#define CV_SUBMINOR_VERSION.*")

            STRING( REGEX REPLACE "#define CV_MAJOR_VERSION[ \t]+|//.*" "" OPENCV2_VERSION_MAJOR ${OPENCV2_VERSION_MAJOR})
            STRING( REGEX REPLACE "#define CV_MINOR_VERSION[ \t]+|//.*" "" OPENCV2_VERSION_MINOR ${OPENCV2_VERSION_MINOR})
            STRING( REGEX REPLACE "#define CV_SUBMINOR_VERSION[ \t]+|//.*" "" OPENCV2_VERSION_PATCH ${OPENCV2_VERSION_PATCH})

            STRING( REGEX MATCH "[0-9]+" OPENCV2_VERSION_MAJOR ${OPENCV2_VERSION_MAJOR} )
            STRING( REGEX MATCH "[0-9]+" OPENCV2_VERSION_MINOR ${OPENCV2_VERSION_MINOR} )
            STRING( REGEX MATCH "[0-9]+" OPENCV2_VERSION_PATCH ${OPENCV2_VERSION_PATCH} )

            # MESSAGE( STATUS "OPENCV2_VERSION_MAJOR ${OPENCV2_VERSION_MAJOR}" )
            # MESSAGE( STATUS "OPENCV2_VERSION_MINOR ${OPENCV2_VERSION_MINOR}" )
            # MESSAGE( STATUS "OPENCV2_VERSION_PATCH ${OPENCV2_VERSION_PATCH}" )

            SET( OPENCV2_VERSION "${OPENCV2_VERSION_MAJOR}.${OPENCV2_VERSION_MINOR}.${OPENCV2_VERSION_PATCH}") 
            SET( OPENCV2_VERSION_WITHOUT_DOT "${OPENCV2_VERSION_MAJOR}${OPENCV2_VERSION_MINOR}${OPENCV2_VERSION_PATCH}") 
            MESSAGE( STATUS "OPENCV2_VERSION ${OPENCV2_VERSION}" )
            MESSAGE( STATUS "OPENCV2_VERSION_WITHOUT_DOT ${OPENCV2_VERSION_WITHOUT_DOT}" )
        ENDIF()


        #~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        # Link Libraries
        #~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        SET (OPENCV2_LIB_DIR_SEARCHPATHS "")
        IF (MSVC90) 
            IF (CMAKE_CL_64)
                SET (OPENCV2_LIB_DIR_SEARCHPATHS  #${OPENCV2_DIR}/x64/vc9/lib ${OPENCV2_DIR}/build/x64/vc9/lib 
                                                  ${OPENCV2_DIR}/x64/vc9/lib ${OPENCV2_DIR}/build/x64/vc9/staticlib
                )
            ELSE ()
                SET (OPENCV2_LIB_DIR_SEARCHPATHS  #${OPENCV2_DIR}/x86/vc9/lib ${OPENCV2_DIR}/build/x86/vc9/lib
                                                  ${OPENCV2_DIR}/x86/vc9/lib ${OPENCV2_DIR}/build/x86/vc9/staticlib
                )
            ENDIF ()    
        ENDIF ()
        IF (MSVC10)
            IF (CMAKE_CL_64)
                SET (OPENCV2_LIB_DIR_SEARCHPATHS ${OPENCV2_DIR}/x64/vc10/lib ${OPENCV2_DIR}/build/x64/vc10/lib )
            ELSE ()
                SET (OPENCV2_LIB_DIR_SEARCHPATHS ${OPENCV2_DIR}/x86/vc10/lib ${OPENCV2_DIR}/build/x86/vc10/lib )
            ENDIF ()    
        ENDIF ()
        FIND_PATH (OPENCV2_LIB_DIR
            NAMES opencv_core${OPENCV2_VERSION_WITHOUT_DOT}.lib
            PATHS ${OPENCV2_LIB_DIR_SEARCHPATHS})  

        MESSAGE( STATUS "--> OpenCVConfig.cmake not found: OPENCV2_LIB_DIR = ${OPENCV2_LIB_DIR}" )

        IF (OPENCV2_LIB_DIR)
            SET(OPENCV_LIB_LIST opencv_core opencv_imgproc opencv_features2d opencv_gpu opencv_calib3d opencv_objdetect opencv_video opencv_highgui opencv_ml opencv_legacy opencv_contrib opencv_flann)

            SET(OPENCV2_LIBS "")
            FOREACH (__LIB ${OPENCV_LIB_LIST})
                IF (CMAKE_MAJOR_VERSION GREATER 2  OR  CMAKE_MINOR_VERSION GREATER 4)
                    # CMake>=2.6 supports "debug XXd optimized XX"
                    SET(OPENCV2_LIBS ${OPENCV2_LIBS} debug ${__LIB}${OPENCV2_VERSION_WITHOUT_DOT}d optimized ${__LIB}${OPENCV2_VERSION_WITHOUT_DOT})
                ELSE ()
                    SET(OPENCV2_LIBS ${OPENCV2_LIBS} ${__LIB}${OPENCV2_VERSION_WITHOUT_DOT})
                ENDIF ()   
            ENDFOREACH ()

            MESSAGE( STATUS "OpenCVConfig.cmake not found: OPENCV2_LIBS = ${OPENCV2_LIBS}" )
            LINK_DIRECTORIES(${OPENCV2_LIB_DIR})    
        ENDIF (OPENCV2_LIB_DIR)

    ENDIF ()

    
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    # Copy the required DLLs to the build folders 
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    SET( OPENCV2_DLL_DIR_DEBUG   "${OPENCV2_DIR}/bin/Debug" ) 
    SET( OPENCV2_DLL_DIR_RELEASE "${OPENCV2_DIR}/bin/Release" ) 

    # grab all the dlls in the bin directory and copy to the build directory
    FILE(GLOB OPENCV2_DLL_FILES_DEBUG ${OPENCV2_DLL_DIR_DEBUG}/*.dll)
    # MESSAGE( STATUS "OPENCV2_DLL_FILES_DEBUG = ${OPENCV2_DLL_FILES_DEBUG}" )
    FILE(GLOB OPENCV2_DLL_FILES_RELEASE ${OPENCV2_DLL_DIR_RELEASE}/*.dll)
    # MESSAGE( STATUS "OPENCV2_DLL_FILES_RELEASE = ${OPENCV2_DLL_FILES_RELEASE}" )

    IF(NOT EXISTS "${EXECUTABLE_OUTPUT_PATH}/Debug")			
	  FILE(MAKE_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}/Debug")			
	ENDIF(NOT EXISTS "${EXECUTABLE_OUTPUT_PATH}/Debug")
	
    IF(NOT EXISTS "${EXECUTABLE_OUTPUT_PATH}/RelWithDebInfo")			
	  FILE(MAKE_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}/RelWithDebInfo")			
	ENDIF(NOT EXISTS "${EXECUTABLE_OUTPUT_PATH}/RelWithDebInfo")
	
    IF(NOT EXISTS "${EXECUTABLE_OUTPUT_PATH}/Release")			
	  FILE(MAKE_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}/Release")			
	ENDIF(NOT EXISTS "${EXECUTABLE_OUTPUT_PATH}/Release")
  
    FOREACH( AN_OPENCV2_DLL_FILE_DEBUG ${OPENCV2_DLL_FILES_DEBUG})
        CONFIGURE_FILE("${AN_OPENCV2_DLL_FILE_DEBUG}" "${EXECUTABLE_OUTPUT_PATH}/Debug" COPYONLY)
    ENDFOREACH() 
    
    FOREACH( AN_OPENCV2_DLL_FILE_RELEASE ${OPENCV2_DLL_FILES_RELEASE})
        CONFIGURE_FILE("${AN_OPENCV2_DLL_FILE_RELEASE}" "${EXECUTABLE_OUTPUT_PATH}/Release" COPYONLY)
        CONFIGURE_FILE("${AN_OPENCV2_DLL_FILE_RELEASE}" "${EXECUTABLE_OUTPUT_PATH}/RelWithDebInfo" COPYONLY)
    ENDFOREACH() 


ENDIF( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )

# Help Message
# MESSAGE(STATUS "OPENCV2_INCLUDE_DIRS=${OPENCV2_INCLUDE_DIRS}")
# MESSAGE(STATUS "OPENCV2_LIBS=${OPENCV2_LIBS}")
# MESSAGE(STATUS "OPENCV2_CUDA_FOUND=${OPENCV2_CUDA_FOUND}")


IF (NOT OPENCV2_FOUND)
    IF (OPENCV2_FIND_REQUIRED)
        #MESSAGE (FATAL_ERROR "OpenCV ver 2 not found.") 
    ELSE ()
        IF (NOT OPENCV2_FIND_QUIETLY)
            MESSAGE (STATUS "OpenCV ver 2 not found.")
        ENDIF ()
    ENDIF ()
ENDIF ()
