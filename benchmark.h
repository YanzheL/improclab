// =======================================================================
// Copyright (c) 2016-2020 by LI YANZHE. All rights reserved.
// Author: LI YANZHE <lee.yanzhe@yanzhe.org>
// Created by LI YANZHE on 5/4/19.
// =======================================================================
#ifndef IMPROCLAB_BENCHMARK_H
#define IMPROCLAB_BENCHMARK_H

#include <tuple>
#include <type_traits>
#include <chrono>

#define GETTIME_HIGH(X, MSG)                                                      \
{                                                                                 \
  auto t_start = std::chrono::high_resolution_clock::now();                       \
  { X }                                                                           \
  auto t_end = std::chrono::high_resolution_clock::now();                         \
  double dur = std::chrono::duration<double, std::nano>(t_end - t_start).count(); \
  long s = (long) (dur / 1e9);                                                    \
  long ms = (long) ((dur / 1e6) - s * 1e3);                                       \
  long us = (long) ((dur / 1e3) - s * 1e6 - ms * 1e3);                            \
  long ns = ((long) dur) % 1000;                                                  \
  std::cout << #MSG                                                               \
  << " time = "                                                                   \
  << s << " s "                                                                   \
  << ms << " ms "                                                                 \
  << us << " us "                                                                 \
  << ns << " ns"                                                                  \
  << std::endl;                                                                   \
}

#endif //IMPROCLAB_BENCHMARK_H
