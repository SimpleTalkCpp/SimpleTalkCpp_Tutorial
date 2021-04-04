cmake_minimum_required(VERSION 3.10)

project(Ex002)

add_subdirectory(MyLib)

add_executable(MyExe MyExe/main.cpp MyExe/pch.h)

target_precompile_headers(MyExe PRIVATE MyExe/pch.h)
target_link_libraries(MyExe MyLib)