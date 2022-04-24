set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(include)
set(MODEL_LIB model_lib)
set(VIEW_LIB view_lib)

find_package(Qt5 COMPONENTS
    Gui
    Core
    Widgets
    REQUIRED)

# get files for view and view-model library
file(GLOB HEADERS ./include/*.h)
file(GLOB UIS ./view/*.ui)
file(GLOB VIEW_MODELS ./controller/*.cpp)

# get files for model library
file(GLOB LIB_FILES ./model/*.cpp)

# create executable
add_executable(${PROJECT_NAME} main.cpp)

# create libraries
add_library(${MODEL_LIB} SHARED ${LIB_FILES})
target_link_libraries(${MODEL_LIB} Qt5::Core)
add_library(${VIEW_LIB} ${HEADERS} ${VIEW_MODELS} ${UIS})
target_link_libraries(${VIEW_LIB}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        ${MODEL_LIB})

# add libraries to main file
target_link_libraries(${PROJECT_NAME} PRIVATE ${VIEW_LIB})
