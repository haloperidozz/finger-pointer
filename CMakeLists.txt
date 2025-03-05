cmake_minimum_required(VERSION 3.20)
project(FingerPointer VERSION 1.1 LANGUAGES C)

if(NOT WIN32)
    message(FATAL_ERROR "FingerPointer is intended for Windows only.")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB SRC_FILES ${SRC_DIR}/*.c ${SRC_DIR}/resource.rc)

add_executable(FingerPointer WIN32 ${SRC_FILES})

target_compile_definitions(FingerPointer PRIVATE _UNICODE UNICODE)

target_compile_features(FingerPointer PRIVATE c_std_99)

target_include_directories(FingerPointer PRIVATE ${SRC_DIR})

target_link_libraries(FingerPointer PRIVATE 
    Shlwapi
    d2d1
    windowscodecs 
    Winmm 
    Dwmapi
    Mfplat
    Mfplay
    Mfuuid
)

if(MINGW)
    target_compile_options(FingerPointer PRIVATE -municode)
    set_target_properties(FingerPointer PROPERTIES LINK_FLAGS_RELEASE -s)
    target_link_options(FingerPointer PRIVATE 
        -municode 
        -static-libgcc 
        -static-libstdc++ 
        -static
    )
endif()

if(MSVC)
    target_compile_definitions(FingerPointer PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()