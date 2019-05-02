// =======================================================================
// Copyright (c) 2016-2020 by LI YANZHE. All rights reserved.
// Author: LI YANZHE <lee.yanzhe@yanzhe.org>
// Created by LI YANZHE on 4/115/19.
// =======================================================================
#ifndef IMPROCLAB_HISTOGRAM_H
#define IMPROCLAB_HISTOGRAM_H

#include "util.h"

namespace lyz {

/**
 * 灰度级映射, 计算G = floor{(G_max - G_min) * C(f) + G_min + 0.5}
 * @param src_regu 输入正规数组
 * @param dst_mapping 输出灰度级映射, len = n
 * @param n 直方图长度
 */
inline void histo_mapping(const float *src_regu, int *dst_mapping, int n) {
  float p = 0;
  for (int i = 0; i < n; ++i) {
    p += src_regu[i];
    dst_mapping[i] = ROUND((n - 1) * p); // (int) ((n-1) * p + 0.5)
  }
}

/**
 * 将原序列按给定的映射，转换到新的序列
 * @param src 输入序列
 * @param mapping 映射表
 * @param dst 输出序列
 * @param n 输入序列长度
 */
inline void value_mapping(const float *src, float *dst, const int *mapping, const int n) {
  zero_fill(dst, n);
  for (int i = 0; i < n; ++i)
    dst[mapping[i]] += src[i];
}

/**
 * 计算给定图像的直方图
 * @param src 原始二维图像矩阵
 * @param dst 输出的直方图, len = 8
 * @param n 图像总像素个数
 */
inline void histo(const unsigned char *src, int *dst, int n, int hlen = 256) {
  zero_fill(dst, hlen);
  for (int i = 0; i < n; ++i) {
    unsigned val = src[i];
    ++dst[val];
  }
}

/**
 * 对输入图像应用灰度级映射
 * @param src 输入图像
 * @param dst 输出图像
 * @param mapping 灰度级映射
 * @param n 总像素数
 */
inline void apply_trans(const unsigned char *src, unsigned char *dst, const int *mapping, size_t n) {
  for (int i = 0; i < n; ++i)
    dst[i] = mapping[src[i]];
}

/**
 * 直方图均衡化
 * @param src_histo_regu 输入正规直方图
 * @param dst_mapping 输出灰度级映射, len = n
 * @param dst_histo_regu 输出均衡化后的正规直方图, len = n
 * @param n 直方图长度
 */
inline void histo_equa(const float *src_histo_regu, int *dst_mapping, float *dst_histo_regu, size_t n) {
  histo_mapping(src_histo_regu, dst_mapping, n); // 计算f->g
  value_mapping(src_histo_regu, dst_histo_regu, dst_mapping, n); // 将P(f)概率序列根据映射 f->g 映射到结果概率序列P(g)
}

/**
 * 直方图规定化
 * @param src_histo_regu 输入正规直方图
 * @param target_histo_regu 目标正规直方图
 * @param dst_mapping 输出灰度级映射, len = n
 * @param dst_histo_regu 输出规定化后的正规直方图, len = n
 * @param n 直方图长度
 */
inline void histo_spec(const float *src_histo_regu,
                       const float *target_histo_regu,
                       int *dst_mapping,
                       float *dst_histo_regu,
                       const size_t n) {
//  int *msrc = new int[n]; // 存储 f->g 映射
  int msrc[n]; // 存储 f->g 映射
  int mtarget[n]; // 存储 z->y 映射
  histo_mapping(src_histo_regu, msrc, n); // 计算f->g
  histo_mapping(target_histo_regu, mtarget, n); // 计算z->y
  for (int i = 0; i < n; ++i) {
    /**
     * msrc[i] = 原直方图均衡化映射 f->g 的象 gi
     * mtarget[k] = 目标直方图均衡化映射 z->y 的象 yk
     * argmin_abs 找出使|gi - yk|最小的mtarget元素的索引值, 也就是 z->y 映射的原象 z
     * dst_mapping[i] = f -> z 映射的象 z
     */
    dst_mapping[i] = argmin_abs(msrc[i], mtarget, n);
  }
  // 将P(f)概率序列根据映射 f->z 映射到结果概率序列P(z)
  value_mapping(src_histo_regu, dst_histo_regu, dst_mapping, n);
//  delete[] msrc;
//  delete[] mtarget;
}

}

#endif //IMPROCLAB_HISTOGRAM_H
