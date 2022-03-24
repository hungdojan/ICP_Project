set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(include)

find_package(Qt5 COMPONENTS
    Gui
    Core
    Widgets
    REQUIRED)

# get files for view and view-model library
file(GLOB HEADERS ./include/*.h)
file(GLOB UIS ./view/*.ui)
file(GLOB VIEW_MODELS controller/*.cpp)

# get files for model library
file(GLOB LIB_FILES ./model/*.cpp)

# create executable
add_executable(${PROJECT_NAME} main.cpp)

# create libraries
add_library(model_lib ${LIB_FILES})
add_library(view_lib ${HEADERS} ${VIEW_MODELS} ${UIS}) # ${UI_HEADERS})
target_link_libraries(view_lib
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets)

# add libraries to main file
target_link_libraries(${PROJECT_NAME} model_lib)
target_link_libraries(${PROJECT_NAME} view_lib)
