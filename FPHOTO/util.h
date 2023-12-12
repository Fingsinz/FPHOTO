#pragma once

#include <opencv2/opencv.hpp>
#include "qimage.h"
#include <stdio.h>
#include <iostream>
#include <locale.h>
#include <wchar.h>
#include <windows.h>

QImage MatToQImage(const cv::Mat &mat);
cv::Mat QImage2cvMat(QImage image);
void putTextZH(cv::Mat &dst, const char *str, cv::Point org, cv::Scalar color, int fontSize, const char *fn, bool italic, bool underline);