// =======================================================================
// Copyright (c) 2016-2020 by LI YANZHE. All rights reserved.
// Author: LI YANZHE <lee.yanzhe@yanzhe.org>
// Created by LI YANZHE on 5/1/19.
// =======================================================================
#ifndef IMPROCLAB_IMGIO_H
#define IMPROCLAB_IMGIO_H

#include <string>
namespace lyz {

/**
 * 读取RAW图像
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

void imwrite(const std::string path, const unsigned char *src, int rows, int cols, int channels);

void plot1d(const std::string name, const int *histo, int hlen);

}

#endif //IMPROCLAB_IMGIO_H
