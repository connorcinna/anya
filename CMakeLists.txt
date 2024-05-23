cmake_minimum_required(VERSION 3.8)
project(sdl_app)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Change path from /src if needed, or add more directories
file(GLOB_RECURSE sources
        "C:/projects/sdl/sdl.cpp"
        )


# Add more include directories if needed
link_directories("C:/sdl2_mingw32/lib")
link_directories("C:/MinGW/lib")

add_executable(sdl_app ${sources})

set_target_properties(sdl_app PROPERTIES WIN32_EXECUTABLE FALSE)

find_package(SDL2 REQUIRED)

target_include_directories(sdl_app
        PUBLIC ${SDL2_INCLUDE_DIRS}
        PUBLIC ${MinGW_INCLUDE_DIRS}
		PUBLIC "C:/MinGW/lib/gcc/mingw32/6.3.0/include"
)

target_link_libraries(sdl_app PUBLIC ${SDL2_LIBRARIES})
