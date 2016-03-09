#TODO: find package for OpenVR and LSL

SET(SendOpenVR_SRC)
SET(SendOpenVR_INCL_DIRS)
SET(SendOpenVR_REQ_LIBS)

FIND_PACKAGE(LSL REQUIRED)
list(APPEND SendOpenVR_INCL_DIRS ${LSL_INCLUDE_DIRS})
list(APPEND SendOpenVR_REQ_LIBS ${LSL_LIBRARIES})

FIND_PACKAGE(OpenVR REQUIRED)
list(APPEND SendOpenVR_INCL_DIRS ${OpenVR_INCLUDE_DIRS})
list(APPEND SendOpenVR_REQ_LIBS ${OpenVR_LIBRARIES})

list(APPEND SendOpenVR_SRC ${CMAKE_CURRENT_LIST_DIR}/SendOpenVR.cpp)

add_executable(SendOpenVR ${SendOpenVR_SRC})
target_include_directories(SendOpenVR PUBLIC ${SendOpenVR_INCL_DIRS})
target_link_libraries(SendOpenVR ${SendOpenVR_REQ_LIBS})

# Install
#TODO: Try cmake's BundleUtilities
#e.g. https://github.com/highfidelity/hifi/blob/da709f3f566ae61d3fdef01ce16ccd193b732961/BUILD_WIN.md#external-libraries
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    IF(${CMAKE_C_SIZEOF_DATA_PTR} EQUAL 8)
        install(TARGETS SendOpenVR
            RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../win64/bin)
        # TODO: Copy necessary DLLs to the same directory. Try FIXUP_BUNDLE
    ELSE()  # 32-bit
        install(TARGETS SendOpenVR
            RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../win32/bin)
        # TODO: Copy necessary DLLs to the same directory. Try FIXUP_BUNDLE
    ENDIF()
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
ELSE()  # Linux
ENDIF()