cmake_minimum_required(VERSION 3.5)

project(improclab)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(
        improclab
        main.cc
        imgproc.h
        io.cc
        io.h
        color_convert.h
        histogram.h
        filter.h
        benchmark.h
)

#target_link_libraries(improclab ${OpenCV_LIBRARIES})
target_link_libraries(improclab opencv_world)