cmake_minimum_required(VERSION 3.20)
project(gumsmaqqer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

file(GLOB SRC
        src/*.cc
        src/*.h)

add_executable(gumsmaqqer ${SRC})
target_compile_options(gumsmaqqer PRIVATE -Wall -Wextra -Wundef)