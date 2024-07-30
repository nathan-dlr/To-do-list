set(CMAKE_CXX_STANDARD 23)
cmake_minimum_required(VERSION 3.14)
project(To_do)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

find_package(wxWidgets COMPONENTS core base REQUIRED)

include("${wxWidgets_USE_FILE}")

set(SRCS
        src/App.cpp
        src/App.h
        src/Database.cpp
        src/Database.h
        src/catch.hpp
        src/main.cpp
        src/MainFrame.cpp
        src/MainFrame.h
        src/sqlite3.c
        src/Task.h
        src/Task.cpp
)

if (WIN32)
    set(SRCS ${SRCS} wxWidgetTest.rc
            src/App.cpp
            src/App.h
            src/Database.cpp
            src/Database.h
            src/catch.hpp
            src/main.cpp
            src/MainFrame.cpp
            src/MainFrame.h
            src/sqlite3.c
            src/Task.h
            src/Task.cpp
    )
endif (WIN32)


add_executable(${PROJECT_NAME} WIN32 ${SRCS}
        src/App.cpp
        src/App.h
        src/Database.cpp
        src/Database.h
        src/catch.hpp
        src/main.cpp
        src/MainFrame.cpp
        src/MainFrame.h
        src/sqlite3.c
        src/Task.h
        src/Task.cpp
)
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

add_library(To_do_lib ${SRCS})

target_include_directories(To_do_lib PUBLIC ${CMAKE_SOURCE_DIR}/src)

enable_testing()
add_subdirectory(Tests)