#pragma once

#include <opencv2/opencv.hpp>

// 图像亮度调节
// @param in：输入图像
// @param out：输出图像
// @param factor：调整因子
void imgBrightnessChange(cv::Mat in, cv::Mat &out, int factor);

// 图像对比度调节
// @param in：输入图像
// @param out：输出图像
// @param factor：调整因子
void imgContrastChange(cv::Mat in, cv::Mat &out, double factor);

// 图像缩放
// @param in：输入图像
// @param out：输出图像
// @param factor：调整因子
void imgResize(cv::Mat in, cv::Mat &out, double factor);

// 图像旋转
// @param in：输入图像
// @param out：输出图像
// @param angle：旋转角度子
void imgRotate(cv::Mat in, cv::Mat &out, double angle);

// 图像水平翻转
// @param in：输入图像
// @param out：输出图像
void imgHorizontalFlip(cv::Mat in, cv::Mat &out);

// 图像垂直翻转
// @param in：输入图像
// @param out：输出图像
void imgVerticalFlip(cv::Mat in, cv::Mat &out);

// 图像反色
// @param in：输入图像
// @param out：输出图像
void imgInversion(cv::Mat in, cv::Mat &out);

// 图像灰度化
// @param in：输入图像
// @param out：输出图像
// @param type：是否以单通道形式输出
void imgGrayscale(cv::Mat in, cv::Mat &out, bool type);

// 图像统计直方图
// @param in：输入图像
// @param out：输出图像
void drawHistogram(cv::Mat in, cv::Mat &out);

// 图像直方图均衡化
// @param in：输入图像
// @param out：输出图像
// @param hist：均衡化后的直方图
void imgHistEqualization(cv::Mat in, cv::Mat &out, cv::Mat &hist);

// 图像算术平均滤波
// @param in：输入图像
// @param out：输出图像
// @param ksize：核大小
void imgArithmeticAverageFiltering(cv::Mat in, cv::Mat &out, int ksize);

// 图像几何平均滤波
// @param in：输入图像
// @param out：输出图像
// @param ksize：核大小
void imgGeometricAverageFiltering(cv::Mat in, cv::Mat &out, int ksize);

// 图像谐波均值滤波
// @param in：输入图像
// @param out：输出图像
// @param ksize：核大小
void imgHarmonicMeanFiltering(cv::Mat in, cv::Mat &out, int ksize);

// 图像中值滤波
// @param in：输入图像
// @param out：输出图像
// @param ksize：核大小
void imgMedianFiltering(cv::Mat in, cv::Mat &out, int ksize);

// 图像最大最小值滤波
// @param in：输入图像
// @param out：输出图像
// @param ksize：核大小
// @param type：滤波类型，最大值还是最小值
void imgMaxMinFiltering(cv::Mat in, cv::Mat &out, int ksize, int type);

// 图像拉普拉斯锐化
// @param in：输入图像
// @param out：输出图像
// @param ksize：核大小
// @param type：滤波类型，四邻域还是八邻域
void imgLaplaceFiltering(cv::Mat in, cv::Mat &out, int type);

// 图像膨胀腐蚀
// @param in：输入图像
// @param out：输出图像
// @param kernelSize：核大小
// @param kernelType：核类型
// @param op：操作类型
// @param cnt：操作次数
void imgDilationOrErosion(cv::Mat in, cv::Mat &out, int kernelSize, int kernelType, int op, int cnt);

// 图像开闭运算
// @param in：输入图像
// @param out：输出图像
// @param kernelSize：核大小
// @param kernelType：核类型
// @param op：操作类型
// @param cnt：操作次数
void imgOpeningOrClosing(cv::Mat in, cv::Mat &out, int kernelSize, int kernelType, int op, int cnt);

// 图像顶帽变换底帽变换
// @param in：输入图像
// @param out：输出图像
// @param kernelSize：核大小
// @param kernelType：核类型
// @param op：操作类型
// @param cnt：操作次数
void imgTopOrBottomHatTrans(cv::Mat in, cv::Mat &out, int kernelSize, int kernelType, int op, int cnt);

// 图像给定阈值分割
// @param in：输入图像
// @param out：输出图像
// @param T：给定阈值
void imgThresholdSegment(cv::Mat in, cv::Mat &out, int T);

// 图像自动全局阈值分割
// @param in：输入图像
// @param out：输出图像
// @param T：自动全局求出的阈值返回
void imgIterativeThresholdSegment(cv::Mat in, cv::Mat &out, int &T);

// 图像OTSU阈值分割
// @param in：输入图像
// @param out：输出图像
// @param T：OTSU求出的阈值返回
void imgOTSUThresholdSegment(cv::Mat in, cv::Mat &out, int &T);

// 图像区域生长
// @param in：输入图像
// @param out：输出图像
// @param T：区域生长阈值，像素在某范围内生长
void imgRegionGrowth(cv::Mat in, cv::Mat &out, int T);

// 图像Sobel算子卷积
// @param in：输入图像
// @param out：输出图像
void sobel(cv::Mat in, cv::Mat &out);

// 图像Roberts算子卷积
// @param in：输入图像
// @param out：输出图像
void roberts(cv::Mat in, cv::Mat &out);

// 图像Prewitt算子卷积
// @param in：输入图像
// @param out：输出图像
void prewitt(cv::Mat in, cv::Mat &out);

// 图像Kirsch算子卷积
// @param in：输入图像
// @param out：输出图像
void kirsch(cv::Mat in, cv::Mat &out);

// 图像高斯拉普拉斯算子卷积
// @param in：输入图像
// @param out：输出图像
void LOG(cv::Mat in, cv::Mat &out);

// 图像点检测（拉普拉斯卷积）
// @param in：输入图像
// @param out：输出图像
// @param type：四邻域或八邻域
void imgPointDetect(cv::Mat in, cv::Mat &out, int type);

// 图像线检测
// @param in：输入图像
// @param out：输出图像
void imgLineDetect(cv::Mat in, cv::Mat &out);