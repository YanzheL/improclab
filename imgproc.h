// =======================================================================
// Copyright (c) 2016-2020 by LI YANZHE. All rights reserved.
// Author: LI YANZHE <lee.yanzhe@yanzhe.org>
// Created by LI YANZHE on 4/15/19.
// =======================================================================
#ifndef IMPROCLAB__IMGPROC_H_
#define IMPROCLAB__IMGPROC_H_

#include <cmath>
#include <limits>
#include <assert.h>
#include "util.h"

namespace lyz {

/**
 * 矩阵乘法 a x b = c
 * 形状 (n1, n2) x (n2, n3) = (n1, n3)
 * @param n1
 * @param n2
 * @param n3
 */
template<typename T1, typename T2, typename T3>
inline void mat_mul(const T1 *a, T2 *b, T3 *c, int n1, int n2, int n3) {
  int i, j, k;
  for (i = 0; i < n1; ++i)
    for (j = 0; j < n3; ++j) {
      T3 tp = 0;
      for (k = 0; k < n2; ++k)
        tp += a[i * n1 + k] * b[k * n3 + j];
      c[i * n3 + j] = tp;
    }
}

template<typename T1, typename T2, typename T3>
inline void mat_mul(const T1 *a, T2 **b, T3 **c, int n1, int n2, int n3) {
  int i, j, k;
  for (i = 0; i < n1; ++i)
    for (j = 0; j < n3; ++j) {
      T3 tp = 0;
      for (k = 0; k < n2; ++k)
        tp += a[i * n1 + k] * b[k][j];
      c[i][j] = tp;
    }
}

/**
 * 两个矩阵对应元素相乘
 * @param a 输入A矩阵起始地址
 * @param b 输入B矩阵起始地址
 * @param c 输出B矩阵起始地址
 * @param rows 当前乘法窗口行数
 * @param cols 当前乘法窗口列数
 * @param w_a A矩阵在内存中线性表示的原始宽度
 * @param w_b B矩阵在内存中线性表示的原始宽度
 * @param w_c C矩阵在内存中线性表示的原始宽度
 */
template<typename T1, typename T2, typename T3>
inline
void mat_dotmul_scope(const T1 *a,
                      const T2 *b,
                      T3 *c,
                      int rows,
                      int cols,
                      int w_a,
                      int w_b,
                      int w_c) {
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      c[i * w_c + j] = a[i * w_a + j] * b[i * w_b + j];
}

template<typename T>
inline float sequence_sum(const T *src, int n) {
  float sum = 0;
  for (int i = 0; i < n; ++i)
    sum += src[i];
  return sum;
}

/**
 * 分离三个通道
 * @param src 输入彩色图像
 * @param dst 输出三个通道灰度图
 * @param rows 输入图像长
 * @param cols 输入图像宽
 */
template<typename T>
inline void split(const T *src, T **dst, int rows, int cols) {
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      for (int k = 0; k < 3; ++k)
        dst[k][IDX(i, j, cols)] = src[IDX(i, j, k, cols, 3)];
}

/**
 * 合并三通道
 * @param src 三个单独通道
 * @param dst 合并后的彩色图像
 * @param rows 输入图像长
 * @param cols 输入图像宽
 */
template<typename T>
inline void merge(T **src, T *dst, int rows, int cols) {
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      for (int k = 0; k < 3; ++k)
        dst[IDX(i, j, k, cols, 3)] = src[k][IDX(i, j, cols)];
}

/**
 * 将输入序列映射到指定的[range_min, range_max]范围
 * @param src 输入原始数组, 各元素需非负
 * @param dst 输出映射后的序列
 * @param n 序列长度
 */
template<typename T_IN, typename T_OUT>
inline void regularize(const T_IN *src, T_OUT *dst, int n, T_OUT range_min = 0.0, T_OUT range_max = 1.0) {
  T_IN min = std::numeric_limits<T_IN>::max();
  T_IN max = 0;
  min_max(src, n, &min, &max);
  const float factor = max - min;
  for (int i = 0; i < n; ++i)
    dst[i] = ((src[i] - min) / factor) * range_max + range_min;
}

//template<typename T>
//inline void regularize(const T *src, float *dst, int cols, int channels) {
//  for (int i = 0; i < channels; ++i) {
//    T min = std::numeric_limits<T>::max();
//    T max = 0;
//    for (int j = 0; j < cols * channels; j += channels) {
//      T val = src[j + i];
//      min = val <= min ? val : min;
//      max = val > max ? val : max;
//    }
//    float factor = max - min;
//    for (int j = 0; j < cols * channels; j += channels)
//      dst[j + i] = (src[j + i] - min) / factor;
//  }
//}

/**
 * 给出长度为n的正态分布序列
 * @param dst 输出数组
 * @param n 长度
 * @param stdev_factor 标准差分母
 * @param ntests 随即样本数, 样本越多, 输出的曲线越接近标准正态分布
 */
inline void normal_seq(float *dst, int n, float stdev_factor = 5.0, int ntests = 1000) {
  static std::default_random_engine generator;
  static std::normal_distribution<float> distribution(n / 2.0, n / stdev_factor);
//  std::uniform_int_distribution<int> distribution(0, n);
  long *dist = new long[n]();
  for (int i = 0; i < ntests; ++i) {
    int val = distribution(generator);
    if (val >= 0 && val < n)
      ++dist[(int) val];
  }
  scalar_mul(dist, dst, 1.0 / ntests, n);
  delete[] dist;
}

}

#endif //IMPROCLAB__IMGPROC_H_
