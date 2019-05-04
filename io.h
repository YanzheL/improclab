// =======================================================================
// Copyright (c) 2016-2020 by LI YANZHE. All rights reserved.
// Author: LI YANZHE <lee.yanzhe@yanzhe.org>
// Created by LI YANZHE on 5/1/19.
// =======================================================================
#ifndef IMPROCLAB_IO_H
#define IMPROCLAB_IO_H

#include <string>
namespace lyz {

/**
 * 读取图像
 * @param path 文件路径
 * @param dst 输出图像内容
 * @param rows 输出图像长
 * @param cols 输出图像宽
 */
void imread(const std::string path, unsigned char *&dst, int &rows, int &cols, int &channels, bool is_grey);

/**
 * 显示图片
 * @param name 窗口名称
 * @param src 源图像
 * @param rows 图像长
 * @param cols 图像宽
 */
void imshow(const std::string name, const unsigned char *src, int rows, int cols, unsigned int channels);

/**
 * 把图片写入文件
 * @param path
 * @param src
 * @param rows
 * @param cols
 * @param channels
 */
void imwrite(const std::string path, const unsigned char *src, int rows, int cols, int channels);

/**
 * 把一维数组作为直方图显示
 * @param name 窗口名称
 * @param src 输入数组
 * @param n 数组长度
 */
void plot1d(const std::string name, const int *src, int n);

}

#endif //IMPROCLAB_IO_H
