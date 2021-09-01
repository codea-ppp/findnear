cmake_minimum_required(VERSION 3.0)
project(findnear)

set(DEBUG)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

add_executable(${PROJECT_NAME} 
	src/stupid_dir.cpp
	src/main.cpp
	)
target_link_libraries(${PROJECT_NAME}
	gflags
	)

add_definitions(
	-Wall
	-Wextra
	-g3
	-O0
	)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
