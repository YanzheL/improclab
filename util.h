// =======================================================================
// Copyright (c) 2016-2020 by LI YANZHE. All rights reserved.
// Author: LI YANZHE <lee.yanzhe@yanzhe.org>
// Created by LI YANZHE on 5/1/19.
// =======================================================================
#ifndef IMPROCLAB_UTIL_H
#define IMPROCLAB_UTIL_H

#include <random>
#include <initializer_list>

//#define IDX(x, y, z, WIDTH, DEPTH) ((x) + WIDTH * ((y) + DEPTH * (z)))
//#define IDX_2D(i, j, WIDTH) ((i) * WIDTH + (j))
//#define IDX_3D(i, j, k, WIDTH, DEPTH) (IDX(i, j, WIDTH) * DEPTH + k)
#define MIN(x, y) ((x) <= (y) ? (x) : (y))
#define MAX(x, y) ((x) >= (y) ? (x) : (y))
#define IS_ZERO(x) (std::fabs(x) <= std::numeric_limits<float>::epsilon())
#define F_EQUAL(x, y) IS_ZERO((x)-(y))
#define ROUND_TO_ZERO(x) (IS_ZERO(x) ? 0 : (x))
#define PI (3.1415925635)
//#define ROUND(x) (int) ((x) + 0.5)
#define ROUND(x) (std::lround(x))

namespace lyz {

/**
 * 用0填充目标序列
 * @param dst 目标序列
 * @param n 序列长度
 */
template<typename T>
inline void zero_fill(T *dst, int n) { for (int i = 0; i < n; ++i) dst[i] = 0; }

/**
 * 获取数组的最大值和最小值
 * @param src
 * @param n
 * @param min 输出最小值, 需要事先赋初值
 * @param max 输出最大值, 需要事先赋初值
 */
template<typename T>
inline void min_max(const T *src, int n, T *min, T *max) {
  for (int i = 0; i < n; ++i) {
    T val = src[i];
    *min = val <= *min ? val : *min;
    *max = val > *max ? val : *max;
  }
}

/**
 * 计算src数组中元素值与x的绝对值最小的元素的索引值, 如果有多个匹配, 取最小索引
 * @tparam T
 * @param x 待比较元素
 * @param src 数组
 * @param n 数组长度
 * @return 最小匹配索引
 */
template<typename T>
inline int argmin_abs(T x, const T *src, int n) {
  T min = std::numeric_limits<T>::max();
  int idx = 0;
  for (int i = 0; i < n; ++i) {
    T abs = std::fabs(src[i] - x);
    if (abs < min) {
      min = abs;
      idx = i;
    }
  }
  return idx;
}

template<typename T1, typename T2>
inline void scalar_mul(const T1 *src, T2 *dst, double factor, int n) {
  for (int i = 0; i < n; ++i)
    dst[i] = src[i] * factor;
}

template<typename T>
inline T **alloc_2d(int m, int n) {
  T **mat = new T *[m];
  for (int i = 0; i < m; ++i)
    mat[i] = new T[n];
  return mat;
}

template<typename T>
inline void delete_2d(T **mat, int m) {
  for (int i = 0; i < m; ++i)
    delete[] mat[i];
  delete[] mat;
}

template<typename T>
inline void print_mat(const T *src, int n) {
  for (int i = 0; i < n; ++i)
    std::cout << src[i] << "\t";
  std::cout << std::endl;
}

inline constexpr int nd_idx(std::initializer_list<int> shape, std::initializer_list<int> indices) {
  int res = 0;
  const auto sbegin = shape.begin();
  const auto ibegin = indices.begin();
  for (int dim = 0; dim < shape.size(); ++dim)
    res += res * sbegin[dim] + ibegin[dim];
  return res;
}

/**
 * 多维索引转一维索引
 * 使用方法:
 * 对于2维mxn矩阵, 访问i,j 元素, 使用 IDX(i,j,n)
 * 对于n维形状为(s1,s2,...,sn)的矩阵, 访问第(e1,e2,...,en)位置的元素, 使用IDX(e1,e2,...,en,s2,s3,...,sn)
 * @param params
 * @return 一维索引
 */
template<typename ...Args>
inline constexpr size_t IDX(const Args... params) {
  constexpr size_t NDIMS = sizeof...(params) / 2 + 1;
  std::initializer_list<int> args{params...};
  auto ibegin = args.begin();
  auto sbegin = ibegin + NDIMS;
  size_t res = 0;
  for (int dim = 0; dim < NDIMS; ++dim) {
    size_t factor = dim > 0 ? sbegin[dim - 1] : 0;
    res = res * factor + ibegin[dim];
  }
  return res;
}

}

#endif //IMPROCLAB_UTIL_H
