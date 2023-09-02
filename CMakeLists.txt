cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD_11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)



project(QtImageScanner VERSION 1.0 LANGUAGES CXX)

configure_file(QtImageScannerConfig.h.in QtImageScannerConfig.h)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED )

add_executable(QtImageScanner
        main.cpp
        mainwindow.cpp
        MyGraphicsView.cpp
)

target_include_directories(QtImageScanner PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${OpenCV_INCLUDE_DIRS}")


target_link_libraries(QtImageScanner Qt5::Widgets "${OpenCV_LIBS}")
