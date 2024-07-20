set(CMAKE_CXX_STANDARD 23)
cmake_minimum_required(VERSION 3.14)
project(To_do)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

find_package(wxWidgets COMPONENTS core base REQUIRED)

include("${wxWidgets_USE_FILE}")

set(SRCS MainFrame.cpp
        sqlite3.c
        Database.h
        Database.cpp
        Task.h
        Task.cpp
        MainFrame.h
        App.h
        App.cpp
)

if (WIN32)
    set(SRCS ${SRCS} wxWidgetTest.rc MainFrame.cpp
            To_do.cpp
            sqlite3.c
            Database.cpp
            Database.h
            Task.cpp
            Task.h
            MainFrame.h
            MainFrame.h
            MainFrame.h
            App.h
            App.h
            App.cpp
            App.cpp
    )
endif (WIN32)


add_executable(${PROJECT_NAME} WIN32 ${SRCS}
        sqlite3.c
        Database.h
        Database.cpp
        Task.h
        Task.cpp
        MainFrame.h
        App.h
        App.cpp)
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

