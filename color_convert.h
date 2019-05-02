// =======================================================================
// Copyright (c) 2016-2020 by LI YANZHE. All rights reserved.
// Author: LI YANZHE <lee.yanzhe@yanzhe.org>
// Created by LI YANZHE on 4/15/19.
// =======================================================================
#ifndef IMPROCLAB_COLOR_CONVERT_H
#define IMPROCLAB_COLOR_CONVERT_H

#include "imgproc.h"
#include <cstring>

namespace lyz {

/**
 * RGB 转 YUV
 * @param src 输入RGB图像, 三通道需要事先归一化
 * @param rows 图像长
 * @param cols 图像宽
 * @param dst_cn 输出3通道
 */
inline void rgb2yuv(const float *src, int rows, int cols, float **dst_cn) {
  constexpr float coef[] = {
      0.299, 0.587, 0.114,
      -0.147, -0.289, 0.436,
      0.615, -0.515, -0.1
  };
  float **src_cn = alloc_2d<float>(3, rows * cols);
  split(src, src_cn, rows, cols);
  matmul(coef, src_cn, dst_cn, 3, 3, rows * cols); // dst_cn = coef x src_cn
  delete_2d(src_cn, 3);
}

/**
 * YUV 转 RGB
 * @param src 输入RGB图像, 三通道需要事先归一化
 * @param rows 图像长
 * @param cols 图像宽
 * @param dst_cn 输出3通道
 */
inline void yuv2rgb(const float *src, int rows, int cols, float **dst_cn) {
  constexpr float coef[] = {
      1, 0, 1.14,
      1, -0.395, -0.581,
      1, 2.032, 0
  };
  float **src_cn = alloc_2d<float>(3, rows * cols);
  split(src, src_cn, rows, cols);
  matmul(coef, src_cn, dst_cn, 3, 3, rows * cols); // dst_cn = coef x src_cn
  delete_2d(src_cn, 3);
}

/**
 * RGB 转 HSI
 * @param src 输入RGB图像, 三通道需要事先归一化
 * @param rows 图像长
 * @param cols 图像宽
 * @param dst_cn 输出3通道
 */
inline void rgb2hsi(const float *src, int rows, int cols, float **dst_cn) {
  float *hc = dst_cn[0];
  float *sc = dst_cn[1];
  float *ic = dst_cn[2];
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      float r = src[IDX_3D(i, j, 0, cols, 3)];
      float g = src[IDX_3D(i, j, 1, cols, 3)];
      float b = src[IDX_3D(i, j, 2, cols, 3)];
      const float mean = (r + g + b) / 3;
      float h = 0, s = 0;
      if (IS_ZERO(mean)) {
        s = 1;
      } else {
        float min_val = MIN(r, MIN(b, g));
        s = 1 - 3 * (min_val / (b + g + r));
      }
      assert(s >= 0 && s <= 1);
      s = ROUND_TO_ZERO(s);
      if (!IS_ZERO(s)) {
        float tp = sqrt((r - g) * (r - g) + (r - b) * (g - b));
        h = IS_ZERO(tp) ? 1 : 0.5 * ((r - g) + (r - b)) / tp;
        assert(std::fabs(h) <= 1);
        h = acos(h);
        h = b > g ? 2 * PI - h : h;
      } else
        s = 0;
      h = ROUND_TO_ZERO(h);
      assert(h >= 0 && h < 2 * PI);
      hc[IDX_2D(i, j, cols)] = h;
      sc[IDX_2D(i, j, cols)] = s;
      ic[IDX_2D(i, j, cols)] = mean;
    }
  }
}

/**
 * HSI 转 RGB
 * @param src 输入HSI图像
 * @param rows 图像长
 * @param cols 图像宽
 * @param dst_cn 输出3通道
 */
inline void hsi2rgb(const float *src, int rows, int cols, float **dst_cn) {
  float *rc = dst_cn[0];
  float *gc = dst_cn[1];
  float *bc = dst_cn[2];
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      float h = src[IDX_3D(i, j, 0, cols, 3)];
      float s = src[IDX_3D(i, j, 1, cols, 3)];
      const float mean = src[IDX_3D(i, j, 2, cols, 3)];
      assert(h >= 0 && h <= 2 * PI);
      float r, g, b;
      if (h >= 0 && h < 2 * PI / 3) {
        b = mean * (1 - s);
        r = mean * (1 + (s * cos(h)) / cos(PI / 3 - h));
        g = 3 * mean - (b + r);
      } else if (h >= 2 * PI / 3 && h <= 4 * PI / 3) {
        r = mean * (1 - s);
        g = mean * (1 + (s * cos(h - 2 * PI / 3)) / cos(PI - h));
        b = 3 * mean - (r + g);
      } else {
        g = mean * (1 - s);
        b = mean * (1 + (s * cos(h - 4 * PI / 3)) / cos(5 * PI / 3 - h));
        r = 3 * mean - (b + g);
      }
      rc[IDX_2D(i, j, cols)] = r;
      bc[IDX_2D(i, j, cols)] = b;
      gc[IDX_2D(i, j, cols)] = g;
    }
  }
}

/**
 * RGB 转 HSV
 * @param src 输入RGB图像, 三通道需要事先归一化
 * @param rows 图像长
 * @param cols 图像宽
 * @param dst_cn 输出3通道
 */
inline void rgb2hsv(const float *src, int rows, int cols, float **dst_cn) {
  float *hc = dst_cn[0];
  float *sc = dst_cn[1];
  float *vc = dst_cn[2];
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      const float r = src[IDX_3D(i, j, 0, cols, 3)];
      const float g = src[IDX_3D(i, j, 1, cols, 3)];
      const float b = src[IDX_3D(i, j, 2, cols, 3)];
      const float max_val = MAX(r, MAX(g, b));
      const float min_val = MIN(r, MIN(g, b));
      const float delta = max_val - min_val;
      float v = max_val;
      float h;
      if (IS_ZERO(delta))
        h = 0;
      else if (F_EQUAL(max_val, r))
        h = (g - b) / delta;
      else if (F_EQUAL(max_val, g))
        h = (b - r) / delta + 2;
      else if (F_EQUAL(max_val, b))
        h = (r - g) / delta + 4;
      if (h < 0)
        h = 6 + fmod(h, 6);
      else if (h >= 6)
        h = fmod(h, 6);
      h /= 6;
      float s = IS_ZERO(v) ? 0 : delta / v;
      hc[IDX_2D(i, j, cols)] = h;
      sc[IDX_2D(i, j, cols)] = s;
      vc[IDX_2D(i, j, cols)] = v;
    }
  }
}

/**
 * HSV 转 RGB
 * @param src 输入HSI图像
 * @param rows 图像长
 * @param cols 图像宽
 * @param dst_cn 输出3通道
 */
inline void hsv2rgb(const float *src, int rows, int cols, float **dst_cn) {
  float *rc = dst_cn[0];
  float *gc = dst_cn[1];
  float *bc = dst_cn[2];
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      const float h = src[IDX_3D(i, j, 0, cols, 3)];
      const float s = src[IDX_3D(i, j, 1, cols, 3)];
      const float v = src[IDX_3D(i, j, 2, cols, 3)];
      const float h_ = 6 * h;
      const float x = v * (1 - s);
      const float y = v * (1 - s * (h_ - (int) h_));
      const float z = v * (1 - (1 - (h_ - (int) h_)) * s);
      float r, g, b;
      switch ((int) h_) {
        case 0:r = v;
          g = z;
          b = x;
          break;
        case 1: r = y;
          g = v;
          b = x;
          break;
        case 2: r = x;
          g = v;
          b = z;
          break;
        case 3: r = x;
          g = y;
          b = v;
          break;
        case 4: r = z;
          g = x;
          b = v;
          break;
        case 5: r = v;
          g = x;
          b = y;
          break;
        default:std::cerr << "ERROR" << std::endl;
      }
      rc[IDX_2D(i, j, cols)] = r;
      bc[IDX_2D(i, j, cols)] = b;
      gc[IDX_2D(i, j, cols)] = g;
    }
  }
}

/**
 * RGB 转 灰度图
 * @param src 输入RGB图像
 * @param rows 图像长
 * @param cols 图像宽
 * @param dst_cn 输出3通道
 */
inline void rgb2grey(const float *src, int rows, int cols, float *dst) {
  auto output_channels = alloc_2d<float>(3, rows * cols);
  rgb2hsi(src, rows, cols, output_channels);
  std::memcpy(dst, output_channels[2], rows * cols * sizeof(float));
  delete_2d(output_channels, 3);
}

}

#endif //IMPROCLAB_COLOR_CONVERT_H
