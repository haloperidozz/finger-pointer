cmake_minimum_required(VERSION 3.20)
project(FingerPointer VERSION 1.0 LANGUAGES C)

if(NOT WIN32)
    message(FATAL_ERROR
        "FingerPointer is intended to be built on Windows only."
    )
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(RES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res)

file(GLOB SRC_FILES ${SRC_DIR}/*.c ${RES_DIR}/resource.rc)

add_executable(FingerPointer WIN32 ${SRC_FILES})

target_compile_definitions(FingerPointer PRIVATE _UNICODE UNICODE)

target_compile_features(FingerPointer PRIVATE c_std_99)

target_compile_definitions(FingerPointer
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
)

target_include_directories(FingerPointer
    PRIVATE
        ${SRC_DIR}
        ${RES_DIR}
)

target_link_libraries(FingerPointer
    PRIVATE
        Shell32 d2d1 windowscodecs Winmm Dwmapi
)

if(MINGW)
    target_compile_options(FingerPointer PRIVATE -municode)

    target_link_options(FingerPointer
        PRIVATE
            -municode -static-libgcc -static-libstdc++ -static
    )
endif()