#include <iostream>
#include "imgproc.h"
#include "color_convert.h"
#include "histogram.h"
#include "imgio.h"
using namespace std;
using namespace lyz;

void test_histo_equa() {
  const int HISTO_LEN = 8;
  float src_regu[] = {0.19, 0.25, 0.21, 0.16, 0.08, 0.06, 0.03, 0.02};
  int dst_mapping[HISTO_LEN];
  float dst_regu[HISTO_LEN];
  histo_equa(src_regu, dst_mapping, dst_regu, HISTO_LEN);
  std::cout << "--------------- histo_equa() result ---------------" << std::endl;
  std::cout << "f->g =\t";
  print_mat(dst_mapping, HISTO_LEN);
  std::cout << "P(g) =\t";
  print_mat(dst_regu, HISTO_LEN);
}

void test_histo_spec() {
  const int HISTO_LEN = 8;
  float src_regu[] = {0.19, 0.25, 0.21, 0.16, 0.08, 0.06, 0.03, 0.02};
  float target_regu[] = {0, 0, 0, 0.15, 0.20, 0.30, 0.20, 0.15};
  int dst_mapping[HISTO_LEN];
  float dst_regu[HISTO_LEN];
  histo_spec(src_regu, target_regu, dst_mapping, dst_regu, HISTO_LEN);
  std::cout << "--------------- histo_spec() result ---------------" << std::endl;
  std::cout << "f->z =\t";
  print_mat(dst_mapping, HISTO_LEN);
  std::cout << "P(z) =\t";
  print_mat(dst_regu, HISTO_LEN);
}

void lab1() {
  unsigned char *img_data;
  int rows;
  int cols;
  int n_channels;
  imread("../ExperimentImage/彩色/bmp/baboon.bmp", img_data, rows, cols, n_channels, false);
  imshow("Original", img_data, rows, cols, n_channels);
  auto original_rgb_channels = alloc_2d<unsigned char>(3, rows * cols);
  split(img_data, original_rgb_channels, rows, cols);
  // 原始RGB三通道
  imshow("Original_R", original_rgb_channels[0], rows, cols, 1);
  imshow("Original_G", original_rgb_channels[1], rows, cols, 1);
  imshow("Original_B", original_rgb_channels[2], rows, cols, 1);
  // 图像正规化, 映射到0~1范围
  auto regularized_rgb = new float[rows * cols * n_channels];
  regularize(img_data, regularized_rgb, rows * cols * n_channels);
  // 分配内存准备输出
  auto recovered_img = new unsigned char[rows * cols]; // 存放每次恢复到0~255的单通道数据
  auto output_channels = alloc_2d<float>(3, rows * cols); // 存放每次处理完后的三个通道
  auto merged = new float[rows * cols * n_channels]; // 存放三通道合并后的数据

  // -------------------- RGB转灰度图 --------------------
  rgb2grey(regularized_rgb, rows, cols, *output_channels);
  regularize(*output_channels, recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("GREY", recovered_img, rows, cols, 1);
  // -------------------------------------------------

  // -------------------- RGB转YUV --------------------
  rgb2yuv(regularized_rgb, rows, cols, output_channels);
  regularize(output_channels[0], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("YUV_Y", recovered_img, rows, cols, 1);
  regularize(output_channels[1], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("YUV_U", recovered_img, rows, cols, 1);
  regularize(output_channels[2], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("YUV_V", recovered_img, rows, cols, 1);
  // -------------------------------------------------

  // -------------------- YUV转RGB --------------------
  merge(output_channels, merged, rows, cols);
  yuv2rgb(merged, rows, cols, output_channels);
  regularize(output_channels[0], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("YUV_R", recovered_img, rows, cols, 1);
  regularize(output_channels[1], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("YUV_G", recovered_img, rows, cols, 1);
  regularize(output_channels[2], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("YUV_B", recovered_img, rows, cols, 1);
  // -------------------------------------------------

  // -------------------- RGB转HSI --------------------
  rgb2hsi(regularized_rgb, rows, cols, output_channels);
  regularize(output_channels[0], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("HSI_H", recovered_img, rows, cols, 1);
  regularize(output_channels[1], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("HSI_S", recovered_img, rows, cols, 1);
  regularize(output_channels[2], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("HSI_I", recovered_img, rows, cols, 1);
  // -------------------------------------------------

  // -------------------- HSI转RGB --------------------
  merge(output_channels, merged, rows, cols);
  hsi2rgb(merged, rows, cols, output_channels);
  regularize(output_channels[0], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("HSI_R", recovered_img, rows, cols, 1);
  regularize(output_channels[1], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("HSI_G", recovered_img, rows, cols, 1);
  regularize(output_channels[2], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("HSI_B", recovered_img, rows, cols, 1);
  // -------------------------------------------------

  // -------------------- RGB转HSV --------------------
  rgb2hsv(regularized_rgb, rows, cols, output_channels);
  regularize(output_channels[0], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("HSV_H", recovered_img, rows, cols, 1);
  regularize(output_channels[1], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("HSV_S", recovered_img, rows, cols, 1);
  regularize(output_channels[2], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("HSV_V", recovered_img, rows, cols, 1);
  // -------------------------------------------------

  // -------------------- HSV转RGB --------------------
  merge(output_channels, merged, rows, cols);
  hsv2rgb(merged, rows, cols, output_channels);
  regularize(output_channels[0], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("HSV_R", recovered_img, rows, cols, 1);
  regularize(output_channels[1], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("HSV_G", recovered_img, rows, cols, 1);
  regularize(output_channels[2], recovered_img, rows * cols, (unsigned char) 0, (unsigned char) 255);
  imshow("HSV_B", recovered_img, rows, cols, 1);
  // -------------------------------------------------

  // 释放预分配内存
  delete_2d(output_channels, 3);
  delete_2d(original_rgb_channels, 3);
  delete[] regularized_rgb;
  delete[] merged;
  delete[] img_data;
}

void lab2() {
  unsigned char *img_data;
  constexpr int HISTO_LEN = 256;
  int rows;
  int cols;
  int n_channels;
  imread("../ExperimentImage/灰度/BMP/boats.bmp", img_data, rows, cols, n_channels, true);
  imshow("Original", img_data, rows, cols, n_channels);
  // 分配内存准备输出
  auto processed_img = new unsigned char[rows * cols]; // 存放每次恢复到0~255的单通道数据
  int orig_histo[HISTO_LEN]; // 原始直方图
  float orig_regu_histo[HISTO_LEN]; // 原始正规直方图
  int processed_histo[HISTO_LEN]; // 处理之后的直方图
  float processed_regu_histo[HISTO_LEN]; // 处理之后的正规直方图
  int trans_map[HISTO_LEN]; // 灰度映射函数

  histo(img_data, orig_histo, rows * cols);
  cout << "-------------------- Original --------------------" << endl;
  plot1d("Original Histogram", orig_histo, HISTO_LEN);
  scalar_mul(orig_histo, orig_regu_histo, 1.0 / (rows * cols), HISTO_LEN);

  // -------------------- 均衡化 --------------------
  histo_equa(orig_regu_histo, trans_map, processed_regu_histo, HISTO_LEN);
  scalar_mul(processed_regu_histo, processed_histo, rows * cols, HISTO_LEN);
  plot1d("Equalized Histogram", processed_histo, HISTO_LEN);
  plot1d("Equalization Mapping Function", trans_map, HISTO_LEN);
  apply_trans(img_data, processed_img, trans_map, rows * cols);
  imshow("Equalization_Result", processed_img, rows, cols, 1);
  // -----------------------------------------------

  // -------------------- 规定化 --------------------
  float target_regu_histo[HISTO_LEN]; // 目标正规直方图
  normal_seq(target_regu_histo, HISTO_LEN); // 随机生成一个成正态分布的曲线作为目标正规直方图
  int target_histo[HISTO_LEN]; // 存储目标直方图
  scalar_mul(target_regu_histo, target_histo, rows * cols, HISTO_LEN);
  plot1d("Target Histogram", target_histo, HISTO_LEN);
  // 直方图规定化
  histo_spec(orig_regu_histo, target_regu_histo, trans_map, processed_regu_histo, HISTO_LEN);
  scalar_mul(processed_regu_histo, processed_histo, rows * cols, HISTO_LEN);
  plot1d("Specified Histogram", processed_histo, HISTO_LEN);
  plot1d("Specification Mapping Function", trans_map, HISTO_LEN);
  apply_trans(img_data, processed_img, trans_map, rows * cols);
  imshow("Specification_Result", processed_img, rows, cols, 1);
  // -----------------------------------------------


  // 释放预分配内存
  delete[] processed_img;
  delete[] img_data;
}

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;

int test() {
  Mat src = imread("../ExperimentImage/灰度/BMP/boats.bmp", IMREAD_COLOR);
  if (src.empty()) {
    return -1;
  }
  vector<Mat> bgr_planes;
  split(src, bgr_planes);
  int histSize = 256;
  float range[] = {0, 256}; //the upper boundary is exclusive
  const float *histRange = {range};
  bool uniform = true, accumulate = false;
  Mat b_hist;
  calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
//  std::cout<<b_hist.at<int>(1);
  std::cout << b_hist.type();
  std::cout << b_hist << std::endl;
  int hist_w = 512, hist_h = 400;
  int bin_w = cvRound((double) hist_w / histSize);
  Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
  normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
  for (int i = 1; i < histSize; i++) {
    line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
         Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
         Scalar(255, 0, 0), 2, 8, 0);
  }
  imshow("Source image", src);
  imshow("calcHist Demo", histImage);
  waitKey();
  return 0;
}

int main() {
//  lab1();
//  waitKey(0);
//  test_histo_equa();
//  test_histo_spec();
//  lab1();
  lab2();
//test();
  return 0;
}