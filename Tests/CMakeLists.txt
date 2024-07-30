cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 23)

find_package(wxWidgets COMPONENTS core base REQUIRED)
include("${wxWidgets_USE_FILE}")

# Add test source files
set(TEST_SRCS
        tests.cpp
)

if (WIN32)
    set(SRCS ${SRCS}
            wxWidgetTest.rc
            tests.cpp
    )
endif (WIN32)

# Add the test executable
add_executable(To_do_tests ${TEST_SRCS})
target_link_libraries(To_do_tests PRIVATE To_do_lib)
target_include_directories(To_do_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(To_do_tests PRIVATE ${wxWidgets_LIBRARIES})
