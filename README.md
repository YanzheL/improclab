# Image Processing from Scratch

## Introduction

This is my course project for Image Processing at HIT.

In this project, I implemented the following common image-processing procedures.

All procedures except image io are implemented from scratch, without using any external library.

The IO module that involves image read/write/show uses [OpenCV](https://opencv.org).

Maybe I will write those image-reading functions by myself if I have extra time. :)

#### Color Converters

- RGB<->YUV color space converter
- RGB<->HSI color space converter
- RGB<->HSV color space converter

#### Histogram-Based Image Enhancing

- Histogram Equalization
- Histogram Specification

#### Noise Filters

- Median Filter
- Fast Median Filter
- Mean Filter
- Fast Mean Filter
- Gradient Filters: Sobel, Roberts, Prewitt

## Results

The outputs of all test cases in `main.cc` are located in [output](output) directory. Those file names are self-explanatory.

## Getting Started

This project uses [CMake](https://cmake.org/) and C++ 14 standard. Here are some options for you to get it compiled.

#### Compiling With IO Module `io.cc` & `io.h`

You need to have OpenCV installed in your system first, then follow these steps in project directory.

Create build directory

```shell
mkdir build && cd build
```

Build the project

```shell
cmake ..
make
```

Run the demo

```shell
./improclab
```

#### Compiling Without IO Module `io.cc` & `io.h`

You cannot run the demo if no IO module presents. But you may still integrate this project in your own projects.

Just remove `main.cc`, `io.cc`, `io.h`, and have fun. At this time, no OpenCV required.

#### Compiling With C++ Standard < 14

The utility function `IDX(...)` in `util.h` is a black magic that uses variadic template pack. It converts multi-demensional indices to 1D(linear) indice in a general way. To understand its use case, you can see [My answer at StackOverflow](https://stackoverflow.com/a/55981266/11211417).

Its definition requires some features of C++ 14. If C++ 14 is not available in your project, you can just remove it and use the macros `IDX_2D()` and `IDX_3D()` instead, which are defined at the top of `util.h`.

`IDX(x,y,w)` can be replaced by `IDX_2D(x,y,w)`

`IDX(x,y,z,w,d)` can be replaced by `IDX_3D(x,y,z,w,d)`

Now this project is C++ 11 compatible!
