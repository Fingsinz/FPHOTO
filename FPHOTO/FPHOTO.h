#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FPHOTO.h"
#include "Global.h"
#include "imgProcess.h"
#include "qgraphicsscene.h"
#include "PhotoShow.h"
#include <opencv2/opencv.hpp>

class FPHOTO : public QMainWindow
{
	Q_OBJECT

public:
	FPHOTO(QWidget *parent = nullptr);
	~FPHOTO();

	QGraphicsView *gs;

	void MENU();

	void postProcess();

	bool openImg;

public Q_SLOTS:

	void openFile();
	void newFile();
	void saveFile();
	void undo();
	void closeWindow();

	void _imgResize();
	void _imgRotate();
	void _imgHorizontalFlip();
	void _imgVerticalFlip();
	void _imgInversion();
	void _imgGrayscale();
	void _imgCrop();

	void _drawHistogram();
	void _imgHistEqualization();

	void _imgArithmeticAverageFiltering();
	void _imgGeometricAverageFiltering();
	void _imgHarmonicMeanFiltering();
	void _imgMedianFiltering();
	void _imgMinFiltering();
	void _imgMaxFiltering();
	void _imgLaplaceFiltering();

	void _imgDilationOrErosion();
	void _imgOpeningOrClosing();
	void _imgTopOrBottomHatTrans();

	void _imgThresholdSegment();
	void _imgIterativeThresholdSegment();
	void _imgOTSUThresholdSegment();
	void _imgRegionGrowth();

	void _imgPointDetect();
	void _imgLineDetect();
	void _imgEdgeDetect();

private:
	Ui::FPHOTOClass ui;
};
