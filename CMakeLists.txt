cmake_minimum_required(VERSION 3.20)
project(gumsmaqqer CXX C ASM)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

file(GLOB SRC
        src/*.cc
        src/*.h
        src/*.S
        lib/*.h
        lib/*.cc
        )

if (MSVC)
    add_compile_definitions(EXECUTABLE_NAME="gumsmaqqer.exe")
else ()
    add_compile_definitions(EXECUTABLE_NAME="gumsmaqqer")
endif ()

add_executable(gumsmaqqer ${SRC})

if (NOT (WIN32))
    if (EXISTS "./version.p6")
        add_custom_command(
                COMMAND ./version.p6
                TARGET gumsmaqqer
                WORKING_DIRECTORY ..
        )
    endif ()
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_BUILD)
endif ()

if (MSVC)
    target_compile_options(gumsmaqqer PRIVATE)
else ()
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-O0 -g)
    else ()
        add_compile_options(-O3)
    endif ()
    add_compile_options(-Wall -Wextra -Wundef -Wno-missing-field-initializers -Wno-sign-compare)
endif ()

if (WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif ()