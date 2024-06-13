cmake_minimum_required(VERSION 3.8)
project(anya)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(WIN32)
	add_compile_options(--target=i686-pc-windows-gnu -std=c++17 -g -Wall -Wextra -pedantic)
else()
	add_compile_options(-std=c++11 -g -Wall -Wextra -pedantic)
endif(WIN32)


find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
if (UNIX)
	find_package(GLEW REQUIRED)
endif(UNIX)

add_executable(anya 
	${CMAKE_BINARY_DIR}/src/core.cpp
	${CMAKE_BINARY_DIR}/src/EventProcessor.cpp
	${CMAKE_BINARY_DIR}/src/EventProcessor.h
	${CMAKE_BINARY_DIR}/src/GameLogic.h
	${CMAKE_BINARY_DIR}/src/GameLogic.cpp
)


if (WIN32)
	set_target_properties(anya PROPERTIES WIN32_EXECUTABLE FALSE)
endif(WIN32)

target_include_directories(anya
	PUBLIC ${CMAKE_BINARY_DIR/include}
    PUBLIC ${SDL2_INCLUDE_DIRS}
    PUBLIC ${MinGW_INCLUDE_DIRS}
	PUBLIC ${GLEW_INCLUDE_DIRS}
	if(WIN32)
		PUBLIC "C:/glew-2.2.0/include"
	endif(WIN32)
)

target_link_libraries(anya PUBLIC ${SDL2_LIBRARIES})
target_link_libraries(anya PUBLIC ${GLEW_LIBRARIES})
target_link_libraries(anya PUBLIC ${OPENGL_LIBRARIES})
if(WIN32)
	target_link_libraries(anya PRIVATE "C:/glew-2.2.0/lib/glew32.lib")
endif(WIN32)

