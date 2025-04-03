#pragma once

#include <opencv2/opencv.hpp>

namespace ImgTools {
/*
 * @brief 图像亮度调节
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param iFactor 调整因子
 * @return void
 */
void ChangeBrightness(const cv::Mat &iMat, cv::Mat &oMat, int iFactor);

/*
 * @brief 图像对比度调节
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param idFactor 调整因子
 * @return void
 */
void ChangeContrast(const cv::Mat &iMat, cv::Mat &oMat, double idFactor);

/*
 * @brief 图像缩放
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param idFactor 调整因子
 * @return void
 */
void Resize(const cv::Mat &iMat, cv::Mat &oMat, double idFactor);

/*
 * @brief 图像旋转
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param idAngle 旋转角度
 * @return void
 */
void Rotate(const cv::Mat &iMat, cv::Mat &oMat, double idAngle);

/*
 * @brief 图像水平翻转
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @return void
 */
void HorizontalFlip(const cv::Mat &iMat, cv::Mat &oMat);

/*
 * @brief 图像垂直翻转
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @return void
 */
void VerticalFlip(const cv::Mat &iMat, cv::Mat &oMat);

/*
 * @brief 图像反色
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @return void
 */
void Inversion(const cv::Mat &iMat, cv::Mat &oMat);

/*
 * @brief 图像灰度化
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param ibType 是否以单通道形式输出
 * @return void
 */
void Grayscale(const cv::Mat &iMat, cv::Mat &oMat, bool ibType);

/*
 * @brief 图像统计直方图
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @return void
 */
void DrawHistogram(const cv::Mat &iMat, cv::Mat &oMat);

/*
 * @brief 图像直方图均衡化
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param oMatHist 均衡化后的直方图
 * @return void
 */
void HistEqualization(const cv::Mat &iMat, cv::Mat &oMat, cv::Mat &oMatHist);

/*
 * @brief 图像算术平均滤波
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param iKSize 核大小
 * @return void
 */
void ArithmeticAverageFilter(const cv::Mat &iMat, cv::Mat &oMat, int iKSize);

/*
 * @brief 图像几何平均滤波
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param iKSize 核大小
 * @return void
 */
void GeometricAverageFilter(const cv::Mat &iMat, cv::Mat &oMat, int iKSize);

/*
 * @brief 图像谐波均值滤波
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param iKSize 核大小
 * @return void
 */
void HarmonicMeanFilter(const cv::Mat &iMat, cv::Mat &oMat, int iKSize);

/*
 * @brief 图像中值滤波
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param iKSize 核大小
 * @return void
 */
void MedianFilter(const cv::Mat &iMat, cv::Mat &oMat, int iKSize);

/*
 * @brief 图像最大最小值滤波
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param iKSize 核大小
 * @param iType 滤波类型，最大值(1)还是最小值(0)
 * @return void
 */
void MaxMinFilter(const cv::Mat &iMat, cv::Mat &oMat, int iKSize, int iType);

/*
 * @brief 图像拉普拉斯锐化
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param iKSize 核大小
 * @param iType 滤波类型，四邻域还是八邻域
 * @return void
 */
void LaplaceFilter(const cv::Mat &iMat, cv::Mat &oMat, int iType);

/*
 * @brief 图像膨胀腐蚀
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param iKSize 核大小
 * @param iKType 核类型, 0: 方形; 1: 十字; 2: 椭圆
 * @param iOperation 操作类型, 0: 膨胀; 1: 腐蚀
 * @param iCnt 操作次数
 * @return void
 */
void DilationOrErosion(const cv::Mat &iMat, cv::Mat &oMat, int iKSize, int iKType, int iOperation, int iCnt);

/*
 * @brief 图像开闭运算
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param iKSize 核大小
 * @param iKType 核类型, 0: 方形; 1: 十字; 2: 椭圆
 * @param iOperation 操作类型, 见MorphTypes
 * @param iCnt 操作次数
 * @return void
 */
void OpeningOrClosing(const cv::Mat &iMat, cv::Mat &oMat, int iKSize, int iKType, int iOperation, int iCnt);

/*
 * @brief 图像顶帽变换底帽变换
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param iKSize 核大小
 * @param iKType 核类型, 0: 方形; 1: 十字; 2: 椭圆
 * @param iOperation 操作类型, 见MorphTypes
 * @param iCnt 操作次数
 * @return void
 */
void TopOrBottomHatTrans(const cv::Mat &iMat, cv::Mat &oMat, int iKSize, int iKType, int iOperation, int iCnt);

/*
 * @brief 图像给定阈值分割
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param iValue 给定阈值
 * @return void
 */
void ThresholdSegment(const cv::Mat &iMat, cv::Mat &oMat, int iValue);

/*
 * @brief 图像自动全局阈值分割
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param oValue 自动全局求出的阈值返回
 * @return void
 */
void IterativeThresholdSegment(const cv::Mat &iMat, cv::Mat &oMat, int &oValue);

/*
 * @brief 图像OTSU阈值分割
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param oValue OTSU求出的阈值返回
 */
void OTSUThresholdSegment(const cv::Mat &iMat, cv::Mat &oMat, int &oValue);

/*
 * @brief 图像区域生长
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param iValue 区域生长阈值，像素在某范围内生长
 * @return void
 */
void RegionGrowth(const cv::Mat &iMat, cv::Mat &oMat, int iValue);

/*
 * @brief 图像Sobel算子卷积
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @return void
 */
void Sobel(const cv::Mat &iMat, cv::Mat &oMat);

/*
 * @brief 图像Roberts算子卷积
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @return void
 */
void Roberts(const cv::Mat &iMat, cv::Mat &oMat);

/*
 * @brief 图像Prewitt算子卷积
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @return void
 */
void Prewitt(const cv::Mat &iMat, cv::Mat &oMat);

/*
 * @brief 图像Kirsch算子卷积
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @return void
 */
void Kirsch(const cv::Mat &iMat, cv::Mat &oMat);

/*
 * @brief 图像高斯拉普拉斯算子卷积
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @return void
 */
void LOG(const cv::Mat &iMat, cv::Mat &oMat);

/*
 * @brief 图像点检测（拉普拉斯卷积）
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @param iType 四邻域(0)或八邻域(1)
 * @return void
 */
void PointDetect(const cv::Mat &iMat, cv::Mat &oMat, int iType);

/*
 * @brief 图像线检测
 * @param iMat 输入图像
 * @param oMat 输出图像
 * @return void
 */
void LineDetect(const cv::Mat &iMat, cv::Mat &oMat);
} // namespace ImgTools
