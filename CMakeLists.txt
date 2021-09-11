cmake_minimum_required(VERSION 3.0)
project(findnear)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

add_executable(${PROJECT_NAME} 
	src/stupid_dir.cpp
	src/main.cpp
	)
target_link_libraries(${PROJECT_NAME}
	gflags
	)

add_executable(${PROJECT_NAME}_test
	test/findnear_test.cpp
	test/main.cpp
	src/stupid_dir.cpp
	)

target_link_libraries(${PROJECT_NAME}_test
	gtest
	)	

add_definitions(
	-Wall
	-Wextra
	-O3
	)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME}_test PUBLIC ${PROJECT_SOURCE_DIR})
