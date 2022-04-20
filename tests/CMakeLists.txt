set(INSTALL_GTEST OFF CACHE BOOL "Install gtest library from subproject")
# import header from src directory
include_directories(../src/include)

# include subdirectories for model library and google tests
add_subdirectory(googletest)

file(GLOB SOURCE *.cpp)
# create executable
add_executable(tests ${SOURCE})
target_link_libraries(tests PRIVATE gtest gtest_main model_lib)