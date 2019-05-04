// =======================================================================
// Copyright (c) 2016-2020 by LI YANZHE. All rights reserved.
// Author: LI YANZHE <lee.yanzhe@yanzhe.org>
// Created by LI YANZHE on 5/1/19.
// =======================================================================
#include "io.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "imgproc.h"

using namespace std;

template<typename T>
inline void reverse_channels(const T *src, T *dst, int rows, int cols, int channels) {
  for (int i = 0; i < rows * cols * channels; i += channels)
    for (int j = 0; j < channels; ++j)
      dst[i + j] = src[i + channels - j - 1];
}

inline cv::Mat toMat(const unsigned char *src, int rows, int cols, int channels) {
  int type = CV_8UC(channels);
  auto bgr = new unsigned char[rows * cols * channels];
  reverse_channels(src, bgr, rows, cols, channels);
  return {rows, cols, type, (void *) bgr};
}

void lyz::imwrite(const string path, const unsigned char *src, int rows, int cols, int channels) {
  cv::imwrite(path, toMat(src, rows, cols, channels));
}

void lyz::imread(const string path,
                 unsigned char *&dst,
                 int &rows,
                 int &cols,
                 int &channels,
                 bool is_grey = false) {
  int flag = is_grey ? cv::IMREAD_GRAYSCALE : cv::IMREAD_COLOR;

  cv::Mat img = cv::imread(path, flag); // BGR
  rows = img.rows;
  cols = img.cols;
  channels = img.channels();
  dst = new unsigned char[rows * cols * channels];
  reverse_channels(img.data, dst, rows, cols, channels); //RGB
}

void lyz::imshow(const string name, const unsigned char *src, int rows, int cols, unsigned int channels) {
  cv::imshow(name, toMat(src, rows, cols, channels));
  imwrite("../output/" + name + ".png ", src, rows, cols, channels);
  cv::waitKey();
}

void lyz::plot1d(const string name, const int *src, int n = 256) {
  constexpr int hist_w = 2048;
  constexpr int hist_h = 1024;
  const int bin_w = ROUND((double) hist_w / (2 * n));
  cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(255, 255, 255));
  cv::Mat mhist = cv::Mat::zeros(n, 1, CV_32F);
  for (int i = 0; i < n; ++i)
    mhist.at<float>(i) = src[i];
  normalize(mhist, mhist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
  for (int bin = 0; bin < n; ++bin) {
    int start = bin * bin_w * 2;
    line(
        histImage,
        cv::Point(start, hist_h),
        cv::Point(start, hist_h - mhist.at<float>(bin)),
        {255, 0, 0},
        bin_w,
        cv::LINE_8,
        0
    );
  }
  cv::imshow(name, histImage);
  cv::imwrite("../output/" + name + ".png ", histImage);
  cv::waitKey();
}