# - Try to find the labstreaminglayer library
#
#  LSL_FOUND - system has lsl
#  LSL_INCLUDE_DIRS - the lsl include directory
#  LSL_LIBRARIES - Link these to use lsl


IF (LSL_INCLUDE_DIRS AND LSL_LIBRARIES)

    # in cache already
    set(LSL_FOUND TRUE)

ELSE (LSL_INCLUDE_DIRS AND LSL_LIBRARIES)
    set(LSL_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../../../Neurophys/labstreaminglayer/LSL/liblsl CACHE PATH "Path to local liblsl")
    
    # Because we want to use the static library,
    # look locally only.
    find_path(LSL_INCLUDE_DIRS
        NAMES
            lsl_cpp.h
        PATHS 
            ${LSL_ROOT}
        PATH_SUFFIXES
            include
    )
        
    set(LSL_LIB_NAMES_RELEASE)
    set(LSL_LIB_NAMES_DEBUG)
    IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        IF (${CMAKE_C_SIZEOF_DATA_PTR} EQUAL 8)
            list(APPEND LSL_LIB_NAMES_RELEASE
                liblsl64.lib)
            list(APPEND LSL_LIB_NAMES_DEBUG
                liblsl64-debug.lib)
        ELSE()
            list(APPEND LSL_LIB_NAMES_RELEASE
                liblsl32.lib)
            list(APPEND LSL_LIB_NAMES_DEBUG
                liblsl32-debug.lib)
        ENDIF()
    ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        list(APPEND LSL_LIB_NAMES_RELEASE
                    liblsl64.dylib)
        list(APPEND LSL_LIB_NAMES_DEBUG
                    liblsl64-debug.dylib)
    ELSE() #Linux
        list(APPEND LSL_LIB_NAMES_RELEASE
                    liblsl64.so)
        list(APPEND LSL_LIB_NAMES_DEBUG
                    liblsl64-debug.so)
    ENDIF()
    
    FIND_LIBRARY(LSL_LIBRARY_RELEASE
            NAMES ${LSL_LIB_NAMES_RELEASE}
            PATHS ${LSL_ROOT}/bin)
    FIND_LIBRARY(LSL_LIBRARY_DEBUG
            NAMES ${LSL_LIB_NAMES_DEBUG}
            PATHS ${LSL_ROOT}/bin)
    SET(LSL_LIBRARIES
        debug ${LSL_LIBRARY_DEBUG}
        optimized ${LSL_LIBRARY_RELEASE})

  include(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(LSL DEFAULT_MSG LSL_LIBRARIES LSL_INCLUDE_DIRS)

  MARK_AS_ADVANCED(LSL_INCLUDE_DIRS LSL_LIBRARIES)

ENDIF (LSL_INCLUDE_DIRS AND LSL_LIBRARIES)