set(CMAKE_CXX_STANDARD 23)
cmake_minimum_required(VERSION 3.14)
project(To_do)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

find_package(wxWidgets COMPONENTS core base REQUIRED)

include("${wxWidgets_USE_FILE}")

set(SRCS main.cpp
        sqlite3.c
        database.h
        database.cpp
        Task.h
        Task.cpp
)

if (WIN32)
    set(SRCS ${SRCS} wxWidgetTest.rc main.cpp
            To_do.cpp
            sqlite3.c
            database.cpp
            database.h
            Task.cpp
            Task.h
    )
endif (WIN32)


add_executable(${PROJECT_NAME} WIN32 ${SRCS}
        sqlite3.c
        database.h
        database.cpp
        Task.h
        Task.cpp)
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

