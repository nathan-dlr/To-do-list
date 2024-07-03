cmake_minimum_required(VERSION 3.14)
project(To_do)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

find_package(wxWidgets COMPONENTS core base REQUIRED)

include("${wxWidgets_USE_FILE}")

set(SRCS main.cpp)

if (WIN32)
    set(SRCS ${SRCS} wxWidgetTest.rc main.cpp
            To_do.cpp)
endif (WIN32)


add_executable(${PROJECT_NAME} WIN32 ${SRCS})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

