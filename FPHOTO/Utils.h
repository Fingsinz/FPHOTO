#pragma once

#include <iostream>
#include <stdio.h>

#include "qimage.h"
#include <locale.h>
#include <opencv2/opencv.hpp>
#include <wchar.h>
#include <windows.h>

/**
 * @brief cv::Mat image 到 QImage 的转换
 * @param mat 源
 * @return QImage
 */
QImage MatToQImage(const cv::Mat &mat);

/**
 * @brief QImage 到 cv::Mat image 的转换
 * @param image 源
 * @return cv::Mat
 */
cv::Mat QImage2cvMat(QImage image);

/**
 * @brief 使用指定的参数在给定图像上绘制中文文本
 * @param dst 将在其上绘制文本的图像
 * @param str 要绘制的文本
 * @param org 将绘制文本的原点
 * @param color 文本的颜色
 * @param fontSize 字体大小
 * @param fn 字体名称
 * @param italic 字体是否应该是斜体
 * @param underline 字体是否应该有下划线
 * @return void
 */
void putTextZH(
    cv::Mat &dst,
    const char *str,
    cv::Point org,
    cv::Scalar color,
    int fontSize,
    const char *fn,
    bool italic,
    bool underline);