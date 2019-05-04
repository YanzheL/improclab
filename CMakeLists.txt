cmake_minimum_required(VERSION 3.5)

project(improclab)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-fstandalone-debug")

find_package(OpenCV REQUIRED)

message(STATUS ${OpenCV_INCLUDE_DIRS})

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(
        improclab
        main.cc
        imgproc.h
        imgio.cc
        imgio.h
        color_convert.h
        histogram.h
        filter.h
        benchmark.h
)

target_link_libraries(improclab ${OpenCV_LIBRARIES})