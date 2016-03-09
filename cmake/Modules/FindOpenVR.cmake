# - Try to find the labstreaminglayer library
#
#  OpenVR_FOUND - system has OpenVR
#  OpenVR_INCLUDE_DIRS - the OpenVR include directory
#  OpenVR_LIBRARIES - Link these to use OpenVR
#  OpenVR_BINARIES - DLLs that must be on the path


IF (OpenVR_INCLUDE_DIRS AND OpenVR_LIBRARIES)

    # in cache already
    set(OpenVR_FOUND TRUE)

ELSE (OpenVR_INCLUDE_DIRS AND OpenVR_LIBRARIES)
    set(OpenVR_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../../openvr CACHE PATH "Path to local openvr SDK")
    # Because we want to use the static library,
    # look locally only.
    find_path(OpenVR_INCLUDE_DIRS
        NAMES
            openvr.h
        PATHS 
            ${OpenVR_ROOT}
        PATH_SUFFIXES
            headers
    )
    
    set(OpenVR_PLAT_DIR)
    set(OpenVR_LIB_NAME)
    set(OpenVR_BIN_NAME)
    IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        IF (${CMAKE_C_SIZEOF_DATA_PTR} EQUAL 8)
            list(APPEND OpenVR_PLAT_DIR
                win64)
        ELSE()
            list(APPEND OpenVR_PLAT_DIR
                win32)
        ENDIF()
        set(OpenVR_LIB_NAME openvr_api.lib)
        set(OpenVR_BIN_NAME openvr_api.dll)
    ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        list(APPEND OpenVR_PLAT_DIR
                osx32)
        set(OpenVR_LIB_NAME libopenvr_api.dylib)
        set(OpenVR_BIN_NAME libopenvr_api.dylib)
    ELSE() #Linux
        IF (${CMAKE_C_SIZEOF_DATA_PTR} EQUAL 8)
            list(APPEND OpenVR_PLAT_DIR
                linux64)
        ELSE ()
            list(APPEND OpenVR_PLAT_DIR
                linux32)
        ENDIF()
        set(OpenVR_LIB_NAME libopenvr_api.so)
        set(OpenVR_BIN_NAME libopenvr_api.so)
    ENDIF()
    
    SET(LIB_PATH "${OpenVR_ROOT}/lib/${OpenVR_PLAT_DIR}")
    FIND_LIBRARY(OpenVR_LIBRARY_RELEASE
            NAMES ${OpenVR_LIB_NAME}
            PATHS ${LIB_PATH})
    SET(OpenVR_LIBRARIES
        ${OpenVR_LIBRARY_RELEASE})
        
    #SET(BIN_PATH "${OpenVR_ROOT}/bin/${OpenVR_PLAT_DIR}")
    #FIND_LIBRARY(OpenVR_BINARIES
    #    NAMES ${OpenVR_BIN_NAME}
    #    PATHS ${BIN_PATH})

  include(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenVR DEFAULT_MSG OpenVR_LIBRARIES OpenVR_INCLUDE_DIRS)

  MARK_AS_ADVANCED(OpenVR_INCLUDE_DIRS OpenVR_LIBRARIES OpenVR_BINARIES)

ENDIF (OpenVR_INCLUDE_DIRS AND OpenVR_LIBRARIES)