#TODO: find package for OpenVR and labstreaminglayer
set(OPENVR_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../openvr CACHE PATH "Path to local openvr SDK" FORCE)
set(LSL_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../../Neurophys/labstreaminglayer/LSL/liblsl CACHE PATH "Path to local liblsl" FORCE)

SET(SendOpenVR_SRC)
SET(SendOpenVR_INCL_DIRS)
SET(SendOpenVR_REQ_LIBS)

FIND_PACKAGE(OpenVR REQUIRED)
list(APPEND SendOpenVR_INCL_DIRS ${OpenVR_INCLUDE_DIRS})
list(APPEND SendOpenVR_REQ_LIBS ${OpenVR_LIBRARIES})

FIND_PACKAGE(labstreaminglayer REQUIRED)
list(APPEND SendOpenVR_INCL_DIRS ${labstreaminglayer_INCLUDE_DIRS})
list(APPEND SendOpenVR_REQ_LIBS ${labstreaminglayer_LIBRARIES})

list(APPEND SendOpenVR_SRC ${CMAKE_CURRENT_LIST_DIR}/SendOpenVR.cpp)

add_executable(SendOpenVR ${SendOpenVR_SRC})
target_include_directories(SendOpenVR PUBLIC ${SendOpenVR_INCL_DIRS})
target_link_libraries(SendOpenVR ${SendOpenVR_REQ_LIBS})

# Install    
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    IF(${CMAKE_C_SIZEOF_DATA_PTR} EQUAL 8)
        #TODO: 64-bit
    ELSE()  # 32-bit
        install(TARGETS SendOpenVR
            RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../win32/bin)
        # TODO: Copy necessary DLLs to the same directory
    ENDIF()
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
ELSE()  # Linux
ENDIF()