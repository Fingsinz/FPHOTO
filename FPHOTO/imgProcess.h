#pragma once

#include <opencv2/opencv.hpp>

// ͼ�����ȵ���
// @param in������ͼ��
// @param out�����ͼ��
// @param factor����������
void imgBrightnessChange(cv::Mat in, cv::Mat &out, int factor);

// ͼ��Աȶȵ���
// @param in������ͼ��
// @param out�����ͼ��
// @param factor����������
void imgContrastChange(cv::Mat in, cv::Mat &out, double factor);

// ͼ������
// @param in������ͼ��
// @param out�����ͼ��
// @param factor����������
void imgResize(cv::Mat in, cv::Mat &out, double factor);

// ͼ����ת
// @param in������ͼ��
// @param out�����ͼ��
// @param angle����ת�Ƕ���
void imgRotate(cv::Mat in, cv::Mat &out, double angle);

// ͼ��ˮƽ��ת
// @param in������ͼ��
// @param out�����ͼ��
void imgHorizontalFlip(cv::Mat in, cv::Mat &out);

// ͼ��ֱ��ת
// @param in������ͼ��
// @param out�����ͼ��
void imgVerticalFlip(cv::Mat in, cv::Mat &out);

// ͼ��ɫ
// @param in������ͼ��
// @param out�����ͼ��
void imgInversion(cv::Mat in, cv::Mat &out);

// ͼ��ҶȻ�
// @param in������ͼ��
// @param out�����ͼ��
// @param type���Ƿ��Ե�ͨ����ʽ���
void imgGrayscale(cv::Mat in, cv::Mat &out, bool type);

// ͼ��ͳ��ֱ��ͼ
// @param in������ͼ��
// @param out�����ͼ��
void drawHistogram(cv::Mat in, cv::Mat &out);

// ͼ��ֱ��ͼ���⻯
// @param in������ͼ��
// @param out�����ͼ��
// @param hist�����⻯���ֱ��ͼ
void imgHistEqualization(cv::Mat in, cv::Mat &out, cv::Mat &hist);

// ͼ������ƽ���˲�
// @param in������ͼ��
// @param out�����ͼ��
// @param ksize���˴�С
void imgArithmeticAverageFiltering(cv::Mat in, cv::Mat &out, int ksize);

// ͼ�񼸺�ƽ���˲�
// @param in������ͼ��
// @param out�����ͼ��
// @param ksize���˴�С
void imgGeometricAverageFiltering(cv::Mat in, cv::Mat &out, int ksize);

// ͼ��г����ֵ�˲�
// @param in������ͼ��
// @param out�����ͼ��
// @param ksize���˴�С
void imgHarmonicMeanFiltering(cv::Mat in, cv::Mat &out, int ksize);

// ͼ����ֵ�˲�
// @param in������ͼ��
// @param out�����ͼ��
// @param ksize���˴�С
void imgMedianFiltering(cv::Mat in, cv::Mat &out, int ksize);

// ͼ�������Сֵ�˲�
// @param in������ͼ��
// @param out�����ͼ��
// @param ksize���˴�С
// @param type���˲����ͣ����ֵ������Сֵ
void imgMaxMinFiltering(cv::Mat in, cv::Mat &out, int ksize, int type);

// ͼ��������˹��
// @param in������ͼ��
// @param out�����ͼ��
// @param ksize���˴�С
// @param type���˲����ͣ��������ǰ�����
void imgLaplaceFiltering(cv::Mat in, cv::Mat &out, int type);

// ͼ�����͸�ʴ
// @param in������ͼ��
// @param out�����ͼ��
// @param kernelSize���˴�С
// @param kernelType��������
// @param op����������
// @param cnt����������
void imgDilationOrErosion(cv::Mat in, cv::Mat &out, int kernelSize, int kernelType, int op, int cnt);

// ͼ�񿪱�����
// @param in������ͼ��
// @param out�����ͼ��
// @param kernelSize���˴�С
// @param kernelType��������
// @param op����������
// @param cnt����������
void imgOpeningOrClosing(cv::Mat in, cv::Mat &out, int kernelSize, int kernelType, int op, int cnt);

// ͼ��ñ�任��ñ�任
// @param in������ͼ��
// @param out�����ͼ��
// @param kernelSize���˴�С
// @param kernelType��������
// @param op����������
// @param cnt����������
void imgTopOrBottomHatTrans(cv::Mat in, cv::Mat &out, int kernelSize, int kernelType, int op, int cnt);

// ͼ�������ֵ�ָ�
// @param in������ͼ��
// @param out�����ͼ��
// @param T��������ֵ
void imgThresholdSegment(cv::Mat in, cv::Mat &out, int T);

// ͼ���Զ�ȫ����ֵ�ָ�
// @param in������ͼ��
// @param out�����ͼ��
// @param T���Զ�ȫ���������ֵ����
void imgIterativeThresholdSegment(cv::Mat in, cv::Mat &out, int &T);

// ͼ��OTSU��ֵ�ָ�
// @param in������ͼ��
// @param out�����ͼ��
// @param T��OTSU�������ֵ����
void imgOTSUThresholdSegment(cv::Mat in, cv::Mat &out, int &T);

// ͼ����������
// @param in������ͼ��
// @param out�����ͼ��
// @param T������������ֵ��������ĳ��Χ������
void imgRegionGrowth(cv::Mat in, cv::Mat &out, int T);

// ͼ��Sobel���Ӿ��
// @param in������ͼ��
// @param out�����ͼ��
void sobel(cv::Mat in, cv::Mat &out);

// ͼ��Roberts���Ӿ��
// @param in������ͼ��
// @param out�����ͼ��
void roberts(cv::Mat in, cv::Mat &out);

// ͼ��Prewitt���Ӿ��
// @param in������ͼ��
// @param out�����ͼ��
void prewitt(cv::Mat in, cv::Mat &out);

// ͼ��Kirsch���Ӿ��
// @param in������ͼ��
// @param out�����ͼ��
void kirsch(cv::Mat in, cv::Mat &out);

// ͼ���˹������˹���Ӿ��
// @param in������ͼ��
// @param out�����ͼ��
void LOG(cv::Mat in, cv::Mat &out);

// ͼ����⣨������˹�����
// @param in������ͼ��
// @param out�����ͼ��
// @param type��������������
void imgPointDetect(cv::Mat in, cv::Mat &out, int type);

// ͼ���߼��
// @param in������ͼ��
// @param out�����ͼ��
void imgLineDetect(cv::Mat in, cv::Mat &out);