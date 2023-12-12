#pragma once

#include <QGraphicsView>
#include "QGraphicsPixmapItem"
#include "qmouseeventtransition.h"
#include "qevent.h"
#include <opencv2/opencv.hpp>

class PhotoShow : public QGraphicsView
{
public:
	explicit PhotoShow(QWidget *parent = 0);

	QGraphicsPixmapItem *pixmapItem;
	QGraphicsScene *scene;
	QPointF point, prepoint;

	cv::Rect box;		// �ü�����
	cv::Scalar _color;	// ������ɫ
	int _thickness;		// ���ʴ�ϸ
	int type;			// ͼ������
	bool isPaint;		// �Ƿ�ͼģʽ
	bool isCrop;		// �Ƿ�ü�ģʽ
	bool isText;		// �Ƿ�����ģʽ
	std::string s;		// ����ı�

	void mousePressEvent(QMouseEvent *event);

	void mouseMoveEvent(QMouseEvent *event);

	void mouseReleaseEvent(QMouseEvent *event);
};
