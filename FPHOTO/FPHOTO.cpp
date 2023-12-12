#include "FPHOTO.h"
#include "util.h"
#include "Global.h"
#include "PhotoShow.h"
#include <opencv2/opencv.hpp>
#include "qfiledialog.h"
#include "qmessagebox.h"
#include "qinputdialog.h"
#include "qcolordialog.h"
#include "qfontdialog.h"
#include "qformlayout.h"
#include "qspinbox.h"
#include "qdialogbuttonbox.h"
#include "qcombobox.h"
#include "qgraphicsview.h"
#include "qpushbutton.h"

extern Global imageData;

FPHOTO::FPHOTO(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("FPHOTO");
	ui.img->setScene(ui.img->scene);
	MENU();
	openImg = false;

	ui.stackSize->setText(QString::fromLocal8Bit("可撤销：") + QString::number(imageData.imgs.size()));
}

FPHOTO::~FPHOTO()
{}

void FPHOTO::MENU()
{
	QFont font;
	font.setFamily(QString::fromLocal8Bit("微软雅黑 Light"));
	font.setPixelSize(16);

	ui.statusBar->setFont(font);

	QMenu *fileMenu = ui.menuBar->addMenu(QString::fromLocal8Bit("文件"));
	fileMenu->setFont(font);
	fileMenu->addAction(QString::fromLocal8Bit("打开"), this, SLOT(openFile()))->setShortcut(QKeySequence::Open);
	fileMenu->addAction(QString::fromLocal8Bit("新建"), this, SLOT(newFile()))->setShortcut(QKeySequence::New);
	fileMenu->addAction(QString::fromLocal8Bit("保存"), this, SLOT(saveFile()))->setShortcut(QKeySequence::Save);
	fileMenu->addAction(QString::fromLocal8Bit("退出"), this, SLOT(closeWindow()))->setShortcut(QKeySequence::Close);

	QMenu *editMenu = ui.menuBar->addMenu(QString::fromLocal8Bit("编辑"));
	editMenu->setFont(font);
	editMenu->addAction(QString::fromLocal8Bit("撤销"), this, SLOT(undo()))->setShortcut(QKeySequence::Undo);

	QMenu *basicMenu = editMenu->addMenu(QString::fromLocal8Bit("图像基本编辑"));
	basicMenu->setFont(font);
	basicMenu->addAction(QString::fromLocal8Bit("图像旋转"), this, SLOT(_imgRotate()));
	QMenu *flipMenu = basicMenu->addMenu(QString::fromLocal8Bit("图像翻转"));
	flipMenu->setFont(font);
	flipMenu->addAction(QString::fromLocal8Bit("水平翻转"), this, SLOT(_imgHorizontalFlip()));
	flipMenu->addAction(QString::fromLocal8Bit("垂直翻转"), this, SLOT(_imgVerticalFlip()));
	basicMenu->addAction(QString::fromLocal8Bit("图像反色"), this, SLOT(_imgInversion()));
	basicMenu->addAction(QString::fromLocal8Bit("图像灰度化"), this, SLOT(_imgGrayscale()));
	basicMenu->addAction(QString::fromLocal8Bit("图像裁剪"), this, SLOT(_imgCrop()));
	basicMenu->addAction(QString::fromLocal8Bit("图像直方图统计"), this, SLOT(_drawHistogram()));
	basicMenu->addAction(QString::fromLocal8Bit("图像直方图均衡"), this, SLOT(_imgHistEqualization()));

	QMenu *adjustMenu = editMenu->addMenu(QString::fromLocal8Bit("调节"));
	adjustMenu->setFont(font);
	adjustMenu->addAction(QString::fromLocal8Bit("图像大小调节"), this, SLOT(_imgResize()));
	//adjustMenu->addAction(QString::fromLocal8Bit("色彩调节"), this, SLOT());

	QMenu *filterMenu = editMenu->addMenu(QString::fromLocal8Bit("图像滤波"));
	filterMenu->setFont(font);
	QMenu *high = filterMenu->addMenu(QString::fromLocal8Bit("高通滤波"));
	high->setFont(font);
	high->addAction(QString::fromLocal8Bit("拉普拉斯锐化"), this, SLOT(_imgLaplaceFiltering()));
	QMenu *low = filterMenu->addMenu(QString::fromLocal8Bit("低通滤波"));
	low->setFont(font);
	low->addAction(QString::fromLocal8Bit("算术平均滤波"), this, SLOT(_imgArithmeticAverageFiltering()));
	low->addAction(QString::fromLocal8Bit("几何平均滤波"), this, SLOT(_imgGeometricAverageFiltering()));
	low->addAction(QString::fromLocal8Bit("谐波均值滤波"), this, SLOT(_imgHarmonicMeanFiltering()));
	low->addAction(QString::fromLocal8Bit("中值滤波"), this, SLOT(_imgMedianFiltering()));
	low->addAction(QString::fromLocal8Bit("最小值滤波"), this, SLOT(_imgMinFiltering()));
	low->addAction(QString::fromLocal8Bit("最大值滤波"), this, SLOT(_imgMaxFiltering()));

	QMenu *morphologyMenu = editMenu->addMenu(QString::fromLocal8Bit("图像形态学处理"));
	morphologyMenu->setFont(font);
	morphologyMenu->addAction(QString::fromLocal8Bit("膨胀/腐蚀"), this, SLOT(_imgDilationOrErosion()));
	morphologyMenu->addAction(QString::fromLocal8Bit("开运算/闭运算"), this, SLOT(_imgOpeningOrClosing()));
	morphologyMenu->addAction(QString::fromLocal8Bit("顶帽变换/底帽变换"), this, SLOT(_imgTopOrBottomHatTrans()));

	QMenu *detectAndSegMenu = editMenu->addMenu(QString::fromLocal8Bit("图像分割与检测"));
	detectAndSegMenu->setFont(font);

	QMenu *segmentMenu = detectAndSegMenu->addMenu(QString::fromLocal8Bit("分割"));
	segmentMenu->setFont(font);
	QMenu *thsegmentMenu = segmentMenu->addMenu(QString::fromLocal8Bit("区域阈值分割"));
	thsegmentMenu->setFont(font);
	thsegmentMenu->addAction(QString::fromLocal8Bit("指定阈值分割"), this, SLOT(_imgThresholdSegment()));
	thsegmentMenu->addAction(QString::fromLocal8Bit("全局自动阈值分割"), this, SLOT(_imgIterativeThresholdSegment()));
	thsegmentMenu->addAction(QString::fromLocal8Bit("OTSU阈值分割"), this, SLOT(_imgOTSUThresholdSegment()));
	segmentMenu->addAction(QString::fromLocal8Bit("区域生长分割"), this, SLOT(_imgRegionGrowth()));
	QMenu *detectMenu = detectAndSegMenu->addMenu(QString::fromLocal8Bit("检测"));
	detectMenu->setFont(font);
	detectMenu->addAction(QString::fromLocal8Bit("点检测"), this, SLOT(_imgPointDetect()));
	detectMenu->addAction(QString::fromLocal8Bit("线检测"), this, SLOT(_imgLineDetect()));
	detectMenu->addAction(QString::fromLocal8Bit("边缘检测"), this, SLOT(_imgEdgeDetect()));

	ui.menuBar->addAction(QString::fromLocal8Bit("关于"), this, [this]
		{
			QMessageBox::about(this, QString::fromLocal8Bit("关于"),
			QString::fromLocal8Bit("FPHOTO\t作者: 风信梓Fingsinz\n"));
		});
}

void FPHOTO::postProcess()
{
	ui.img->pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	ui.img->scene->addItem(ui.img->pixmapItem);
	ui.img->setScene(ui.img->scene);

	ui.stackSize->setText(QString::fromLocal8Bit("可撤销：") + QString::number(imageData.imgs.size()));
	ui.imginfo->setText(QString::fromLocal8Bit("图片信息：") + QString::number(imageData.nowImg.rows) + QString::fromLocal8Bit("×") + QString::number(imageData.nowImg.cols) + QString::fromLocal8Bit("，通道数：") + QString::number(imageData.nowImg.channels()));
}

void FPHOTO::closeWindow()
{
	QApplication *app;
	app->exit(0);
}

void FPHOTO::openFile()
{
	imageData.nowImg.release();
	openImg = false;
	QString filename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择图像文件"), "", tr("*.jpg *.png *.tif *.tiff *.bmp"));
	if (filename.isEmpty())
		return;

	imageData.nowImg = cv::imread(filename.toLocal8Bit().toStdString());
	if (!imageData.nowImg.empty())
	{
		openImg = true;
		imageData.keepTrace(imageData.nowImg);
		ui.info->append(QString::fromLocal8Bit("打开文件"));
		postProcess();
	}
	else
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("打开图片失败"));
}

void FPHOTO::newFile()
{
	openImg = true;
	imageData.nowImg = cv::Mat(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));
	imageData.keepTrace(imageData.nowImg);
	ui.info->append(QString::fromLocal8Bit("新建文件"));
	postProcess();
}

void FPHOTO::saveFile()
{
	if (!openImg or imageData.nowImg.empty())	return;
	QString filename = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("保存图像文件"), "", tr("*.jpg *.png *.tif *.tiff *.bmp"));
	if (filename.isEmpty())	return;
	cv::imwrite(filename.toLocal8Bit().toStdString(), imageData.nowImg);
	ui.info->append(QString::fromLocal8Bit("保存文件"));
	ui.statusBar->showMessage(QString::fromLocal8Bit("已保存"));
}

void FPHOTO::undo()
{
	if (imageData.imgs.empty())
	{
		imageData.nowImg.release();
		return;
	}

	imageData.nowImg = imageData.imgs.back();

	imageData.imgs.pop_back();
	ui.img->pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	ui.img->scene->addItem(ui.img->pixmapItem);
	ui.info->append(QString::fromLocal8Bit("撤销动作"));
	ui.stackSize->setText(QString::fromLocal8Bit("可撤销：") + QString::number(imageData.imgs.size()));
	ui.statusBar->showMessage(QString::fromLocal8Bit("已撤销"));
	ui.imginfo->setText(QString::fromLocal8Bit("图片信息：") + QString::number(imageData.nowImg.rows) + QString::fromLocal8Bit("×") + QString::number(imageData.nowImg.cols) + QString::fromLocal8Bit("，通道数：") + QString::number(imageData.nowImg.channels()));
	if (imageData.imgs.empty())
	{
		imageData.nowImg.release();
		ui.img->setScene(nullptr);
		openImg = false;
		ui.info->append(QString::fromLocal8Bit("撤销动作"));
		ui.stackSize->setText(QString::fromLocal8Bit("可撤销：") + QString::number(imageData.imgs.size()));
		ui.statusBar->showMessage(QString::fromLocal8Bit("已撤销"));
		ui.imginfo->clear();
	}
}

void FPHOTO::_imgResize()
{
	if (!openImg)	return;

	QFont font; font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(12);
	QDialog dlg(this);
	dlg.setWindowTitle(QString::fromLocal8Bit("缩放")), dlg.setFont(font), dlg.setFixedSize(600, 600);
	QFormLayout form(&dlg);

	QGraphicsScene *showImg = new QGraphicsScene(&dlg);
	showImg->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	QGraphicsView *gv = new QGraphicsView(&dlg);	gv->setScene(showImg);
	form.addRow(gv);

	QSlider *slider = new QSlider(Qt::Horizontal, &dlg);
	form.addRow(slider);
	slider->setMinimum(1), slider->setMaximum(100), slider->setValue(10);
	QLabel *label = new QLabel(QString::number(1), &dlg);
	form.addRow(label, slider);
	QLabel *info = new QLabel(QString::fromLocal8Bit("图像大小：") + QString::number((int)imageData.nowImg.rows) + QString::fromLocal8Bit("×") + QString::number((int)imageData.nowImg.rows), &dlg);
	form.addRow(info);

	connect(slider, &QSlider::valueChanged, &dlg, [slider, label, info] {
		double factor = slider->value() / 10.0;
		label->setText(QString::number(factor));
		info->setText(QString::fromLocal8Bit("图像大小：") + QString::number((int)imageData.nowImg.rows * factor) + QString::fromLocal8Bit("×") + QString::number((int)imageData.nowImg.rows * factor));
		});
	connect(slider, &QSlider::sliderReleased, &dlg, [slider, showImg] {
		showImg->clear();
		double factor = slider->value() / 10.0;
		imgResize(imageData.nowImg, imageData.tmp, factor);
		showImg->addPixmap(QPixmap::fromImage(MatToQImage(imageData.tmp)));
		});

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Rejected)	return;

	imageData.keepTrace(imageData.nowImg);
	imageData.nowImg = imageData.tmp.clone();

	// 后置处理
	ui.info->append(QString::fromLocal8Bit("图片缩放"));
	postProcess();
}

void FPHOTO::_imgRotate()
{
	if (!openImg)	return;

	double angle = QInputDialog::getDouble(this, QString::fromLocal8Bit("旋转"), QString::fromLocal8Bit("旋转角度"), 0, -360, 360);
	if (angle == 0)	return;

	imageData.keepTrace(imageData.nowImg);
	// 功能函数
	imgRotate(imageData.nowImg, imageData.nowImg, angle);

	// 后置处理
	ui.info->append(QString::fromLocal8Bit("图片旋转") + QString::number(angle) + QString::fromLocal8Bit("°"));
	postProcess();
}

void FPHOTO::_imgHorizontalFlip()
{
	if (!openImg)	return;

	imageData.keepTrace(imageData.nowImg);
	// 功能函数
	imgHorizontalFlip(imageData.nowImg, imageData.nowImg);

	// 后置处理
	ui.info->append(QString::fromLocal8Bit("图片水平翻转"));
	postProcess();
}

void FPHOTO::_imgVerticalFlip()
{
	if (!openImg)	return;

	imageData.keepTrace(imageData.nowImg);
	// 功能函数
	imgVerticalFlip(imageData.nowImg, imageData.nowImg);

	// 后置处理
	ui.info->append(QString::fromLocal8Bit("图片垂直翻转"));
	postProcess();
}

void FPHOTO::_imgInversion()
{
	if (!openImg)	return;

	imageData.keepTrace(imageData.nowImg);
	// 功能函数
	imgInversion(imageData.nowImg, imageData.nowImg);

	// 后置处理
	ui.info->append(QString::fromLocal8Bit("图片反色"));
	postProcess();
}

void FPHOTO::_imgGrayscale()
{
	if (!openImg or imageData.nowImg.channels() == 1)	return;

	bool ok;
	QString select = QInputDialog::getItem(this, QString::fromLocal8Bit("灰度化保存形式"), QString::fromLocal8Bit(""), QStringList() << QString::fromLocal8Bit("以单通道保存") << QString::fromLocal8Bit("以三通道保存"), 0, false, &ok);

	bool type = select == QString::fromLocal8Bit("以单通道保存");
	if (!ok)return;

	imageData.keepTrace(imageData.nowImg);
	// 功能函数
	imgGrayscale(imageData.nowImg, imageData.nowImg, type);

	// 后置处理
	ui.info->append(QString::fromLocal8Bit("图片灰度化"));
	postProcess();
}

void FPHOTO::_imgCrop()
{
	if (!openImg)	return;
	imageData.keepTrace(imageData.nowImg);
	ui.img->type = 2, ui.img->_color = cv::Scalar(255, 255, 255), ui.img->_thickness = 1, ui.img->isCrop = 1;

	// 后置处理
	ui.info->append(QString::fromLocal8Bit("图片裁剪"));
	postProcess();
}

void FPHOTO::_drawHistogram()
{
	if (!openImg)	return;

	// 功能函数
	drawHistogram(imageData.nowImg, imageData.Hist);

	// 后置处理
	QFont font; font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
	QDialog dlg(this);
	QFormLayout form(&dlg);
	dlg.setFont(font), dlg.setWindowTitle(QString::fromLocal8Bit("图像直方图"));

	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.Hist)));
	QGraphicsView *gv = new QGraphicsView(&dlg);
	gv->setScene(gs);
	form.addRow(gv);
	dlg.exec();
}

void FPHOTO::_imgHistEqualization()
{
	if (!openImg)	return;

	imageData.keepTrace(imageData.nowImg);
	// 功能函数
	imgHistEqualization(imageData.nowImg, imageData.nowImg, imageData.Hist);

	// 后置处理
	ui.info->append(QString::fromLocal8Bit("直方图均衡化"));
	postProcess();
}

void FPHOTO::_imgArithmeticAverageFiltering()
{
	if (!openImg)	return;

	int ksize;

	QFont font; font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
	QDialog dlg(this);	dlg.setWindowTitle(QString::fromLocal8Bit("算术平均滤波"));
	QFormLayout form(&dlg);
	dlg.setFont(font);

	QGraphicsView *gv = new QGraphicsView(&dlg);
	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	gv->setScene(gs);
	form.addRow(gv);

	QLineEdit *le = new QLineEdit(&dlg);
	QLabel *lb = new QLabel(&dlg);
	lb->setText(QString::fromLocal8Bit("输入滤波核大小："));
	form.addRow(lb, le);

	connect(le, &QLineEdit::editingFinished, &dlg, [le, lb, gs, &ksize] {
		bool ok;
		ksize = le->text().toInt(&ok);
		if (!ok or ksize == 0)	return;
		ksize = ksize % 2 ? ksize : ksize - 1;
		gs->clear();
		imgArithmeticAverageFiltering(imageData.nowImg, imageData.tmp, ksize);
		gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.tmp)));
		});

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Rejected)	return;

	imageData.keepTrace(imageData.nowImg);
	imageData.nowImg = imageData.tmp.clone();

	// 后置处理
	ui.info->append(QString::fromLocal8Bit("图像算术平均滤波：") + QString::number(ksize));
	postProcess();
}

void FPHOTO::_imgGeometricAverageFiltering()
{
	if (!openImg)	return;

	int ksize;

	QFont font; font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
	QDialog dlg(this); dlg.setWindowTitle(QString::fromLocal8Bit("几何平均滤波"));
	QFormLayout form(&dlg);
	dlg.setFont(font);

	QGraphicsView *gv = new QGraphicsView(&dlg);
	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	gv->setScene(gs);
	form.addRow(gv);

	QLineEdit *le = new QLineEdit(&dlg);
	QLabel *lb = new QLabel(&dlg);
	lb->setText(QString::fromLocal8Bit("输入滤波核大小："));
	form.addRow(lb, le);

	connect(le, &QLineEdit::editingFinished, &dlg, [le, lb, gs, &ksize] {
		bool ok;
		ksize = le->text().toInt(&ok);
		if (!ok or ksize == 0)	return;
		ksize = ksize % 2 ? ksize : ksize - 1;
		gs->clear();
		imgGeometricAverageFiltering(imageData.nowImg, imageData.tmp, ksize);
		gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.tmp)));
		});

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Rejected)	return;

	imageData.keepTrace(imageData.nowImg);
	imageData.nowImg = imageData.tmp.clone();

	// 后置处理
	ui.info->append(QString::fromLocal8Bit("图像几何平均滤波：") + QString::number(ksize));
	postProcess();
}

void FPHOTO::_imgHarmonicMeanFiltering()
{
	if (!openImg)	return;

	int ksize;

	QFont font; font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
	QDialog dlg(this); dlg.setWindowTitle(QString::fromLocal8Bit("谐波均值滤波"));
	QFormLayout form(&dlg);
	dlg.setFont(font);

	QGraphicsView *gv = new QGraphicsView(&dlg);
	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	gv->setScene(gs);
	form.addRow(gv);

	QLineEdit *le = new QLineEdit(&dlg);
	QLabel *lb = new QLabel(&dlg);
	lb->setText(QString::fromLocal8Bit("输入滤波核大小："));
	form.addRow(lb, le);

	connect(le, &QLineEdit::editingFinished, &dlg, [le, lb, gs, &ksize] {
		bool ok;
		ksize = le->text().toInt(&ok);
		if (!ok or ksize == 0)	return;
		ksize = ksize % 2 ? ksize : ksize - 1;
		gs->clear();
		imgHarmonicMeanFiltering(imageData.nowImg, imageData.tmp, ksize);
		gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.tmp)));
		});

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Rejected)	return;

	imageData.keepTrace(imageData.nowImg);
	imageData.nowImg = imageData.tmp.clone();

	// 后置处理
	ui.info->append(QString::fromLocal8Bit("图像谐波均值滤波：") + QString::number(ksize));
	postProcess();
}

void FPHOTO::_imgMedianFiltering()
{
	if (!openImg)	return;

	int ksize;

	QFont font; font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
	QDialog dlg(this); dlg.setWindowTitle(QString::fromLocal8Bit("中值滤波"));
	QFormLayout form(&dlg);
	dlg.setFont(font);

	QGraphicsView *gv = new QGraphicsView(&dlg);
	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	gv->setScene(gs);
	form.addRow(gv);

	QLineEdit *le = new QLineEdit(&dlg);
	QLabel *lb = new QLabel(&dlg);
	lb->setText(QString::fromLocal8Bit("输入滤波核大小："));
	form.addRow(lb, le);

	connect(le, &QLineEdit::editingFinished, &dlg, [le, lb, gs, &ksize] {
		bool ok;
		ksize = le->text().toInt(&ok);
		if (!ok or ksize == 0)	return;
		ksize = ksize % 2 ? ksize : ksize - 1;
		gs->clear();
		imgMedianFiltering(imageData.nowImg, imageData.tmp, ksize);
		gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.tmp)));
		});

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Rejected)	return;

	imageData.keepTrace(imageData.nowImg);
	imageData.nowImg = imageData.tmp.clone();

	// 后置处理
	ui.info->append(QString::fromLocal8Bit("图像中值滤波：") + QString::number(ksize));
	postProcess();
}

void FPHOTO::_imgMinFiltering()
{
	if (!openImg)	return;

	int ksize;

	QFont font; font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
	QDialog dlg(this); dlg.setWindowTitle(QString::fromLocal8Bit("最小值滤波"));
	QFormLayout form(&dlg);
	dlg.setFont(font);

	QGraphicsView *gv = new QGraphicsView(&dlg);
	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	gv->setScene(gs);
	form.addRow(gv);

	QLineEdit *le = new QLineEdit(&dlg);
	QLabel *lb = new QLabel(&dlg);
	lb->setText(QString::fromLocal8Bit("输入滤波核大小："));
	form.addRow(lb, le);

	connect(le, &QLineEdit::editingFinished, &dlg, [le, lb, gs, &ksize] {
		bool ok;
		ksize = le->text().toInt(&ok);
		if (!ok or ksize == 0)	return;
		ksize = ksize % 2 ? ksize : ksize - 1;
		gs->clear();
		imgMaxMinFiltering(imageData.nowImg, imageData.tmp, ksize, 0);
		gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.tmp)));
		});

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Rejected)	return;

	imageData.keepTrace(imageData.nowImg);
	imageData.nowImg = imageData.tmp.clone();

	// 后置处理
	ui.info->append(QString::fromLocal8Bit("图像最小值滤波：") + QString::number(ksize));
	postProcess();
}

void FPHOTO::_imgMaxFiltering()
{
	if (!openImg)	return;

	int ksize;

	QFont font; font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
	QDialog dlg(this); dlg.setWindowTitle(QString::fromLocal8Bit("最大值滤波"));
	QFormLayout form(&dlg);
	dlg.setFont(font);

	QGraphicsView *gv = new QGraphicsView(&dlg);
	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	gv->setScene(gs);
	form.addRow(gv);

	QLineEdit *le = new QLineEdit(&dlg);
	QLabel *lb = new QLabel(&dlg);
	lb->setText(QString::fromLocal8Bit("输入滤波核大小："));
	form.addRow(lb, le);

	connect(le, &QLineEdit::editingFinished, &dlg, [le, lb, gs, &ksize] {
		bool ok;
		ksize = le->text().toInt(&ok);
		if (!ok or ksize == 0)	return;
		ksize = ksize % 2 ? ksize : ksize - 1;
		gs->clear();
		imgMaxMinFiltering(imageData.nowImg, imageData.tmp, ksize, 1);
		gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.tmp)));
		});

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Rejected)	return;

	imageData.keepTrace(imageData.nowImg);
	imageData.nowImg = imageData.tmp.clone();

	// 后置处理
	ui.info->append(QString::fromLocal8Bit(" 图像最大值滤波：") + QString::number(ksize));
	postProcess();
}

void FPHOTO::_imgLaplaceFiltering()
{
	if (!openImg)	return;

	QFont font; font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
	QDialog dlg(this); dlg.setWindowTitle(QString::fromLocal8Bit("拉普拉斯高通滤波"));
	QFormLayout form(&dlg);
	dlg.setFont(font);

	QGraphicsView *gv = new QGraphicsView(&dlg);
	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	gv->setScene(gs);
	form.addRow(gv);

	QComboBox *cb = new QComboBox(&dlg);
	cb->addItem(QString::fromLocal8Bit("四邻域锐化"));
	cb->addItem(QString::fromLocal8Bit("八邻域锐化"));
	cb->addItem(QString::fromLocal8Bit("请选择"));	cb->setCurrentIndex(2);
	form.addRow(cb);

	connect(cb, static_cast<void (QComboBox:: *)(int idx)>(&QComboBox::currentIndexChanged), &dlg, [cb, gs] (int idx) {
		gs->clear();
		if (idx == 2)
		{
			gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
			return;
		}
		imgLaplaceFiltering(imageData.nowImg, imageData.tmp, idx);
		gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.tmp)));
		});

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Rejected)	return;

	imageData.keepTrace(imageData.nowImg);
	imageData.nowImg = imageData.tmp.clone();

	// 后置处理
	ui.info->append(QString::fromLocal8Bit("图像锐化"));
	postProcess();
}

void FPHOTO::_imgDilationOrErosion()
{
	if (!openImg)	return;

	int op;
	int kernelSize;
	int kernelType;
	int cnt;

	// 执行输入对话
	QFont font; font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
	QDialog dlg(this);
	QFormLayout form(&dlg);
	dlg.setFont(font), dlg.setWindowTitle(QString::fromLocal8Bit("膨胀腐蚀预览"));

	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	QGraphicsView *gv = new QGraphicsView(&dlg);	gv->setScene(gs);
	form.addRow(gv);
	dlg.setFixedSize(max(400, imageData.nowImg.cols), max(400, imageData.nowImg.rows));

	QComboBox *cb1 = new QComboBox(&dlg);
	cb1->addItem(QString::fromLocal8Bit(" 膨胀"));
	cb1->addItem(QString::fromLocal8Bit(" 腐蚀"));
	QString tips1 = QString::fromLocal8Bit(" 请选择膨胀或腐蚀操作：");
	form.addRow(tips1, cb1);

	QSpinBox *sp1 = new QSpinBox(&dlg);
	sp1->setMinimum(1), sp1->setMaximum(10), sp1->setValue(1);
	QString tips2 = QString::fromLocal8Bit(" 请输入膨胀或腐蚀结构元大小：");
	form.addRow(tips2, sp1);

	QComboBox *cb2 = new QComboBox(&dlg);
	cb2->addItem(QString::fromLocal8Bit(" 矩形"));
	cb2->addItem(QString::fromLocal8Bit(" 十字形"));
	cb2->addItem(QString::fromLocal8Bit(" 椭圆"));
	QString tips3 = QString::fromLocal8Bit(" 请选择膨胀或腐蚀结构元形状：");
	form.addRow(tips3, cb2);

	QSpinBox *sp2 = new QSpinBox(&dlg);
	sp2->setMinimum(1), sp2->setMaximum(10), sp2->setValue(1);
	QString tips4 = QString::fromLocal8Bit(" 请输入膨胀或腐蚀次数：");
	form.addRow(tips4, sp2);

	QPushButton *pb = new QPushButton(&dlg);
	pb->setText(QString::fromLocal8Bit("应用"));
	form.addRow(pb);

	connect(pb, &QPushButton::clicked, &dlg, [gs, cb1, sp1, cb2, sp2, &kernelSize, &kernelType, &op, &cnt] () {
		gs->clear();
		kernelSize = sp1->value(); kernelType = cb2->currentIndex();
		op = cb1->currentIndex(), cnt = sp2->value();
		imgDilationOrErosion(imageData.nowImg, imageData.tmp, kernelSize, kernelType, op, cnt);
		gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.tmp)));
		});

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Rejected)	return;

	imageData.keepTrace(imageData.nowImg);
	imageData.nowImg = imageData.tmp.clone();

	if (op == 0)
		ui.info->append(QString::fromLocal8Bit("图像膨胀"));
	else if (op == 1)
		ui.info->append(QString::fromLocal8Bit("图像腐蚀"));
	postProcess();
}

void FPHOTO::_imgOpeningOrClosing()
{
	if (!openImg)	return;

	int op;
	int kernelSize;
	int kernelType;
	int cnt;

	// 执行输入对话
	QFont font; font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
	QDialog dlg(this);
	QFormLayout form(&dlg);
	dlg.setFont(font), dlg.setWindowTitle(QString::fromLocal8Bit("开运算闭运算预览"));

	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	QGraphicsView *gv = new QGraphicsView(&dlg);	gv->setScene(gs);
	form.addRow(gv);
	dlg.setFixedSize(max(400, imageData.nowImg.cols), max(400, imageData.nowImg.rows));

	QComboBox *cb1 = new QComboBox(&dlg);
	cb1->addItem(QString::fromLocal8Bit(" 开运算"));
	cb1->addItem(QString::fromLocal8Bit(" 闭运算"));
	QString tips = QString::fromLocal8Bit(" 请选择开运算或闭运算操作：");
	form.addRow(tips, cb1);

	QSpinBox *sp1 = new QSpinBox(&dlg);
	sp1->setMinimum(1), sp1->setMaximum(10), sp1->setValue(1);
	QString tips1 = QString::fromLocal8Bit(" 请输入开运算或闭运算结构元大小：");
	form.addRow(tips1, sp1);

	QComboBox *cb2 = new QComboBox(&dlg);
	cb2->addItem(QString::fromLocal8Bit(" 矩形"));
	cb2->addItem(QString::fromLocal8Bit(" 十字形"));
	cb2->addItem(QString::fromLocal8Bit(" 椭圆"));
	QString tips3 = QString::fromLocal8Bit(" 请选择开运算或闭运算结构元形状：");
	form.addRow(tips3, cb2);

	QSpinBox *sp2 = new QSpinBox(&dlg);
	sp2->setMinimum(1), sp2->setMaximum(10), sp2->setValue(1);
	QString tips2 = QString::fromLocal8Bit(" 请输入开运算或闭运算次数：");
	form.addRow(tips2, sp2);

	QPushButton *pb = new QPushButton(&dlg);
	pb->setText(QString::fromLocal8Bit("应用"));
	form.addRow(pb);

	connect(pb, &QPushButton::clicked, &dlg, [gs, cb1, sp1, cb2, sp2, &kernelSize, &kernelType, &op, &cnt] () {
		gs->clear();
		kernelSize = sp1->value(); kernelType = cb2->currentIndex();
		op = cb1->currentIndex() + 2, cnt = sp2->value();
		imgOpeningOrClosing(imageData.nowImg, imageData.tmp, kernelSize, kernelType, op, cnt);;
		gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.tmp)));
		});

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Rejected)	return;

	imageData.keepTrace(imageData.nowImg);
	imageData.nowImg = imageData.tmp.clone();

	if (op == 2)
		ui.info->append(QString::fromLocal8Bit("图像开运算"));
	else if (op == 3)
		ui.info->append(QString::fromLocal8Bit("图像闭运算"));
	postProcess();
}

void FPHOTO::_imgTopOrBottomHatTrans()
{
	if (!openImg)	return;

	int op;
	int kernelSize;
	int kernelType;
	int cnt;

	// 执行输入对话
	QFont font; font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
	QDialog dlg(this);
	QFormLayout form(&dlg);
	dlg.setFont(font), dlg.setWindowTitle(QString::fromLocal8Bit("顶帽变换底帽变换预览"));

	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	QGraphicsView *gv = new QGraphicsView(&dlg);	gv->setScene(gs);
	form.addRow(gv);
	dlg.setFixedSize(max(400, imageData.nowImg.cols), max(400, imageData.nowImg.rows));

	QComboBox *cb1 = new QComboBox(&dlg);
	cb1->addItem(QString::fromLocal8Bit(" 顶帽变换"));
	cb1->addItem(QString::fromLocal8Bit(" 底帽变换"));
	QString tips = QString::fromLocal8Bit(" 请选择顶帽变换或底帽变换：");
	form.addRow(tips, cb1);

	QSpinBox *sp1 = new QSpinBox(&dlg);
	sp1->setMinimum(1), sp1->setMaximum(10), sp1->setValue(1);
	QString tips1 = QString::fromLocal8Bit(" 请输入顶帽变换或底帽变换结构元大小：");
	form.addRow(tips1, sp1);

	QComboBox *cb2 = new QComboBox(&dlg);
	cb2->addItem(QString::fromLocal8Bit(" 矩形"));
	cb2->addItem(QString::fromLocal8Bit(" 十字形"));
	cb2->addItem(QString::fromLocal8Bit(" 椭圆"));
	QString tips3 = QString::fromLocal8Bit(" 请选择顶帽变换或底帽变换结构元形状：");
	form.addRow(tips3, cb2);

	QSpinBox *sp2 = new QSpinBox(&dlg);
	sp2->setMinimum(1), sp2->setMaximum(10), sp2->setValue(1);
	QString tips2 = QString::fromLocal8Bit(" 请输入顶帽变换或底帽变换次数：");
	form.addRow(tips2, sp2);

	QPushButton *pb = new QPushButton(&dlg);
	pb->setText(QString::fromLocal8Bit("应用"));
	form.addRow(pb);

	connect(pb, &QPushButton::clicked, &dlg, [gs, cb1, sp1, cb2, sp2, &kernelSize, &kernelType, &op, &cnt] () {
		gs->clear();
		kernelSize = sp1->value(); kernelType = cb2->currentIndex();
		op = cb1->currentIndex() + 5, cnt = sp2->value();
		imgTopOrBottomHatTrans(imageData.nowImg, imageData.tmp, kernelSize, kernelType, op, cnt);
		gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.tmp)));
		});

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Rejected)	return;

	imageData.keepTrace(imageData.nowImg);
	imageData.nowImg = imageData.tmp.clone();

	if (op == 5)
		ui.info->append(QString::fromLocal8Bit("图像顶帽变换"));
	else if (op == 6)
		ui.info->append(QString::fromLocal8Bit("图像底帽变换"));
	postProcess();
}

void FPHOTO::_imgThresholdSegment()
{
	if (!openImg)	return;

	int t = QInputDialog::getInt(this, QString::fromLocal8Bit("阈值分割"), QString::fromLocal8Bit("请输入阈值："), 0, 0, 255);

	imgThresholdSegment(imageData.nowImg, imageData.gray, t);

	QDialog dlg(this);
	QFormLayout form(&dlg);
	dlg.setFixedSize(420, 400);
	dlg.setWindowTitle(QString::fromLocal8Bit("是否保存覆盖？该图像为单通道图像！"));

	QGraphicsScene *showImg = new QGraphicsScene(&dlg);
	showImg->addPixmap(QPixmap::fromImage(MatToQImage(imageData.gray)));
	QGraphicsView *gv = new QGraphicsView(&dlg);	gv->setScene(showImg);
	form.addRow(gv);

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Accepted)
	{
		imageData.keepTrace(imageData.nowImg);
		imageData.nowImg = imageData.gray.clone();
		ui.info->append(QString::fromLocal8Bit("图像指定阈值分割：") + QString::number(t));
	}

	// 后置处理
	imageData.gray.release();
	ui.img->pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	ui.img->scene->addItem(ui.img->pixmapItem);
	ui.stackSize->setText(QString::fromLocal8Bit("可撤销：") + QString::number(imageData.imgs.size()));
	ui.imginfo->setText(QString::fromLocal8Bit("图片信息：") + QString::number(imageData.nowImg.rows) + QString::fromLocal8Bit("×") + QString::number(imageData.nowImg.cols) + QString::fromLocal8Bit("，通道数：") + QString::number(imageData.nowImg.channels()));
}

void FPHOTO::_imgIterativeThresholdSegment()
{
	if (!openImg)	return;

	int t;
	imgIterativeThresholdSegment(imageData.nowImg, imageData.gray, t);

	QDialog dlg(this);
	QFormLayout form(&dlg);
	dlg.setFixedSize(420, 400);
	dlg.setWindowTitle(QString::fromLocal8Bit("是否保存覆盖？该图像为单通道图像！"));

	QGraphicsScene *showImg = new QGraphicsScene(&dlg);
	showImg->addPixmap(QPixmap::fromImage(MatToQImage(imageData.gray)));
	QGraphicsView *gv = new QGraphicsView(&dlg);	gv->setScene(showImg);
	form.addRow(gv);

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Accepted)
	{
		imageData.keepTrace(imageData.nowImg);
		imageData.nowImg = imageData.gray.clone();
		ui.info->append(QString::fromLocal8Bit("图像全局自动阈值分割：") + QString::number(t));
	}

	// 后置处理
	imageData.gray.release();
	ui.img->pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	ui.img->scene->addItem(ui.img->pixmapItem);
	ui.stackSize->setText(QString::fromLocal8Bit("可撤销：") + QString::number(imageData.imgs.size()));
	ui.imginfo->setText(QString::fromLocal8Bit("图片信息：") + QString::number(imageData.nowImg.rows) + QString::fromLocal8Bit("×") + QString::number(imageData.nowImg.cols) + QString::fromLocal8Bit("，通道数：") + QString::number(imageData.nowImg.channels()));
}

void FPHOTO::_imgOTSUThresholdSegment()
{
	if (!openImg)	return;

	int t;
	imgOTSUThresholdSegment(imageData.nowImg, imageData.gray, t);

	QDialog dlg(this);
	QFormLayout form(&dlg);
	dlg.setFixedSize(420, 400);
	dlg.setWindowTitle(QString::fromLocal8Bit("是否保存覆盖？该图像为单通道图像！"));

	QGraphicsScene *showImg = new QGraphicsScene(&dlg);
	showImg->addPixmap(QPixmap::fromImage(MatToQImage(imageData.gray)));
	QGraphicsView *gv = new QGraphicsView(&dlg);	gv->setScene(showImg);
	form.addRow(gv);

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Accepted)
	{
		imageData.keepTrace(imageData.nowImg);
		imageData.nowImg = imageData.gray.clone();
		ui.info->append(QString::fromLocal8Bit("图像OTSU阈值分割：") + QString::number(t));
	}

	// 后置处理
	imageData.gray.release();
	ui.img->pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	ui.img->scene->addItem(ui.img->pixmapItem);
	ui.stackSize->setText(QString::fromLocal8Bit("可撤销：") + QString::number(imageData.imgs.size()));
	ui.imginfo->setText(QString::fromLocal8Bit("图片信息：") + QString::number(imageData.nowImg.rows) + QString::fromLocal8Bit("×") + QString::number(imageData.nowImg.cols) + QString::fromLocal8Bit("，通道数：") + QString::number(imageData.nowImg.channels()));
}

void FPHOTO::_imgRegionGrowth()
{
	if (!openImg)	return;
	if (imageData.nowImg.channels() != 1)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("图像区域生长分割须在单通道下进行!"));
		return;
	}

	int T = QInputDialog::getInt(this, QString::fromLocal8Bit("图像区域生长分割"), QString::fromLocal8Bit("请输入区域生长阈值:"));
	imgRegionGrowth(imageData.nowImg, imageData.gray, T);

	QDialog dlg(this);
	QFormLayout form(&dlg);
	dlg.setFixedSize(400, 400);
	dlg.setWindowTitle(QString::fromLocal8Bit("是否保存覆盖？该图像为单通道图像！"));

	QGraphicsScene *showImg = new QGraphicsScene(&dlg);
	showImg->addPixmap(QPixmap::fromImage(MatToQImage(imageData.gray)));
	QGraphicsView *gv = new QGraphicsView(&dlg);	gv->setScene(showImg);
	form.addRow(gv);

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Accepted)
	{
		imageData.keepTrace(imageData.nowImg);
		imageData.nowImg = imageData.gray.clone();
		ui.info->append(QString::fromLocal8Bit("图像区域生长分割"));
	}

	// 后置处理
	imageData.gray.release();
	ui.img->pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	ui.img->scene->addItem(ui.img->pixmapItem);
	ui.stackSize->setText(QString::fromLocal8Bit("可撤销：") + QString::number(imageData.imgs.size()));
	ui.imginfo->setText(QString::fromLocal8Bit("图片信息：") + QString::number(imageData.nowImg.rows) + QString::fromLocal8Bit("×") + QString::number(imageData.nowImg.cols) + QString::fromLocal8Bit("，通道数：") + QString::number(imageData.nowImg.channels()));
}

void FPHOTO::_imgPointDetect()
{
	if (!openImg)	return;

	QFont font; font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
	QDialog dlg(this);
	dlg.setWindowTitle(QString::fromLocal8Bit("点检测")), dlg.setFont(font);
	QFormLayout form(&dlg);

	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	QGraphicsView *gv = new QGraphicsView(&dlg);	gv->setScene(gs);
	form.addRow(gv);

	QComboBox *cb = new QComboBox(&dlg);
	cb->addItem(QString::fromLocal8Bit("四邻域检测"));
	cb->addItem(QString::fromLocal8Bit("八邻域检测"));
	cb->addItem(QString::fromLocal8Bit("请选择"));	cb->setCurrentIndex(2);
	form.addRow(cb);

	connect(cb, static_cast<void (QComboBox:: *)(int idx)>(&QComboBox::currentIndexChanged), &dlg, [cb, gs] (int idx) {
		gs->clear();
		if (idx == 2)
		{
			gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
			return;
		}
		imgPointDetect(imageData.nowImg, imageData.tmp, idx);
		gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.tmp)));
		});

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Rejected)	return;

	imageData.keepTrace(imageData.nowImg);
	imageData.nowImg = imageData.tmp.clone();

	// 后置处理
	ui.info->append(QString::fromLocal8Bit("图像孤立点检测"));
	postProcess();
}

void FPHOTO::_imgLineDetect()
{
	if (!openImg)	return;

	QFont font; font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
	QDialog dlg(this);
	dlg.setWindowTitle(QString::fromLocal8Bit("线检测")), dlg.setFont(font);
	QFormLayout form(&dlg);

	imgLineDetect(imageData.nowImg, imageData.tmp);
	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.tmp)));
	QGraphicsView *gv = new QGraphicsView(&dlg);	gv->setScene(gs);
	form.addRow(gv);

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Rejected)	return;

	imageData.keepTrace(imageData.nowImg);
	imageData.nowImg = imageData.tmp.clone();

	// 后置处理
	ui.info->append(QString::fromLocal8Bit("图像线检测"));
	postProcess();
}

void FPHOTO::_imgEdgeDetect()
{
	if (!openImg)	return;

	QFont font; font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
	QDialog dlg(this);
	dlg.setWindowTitle(QString::fromLocal8Bit("线检测")), dlg.setFont(font);
	QFormLayout form(&dlg);

	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	QGraphicsView *gv = new QGraphicsView(&dlg);	gv->setScene(gs);
	form.addRow(gv);

	QComboBox *cb = new QComboBox(&dlg);
	cb->addItem(QString::fromLocal8Bit("Sobel算子边缘检测"));
	cb->addItem(QString::fromLocal8Bit("Robert算子边缘检测"));
	cb->addItem(QString::fromLocal8Bit("Prewitt算子边缘检测"));
	cb->addItem(QString::fromLocal8Bit("Kirsch算子边缘检测"));
	cb->addItem(QString::fromLocal8Bit("高斯拉普拉斯算子边缘检测"));
	cb->addItem(QString::fromLocal8Bit("Canny边缘检测"));
	cb->addItem(QString::fromLocal8Bit("请选择"));	cb->setCurrentIndex(6);
	form.addRow(cb);

	connect(cb, static_cast<void (QComboBox:: *)(int idx)>(&QComboBox::currentIndexChanged), &dlg, [cb, gs] (int idx) {
		gs->clear();
		if (idx == 6)
		{
			gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
			return;
		}
		switch (idx)
		{
			case 0: sobel(imageData.nowImg, imageData.tmp);	break;
			case 1: roberts(imageData.nowImg, imageData.tmp);	break;
			case 2: prewitt(imageData.nowImg, imageData.tmp);	break;
			case 3: kirsch(imageData.nowImg, imageData.tmp);	break;
			case 4: LOG(imageData.nowImg, imageData.tmp);	break;
			case 5: cv::Canny(imageData.nowImg, imageData.tmp, 100, 150);	break;
			default:
				break;
		}
		gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.tmp)));
		});

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Rejected)	return;

	imageData.keepTrace(imageData.nowImg);
	imageData.nowImg = imageData.tmp.clone();

	// 后置处理
	ui.info->append(QString::fromLocal8Bit("图像边缘检测"));
	postProcess();
}