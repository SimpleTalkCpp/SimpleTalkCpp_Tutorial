cmake_minimum_required(VERSION 3.10)

project(MyLib)

set(ABC "Hey")
message("Hello World ${ABC}")

set(src_files src/MyLib.cpp include/MyLib.h)
#add_library(MyLib ${src_files})

file(GLOB_RECURSE search_src_files 
	"include/*.*"
	"src/*.*"
	)
message("search_src_files = ${search_src_files}")

#source_group(Test files ${src_files})

foreach(FILE ${search_src_files}) 
	get_filename_component(PARENT_DIR "${FILE}" PATH)
	file(RELATIVE_PATH PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} ${PARENT_DIR})
	string(REPLACE "/" "\\" GROUP "${PARENT_DIR}")
	set(GROUP "${GROUP}")
	source_group("${GROUP}" FILES "${FILE}")
endforeach()


add_library(MyLib ${search_src_files})

target_include_directories(MyLib PUBLIC include)
