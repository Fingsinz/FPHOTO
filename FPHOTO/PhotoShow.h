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

	cv::Rect box;		// 裁剪区域
	cv::Scalar _color;	// 画笔颜色
	int _thickness;		// 画笔粗细
	int type;			// 图画类型
	bool isPaint;		// 是否画图模式
	bool isCrop;		// 是否裁剪模式
	bool isText;		// 是否文字模式
	std::string s;		// 添加文本

	void mousePressEvent(QMouseEvent *event);

	void mouseMoveEvent(QMouseEvent *event);

	void mouseReleaseEvent(QMouseEvent *event);
};
