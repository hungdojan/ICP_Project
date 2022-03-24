cmake_minimum_required(VERSION 3.15)
project(DiagramEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(DiagramEditor
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

