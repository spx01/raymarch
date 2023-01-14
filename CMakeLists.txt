cmake_minimum_required(VERSION 3.24)
project(raymarch)

set(CMAKE_CXX_STANDARD 23)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(raymarch src/imgui_impl_glfw.cpp src/imgui_impl_opengl3.cpp src/application.cpp src/application.hpp src/app_gui.cpp src/app_gui.hpp src/common.hpp src/main.cpp )
target_link_libraries(raymarch ${CONAN_LIBS})
