cmake_minimum_required(VERSION 3.0)
project(findnear)

set(DEBUG)

add_executable(${PROJECT_NAME} 
	src/stupid_dir.cpp
	src/main.cpp
	)
add_definitions(
	-Wall
	-Wextra
	-g3
	-O0
	)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
