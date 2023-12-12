#pragma once

#include <opencv2/opencv.hpp>
#include <deque>

class Global
{
public:
	cv::Mat nowImg;				// ��ǰͼ��
	std::deque<cv::Mat> imgs;	// ����ջ
	cv::Mat Hist;				// ���ֱ��ͼ
	cv::Mat gray;				// ��ŻҶ�ͼ
	cv::Mat tmp;				// �����ʱ����

	void keepTrace(cv::Mat &now);
};
