cmake_minimum_required(VERSION 3.10)

project(Ex003)

# compile fmt
# add_subdirectory(fmt)


set(fmt_DIR fmt/_build)
find_package(fmt)

add_executable(MyExe main.cpp)

target_link_libraries(MyExe fmt::fmt)