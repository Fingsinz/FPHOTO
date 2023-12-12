#include "util.h"
#include "FPHOTO.h"
#include "Global.h"
#include "PhotoShow.h"
#include "Global.h"
#include "imgProcess.h"

extern Global imageData;

PhotoShow::PhotoShow(QWidget *parent) :
	QGraphicsView(parent)
{
	type = -1, isCrop = 0, isText = 0, _color = cv::Scalar(255, 255, 255), _thickness = 1;
	isPaint = false;
	pixmapItem = new QGraphicsPixmapItem;
	scene = new QGraphicsScene(this);
	setScene(scene);
}

void PhotoShow::mousePressEvent(QMouseEvent *e)
{
	if (type == -1)	return;
	prepoint = pixmapItem->mapFromScene(mapToScene(e->pos()));
	if (isText)
	{
		putTextZH(imageData.nowImg, s.c_str(), cv::Point(prepoint.x(), prepoint.y()), _color, _thickness, "Î¢ÈíÑÅºÚ", false, false);
		pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
		scene->addItem(pixmapItem);
		return;
	}

	isPaint = true;
}

void PhotoShow::mouseMoveEvent(QMouseEvent *e)
{
	if (!isPaint or type == -1)	return;
	point = pixmapItem->mapFromScene(mapToScene(e->pos()));
	imageData.tmp = imageData.nowImg.clone();
	switch (type)
	{
		case 0:
			circle(imageData.nowImg, cv::Point(point.x(), point.y()), _thickness, _color, -2);
			break;
		case 1:
			line(imageData.tmp, cv::Point(prepoint.x(), prepoint.y()), cv::Point(point.x(), point.y()), _color, _thickness);
			break;
		case 2:
			rectangle(imageData.tmp, cv::Point(prepoint.x(), prepoint.y()), cv::Point(point.x(), point.y()), _color, _thickness);
			break;
		case 3:
			circle(imageData.tmp, cv::Point(prepoint.x(), prepoint.y()), abs(prepoint.x() - point.x()) >= 0 ? abs(prepoint.x() - point.x()) / 2 : 0, _color, _thickness);
			break;
		default:
			break;
	}

	pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(imageData.tmp)));
	scene->addItem(pixmapItem);
}

void PhotoShow::mouseReleaseEvent(QMouseEvent *e)
{
	if (isCrop)
	{
		box = cv::Rect(cv::Point(prepoint.x(), prepoint.y()), cv::Point(point.x(), point.y()));
		imageData.nowImg = cv::Mat(imageData.nowImg, box);
		pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
		scene->addItem(pixmapItem);
		isCrop = false;
		type = -1;
		return;
	}

	if (!isPaint or type == -1)	return;
	point = pixmapItem->mapFromScene(mapToScene(e->pos()));
	switch (type)
	{
		case 0:
			break;
		case 1:
			line(imageData.nowImg, cv::Point(prepoint.x(), prepoint.y()), cv::Point(point.x(), point.y()), _color, _thickness);
			break;
		case 2:
			rectangle(imageData.nowImg, cv::Point(prepoint.x(), prepoint.y()), cv::Point(point.x(), point.y()), _color, _thickness);
			break;
		case 3:
			circle(imageData.nowImg, cv::Point(prepoint.x(), prepoint.y()), abs(prepoint.x() - point.x()) >= 0 ? abs(prepoint.x() - point.x()) / 2 : 0, _color, _thickness);
			break;
		default:
			break;
	}

	isPaint = false;
	imageData.nowImg = imageData.tmp.clone();
	pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	scene->addItem(pixmapItem);
}