# Image Processing from Scratch

## Introduction

This is my course project for Image Processing course at HIT.

In this project, I implemented the following common image-processing procedures.

All procedures except image io are implemented from scratch, without using any external library.

The IO module that involves image read/write/show uses [OpenCV4](https://opencv.org).

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
