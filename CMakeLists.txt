cmake_minimum_required(VERSION 3.8)
project(anya)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Add more include directories if needed
link_directories("C:/sdl2_mingw32/lib")
link_directories("C:/MinGW/lib")

add_compile_options(--target=i686-pc-windows-gnu)

add_executable(anya 
	${CMAKE_BINARY_DIR}/src/core.cpp
	${CMAKE_BINARY_DIR}/src/EventProcessor.cpp
	${CMAKE_BINARY_DIR}/src/EventProcessor.h
)


set_target_properties(anya PROPERTIES WIN32_EXECUTABLE FALSE)

find_package(SDL2 REQUIRED)

target_include_directories(anya
        PUBLIC ${SDL2_INCLUDE_DIRS}
        PUBLIC ${MinGW_INCLUDE_DIRS}
		PUBLIC "C:/MinGW/lib/gcc/mingw32/6.3.0/include"
)

target_link_libraries(anya PUBLIC ${SDL2_LIBRARIES})
