// =======================================================================
// Copyright (c) 2016-2020 by LI YANZHE. All rights reserved.
// Author: LI YANZHE <lee.yanzhe@yanzhe.org>
// Created by LI YANZHE on 5/4/19.
// =======================================================================
#ifndef IMPROCLAB_FILTER_H
#define IMPROCLAB_FILTER_H

#include "util.h"
#include "histogram.h"
#include "imgproc.h"
#include <algorithm>
#include <assert.h>

namespace lyz {

/**
 * 从src矩阵中按照给定的核提取出数据存入dst
 * @param src 源矩阵
 * @param dst 输出矩阵
 * @param cols 源矩阵列数
 * @param k_rows 核行数
 * @param k_cols 核列数
 * @param r 当前核左上角在源矩阵中的行坐标
 * @param c 当前核左上角在源矩阵中的列坐标
 */
template<typename T>
inline void extract_kernel(const T *src, T *dst, int cols, int k_rows, int k_cols, int r, int c) {
  for (int i = 0; i < k_rows; ++i)
    for (int j = 0; j < k_cols; ++j)
      dst[IDX(i, j, k_cols)] = src[IDX(r + i, c + j, cols)];
}

/**
 * 普通排序方法求中位数
 */
template<typename T>
inline T median_slow(const T *src, int n) {
  std::vector<T> vec(src, src + n);
  std::sort(vec.begin(), vec.end());
  return vec[n / 2 + 1];
}

/**
 * 普通排序方法求平均值
 */
template<typename T>
inline T mean_slow(const T *src, int n) {
  double sum = 0;
  for (int i = 0; i < n; ++i) {
    sum += src[i];
  }
  return sum / n;
}

//inline int sum_histo(const int *histo) {
//  int sum = 0;
//  for (int i = 0; i < 256; ++i)
//    sum += histo[i];
//  return sum;
//}

/**
 * 根据直方图求中位数
 * @param histo 当前核对应的直方图
 * @param k_size 核尺寸
 * @param hlen 直方图长度, 默认256
 * @return 中位数
 */
inline int median_fast(const int *histo, const int k_size, int hlen = 256) {
  const int stop = k_size * k_size / 2 + 1;
  int sum = 0;
  int i;
  for (i = 0; i < hlen; ++i) {
    sum += histo[i];
    if (sum > stop)
      break;
  }
  return i;
}

/**
 * 普通中值滤波
 * @param src 输入图像
 * @param dst 输出图像
 * @param rows 图像列数
 * @param cols 图像行数
 * @param k_size 核尺寸
 */
template<typename T>
inline void median_filter_slow(const T *src, T *dst, int rows, int cols, int k_size) {
  const int end_r = rows - k_size;
  const int end_c = cols - k_size;
  const int center_offset = std::ceil(k_size / 2.0) - 1;
  auto kernel = new T[k_size * k_size];
  for (int i = 0; i <= end_r; ++i) {
    for (int j = 0; j <= end_c; ++j) {
      extract_kernel(src, kernel, cols, k_size, k_size, i, j);
      dst[IDX(i + center_offset, j + center_offset, cols)] = median_slow(kernel, k_size * k_size);
    }
  }
  delete[] kernel;
}

/**
 * 快速中值滤波
 * @param src 输入图像
 * @param dst 输出图像
 * @param rows 图像列数
 * @param cols 图像行数
 * @param k_size 核尺寸
 */
inline void median_filter_fast(const unsigned char *src, unsigned char *dst, int rows, int cols, int k_size) {
  const int end_r = rows - k_size;
  const int end_c = cols - k_size;
  const int center_offset = std::ceil(k_size / 2.0) - 1;
  int kern_hist[256] = {0};
  // 初始化核直方图
  for (int i = 0; i < k_size; ++i)
    for (int j = 0; j < k_size; ++j)
      ++kern_hist[src[IDX(i, j, cols)]];
  int i = 0, j = 0;
  bool is_horizon = true; // true代表下一步应该横向走
  bool is_right = true; // true代表下一步应该向右走
  int ct = 0; // 已经处理过的像素数
  do {
    dst[IDX(i + center_offset, j + center_offset, cols)] = median_fast(kern_hist, k_size);
    if (is_horizon) {
      for (int k = 0; k < k_size; ++k) {
        int left_idx = IDX(i + k, j, cols); // 代表当前核最左列的索引
        int right_idx = IDX(i + k, j + k_size - 1, cols); // 代表当前核最右列的索引
        if (is_right) {
          --kern_hist[src[left_idx]]; // 把最左列的像素在直方图中的数据删除
          ++kern_hist[src[right_idx + 1]]; // 把最右列的下一列像素在直方图中的数据加1
        } else {
          --kern_hist[src[right_idx]]; // 把最右列的像素在直方图中的数据删除
          ++kern_hist[src[left_idx - 1]]; // 把最左列的下一列像素在直方图中的数据加1
        }
      }
      j = is_right ? j + 1 : j - 1;
      // 如果横向走到头, 则设置下一次应该更新行数(往下移动一行, 列位置不变)
      if ((is_right && j == end_c) || (!is_right && j == 0))
        is_horizon = false;
    } else {
      for (int k = 0; k < k_size; ++k) {
        --kern_hist[src[IDX(i, j + k, cols)]]; // 把核的第一行像素在直方图中的数据删除
        ++kern_hist[src[IDX(i + k_size, j + k, cols)]]; // 把核移动后的下一行像素在直方图中的数据加1
      }
      ++i;
      is_horizon = true; // 换行完成, 下一步继续横着走
      is_right = !is_right; // 每一次换行都代表上一步到了最左边或者最右边, 下一步左右方向应该调换
    }
    ++ct;
  } while (ct != (end_c + 1) * (end_r + 1));
}

/**
 * 普通均值滤波
 * @param src 输入图像
 * @param dst 输出图像
 * @param rows 图像列数
 * @param cols 图像行数
 * @param k_size 核尺寸
 */
template<typename T>
inline void mean_filter_slow(const T *src, T *dst, int rows, int cols, int k_size) {
  const int end_r = rows - k_size;
  const int end_c = cols - k_size;
  const int center_offset = std::ceil(k_size / 2.0) - 1;
  auto kernel = new T[k_size * k_size];
  for (int i = 0; i <= end_r; ++i) {
    for (int j = 0; j <= end_c; ++j) {
      extract_kernel(src, kernel, cols, k_size, k_size, i, j);
      dst[IDX(i + center_offset, j + center_offset, cols)] = mean_slow(kernel, k_size * k_size);
    }
  }
  delete[] kernel;
}

/**
 * 快速均值滤波
 * @param src 输入图像
 * @param dst 输出图像
 * @param rows 图像列数
 * @param cols 图像行数
 * @param k_size 核尺寸
 */
template<typename T>
inline void mean_filter_fast(const T *src, T *dst, int rows, int cols, int k_size) {
  const int end_r = rows - k_size;
  const int end_c = cols - k_size;
  const int center_offset = std::ceil(k_size / 2.0) - 1;
  int sum = 0;
  for (int i = 0; i < k_size; ++i)
    for (int j = 0; j < k_size; ++j)
      sum += src[IDX(i, j, cols)];
  int i = 0, j = 0;
  bool is_horizon = true; // true代表下一步应该横向走
  bool is_right = true; // true代表下一步应该向右走
  int ct = 0; // 已经处理过的像素数
  do {
    dst[IDX(i + center_offset, j + center_offset, cols)] = sum / (k_size * k_size);
    if (is_horizon) {
      for (int k = 0; k < k_size; ++k) {
        int left_idx = IDX(i + k, j, cols); // 代表当前核最左列的索引
        int right_idx = IDX(i + k, j + k_size - 1, cols); // 代表当前核最右列的索引
        if (is_right) {
          sum -= src[left_idx];
          sum += src[right_idx + 1];
        } else {
          sum -= src[right_idx];
          sum += src[left_idx - 1];
        }
      }
      j = is_right ? j + 1 : j - 1;
      // 如果横向走到头, 则设置下一次应该更新行数(往下移动一行, 列位置不变)
      if ((is_right && j == end_c) || (!is_right && j == 0))
        is_horizon = false;
    } else {
      for (int k = 0; k < k_size; ++k) {
        sum -= src[IDX(i, j + k, cols)];
        sum += src[IDX(i + k_size, j + k, cols)];
      }
      ++i;
      is_horizon = true; // 换行完成, 下一步继续横着走
      is_right = !is_right; // 每一次换行都代表上一步到了最左边或者最右边, 下一步左右方向应该调换
    }
    ++ct;
  } while (ct != (end_c + 1) * (end_r + 1));
}

/**
 * 通用的梯度滤波函数
 * @param H1 梯度3x3算子H1
 * @param H2 梯度3x3算子H2
 * @param src 输入图像
 * @param dst 输出图像
 * @param rows 图像行数
 * @param cols 图像列数
 * @param k_size 梯度算子尺寸
 */
inline void gradient_filter(const int *H1,
                            const int *H2,
                            const float *src,
                            float *dst,
                            int rows,
                            int cols,
                            int k_size) {
  auto K = new float[k_size * k_size];
  const int center_offset = std::ceil(k_size / 2.0) - 1;
  for (int i = 0; i < rows - k_size; ++i) {
    for (int j = 0; j < cols - k_size; ++j) {
      float df = 0;
      mat_dotmul_scope(H1, src + IDX(i, j, cols), K, k_size, k_size, k_size, cols, k_size); // K = H1 * src
      df = std::abs(sequence_sum(K, k_size * k_size)); // df = |K|
      mat_dotmul_scope(H2, src + IDX(i, j, cols), K, k_size, k_size, k_size, cols, k_size); // K = H2 * src
      df += std::abs(sequence_sum(K, k_size * k_size));
      dst[IDX(i + center_offset, j + center_offset, cols)] = df;
    }
  }
  delete[] K;
}

/**
 * Sobel 梯度滤波
 */
inline void sobel_filter(const float *src, float *dst, int rows, int cols) {
  constexpr int H1[] = {
      -1, 0, 1,
      -2, 0, 2,
      -1, 0, 1
  };
  constexpr int H2[] = {
      -1, -2, -1,
      0, 0, 0,
      1, 2, 1
  };
  gradient_filter(H1, H2, src, dst, rows, cols, 3);
}

/**
 * Roberts 梯度滤波
 */
inline void roberts_filter(const float *src, float *dst, int rows, int cols) {
  constexpr int H1[] = {
      0, 0, 0,
      0, -1, 0,
      0, 0, 1
  };
  constexpr int H2[] = {
      0, 0, 0,
      0, 0, -1,
      0, 1, 0
  };
  gradient_filter(H1, H2, src, dst, rows, cols, 3);
}

/**
 * Prewitt 梯度滤波
 */
inline void prewitt_filter(const float *src, float *dst, int rows, int cols) {
  constexpr int H1[] = {
      -1, -1, -1,
      0, 0, 0,
      1, 1, 1
  };
  constexpr int H2[] = {
      -1, 0, 1,
      -1, 0, 1,
      -1, 0, 1
  };
  gradient_filter(H1, H2, src, dst, rows, cols, 3);
}

}

#endif //IMPROCLAB_FILTER_H
