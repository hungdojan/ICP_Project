cmake_minimum_required(VERSION 3.15)
project(DiagramEditor)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)     # .so
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)     # .a

add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=0)
add_subdirectory(src)
add_subdirectory(tests)