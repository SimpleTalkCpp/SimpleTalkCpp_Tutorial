cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE vcpkg/scripts/buildsystems/vcpkg.cmake)

project(Ex004)

find_package(fmt)
find_package(imgui)

add_executable(MyExe imgui_demo.cpp)

target_link_libraries(MyExe fmt::fmt imgui::imgui)