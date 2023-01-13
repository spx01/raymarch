cmake_minimum_required(VERSION 3.24)
project(raymarch)

set(CMAKE_CXX_STANDARD 23)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(raymarch main.cpp)
target_link_libraries(raymarch ${CONAN_LIBS})
