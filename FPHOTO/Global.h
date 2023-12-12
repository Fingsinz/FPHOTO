#pragma once

#include <opencv2/opencv.hpp>
#include <deque>

class Global
{
public:
	cv::Mat nowImg;				// 当前图像
	std::deque<cv::Mat> imgs;	// 撤销栈
	cv::Mat Hist;				// 存放直方图
	cv::Mat gray;				// 存放灰度图
	cv::Mat tmp;				// 存放临时数据

	void keepTrace(cv::Mat &now);
};
