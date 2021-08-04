cmake_minimum_required(VERSION 3.20)
project(gumsmaqqer CXX C ASM)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

if (WIN32)
else ()
    set(CMAKE_CXX_COMPILER clang++)
endif ()

file(GLOB SRC
        src/*.cc
        src/*.h
        src/*.S
        lib/*.cc
        lib/*.h
        lib/*.S)

add_executable(gumsmaqqer ${SRC})

if (WIN32)
else ()
    target_compile_options(gumsmaqqer PRIVATE -Wall -Wextra -Wundef -march=native -Wno-unused-parameter -Wno-unused-variable)
endif ()