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

	ui.stackSize->setText(QString::fromLocal8Bit("�ɳ�����") + QString::number(imageData.imgs.size()));
}

FPHOTO::~FPHOTO()
{}

void FPHOTO::MENU()
{
	QFont font;
	font.setFamily(QString::fromLocal8Bit("΢���ź� Light"));
	font.setPixelSize(16);

	ui.statusBar->setFont(font);

	QMenu *fileMenu = ui.menuBar->addMenu(QString::fromLocal8Bit("�ļ�"));
	fileMenu->setFont(font);
	fileMenu->addAction(QString::fromLocal8Bit("��"), this, SLOT(openFile()))->setShortcut(QKeySequence::Open);
	fileMenu->addAction(QString::fromLocal8Bit("�½�"), this, SLOT(newFile()))->setShortcut(QKeySequence::New);
	fileMenu->addAction(QString::fromLocal8Bit("����"), this, SLOT(saveFile()))->setShortcut(QKeySequence::Save);
	fileMenu->addAction(QString::fromLocal8Bit("�˳�"), this, SLOT(closeWindow()))->setShortcut(QKeySequence::Close);

	QMenu *editMenu = ui.menuBar->addMenu(QString::fromLocal8Bit("�༭"));
	editMenu->setFont(font);
	editMenu->addAction(QString::fromLocal8Bit("����"), this, SLOT(undo()))->setShortcut(QKeySequence::Undo);

	QMenu *basicMenu = editMenu->addMenu(QString::fromLocal8Bit("ͼ������༭"));
	basicMenu->setFont(font);
	basicMenu->addAction(QString::fromLocal8Bit("ͼ����ת"), this, SLOT(_imgRotate()));
	QMenu *flipMenu = basicMenu->addMenu(QString::fromLocal8Bit("ͼ��ת"));
	flipMenu->setFont(font);
	flipMenu->addAction(QString::fromLocal8Bit("ˮƽ��ת"), this, SLOT(_imgHorizontalFlip()));
	flipMenu->addAction(QString::fromLocal8Bit("��ֱ��ת"), this, SLOT(_imgVerticalFlip()));
	basicMenu->addAction(QString::fromLocal8Bit("ͼ��ɫ"), this, SLOT(_imgInversion()));
	basicMenu->addAction(QString::fromLocal8Bit("ͼ��ҶȻ�"), this, SLOT(_imgGrayscale()));
	basicMenu->addAction(QString::fromLocal8Bit("ͼ��ü�"), this, SLOT(_imgCrop()));
	basicMenu->addAction(QString::fromLocal8Bit("ͼ��ֱ��ͼͳ��"), this, SLOT(_drawHistogram()));
	basicMenu->addAction(QString::fromLocal8Bit("ͼ��ֱ��ͼ����"), this, SLOT(_imgHistEqualization()));

	QMenu *adjustMenu = editMenu->addMenu(QString::fromLocal8Bit("����"));
	adjustMenu->setFont(font);
	adjustMenu->addAction(QString::fromLocal8Bit("ͼ���С����"), this, SLOT(_imgResize()));
	//adjustMenu->addAction(QString::fromLocal8Bit("ɫ�ʵ���"), this, SLOT());

	QMenu *filterMenu = editMenu->addMenu(QString::fromLocal8Bit("ͼ���˲�"));
	filterMenu->setFont(font);
	QMenu *high = filterMenu->addMenu(QString::fromLocal8Bit("��ͨ�˲�"));
	high->setFont(font);
	high->addAction(QString::fromLocal8Bit("������˹��"), this, SLOT(_imgLaplaceFiltering()));
	QMenu *low = filterMenu->addMenu(QString::fromLocal8Bit("��ͨ�˲�"));
	low->setFont(font);
	low->addAction(QString::fromLocal8Bit("����ƽ���˲�"), this, SLOT(_imgArithmeticAverageFiltering()));
	low->addAction(QString::fromLocal8Bit("����ƽ���˲�"), this, SLOT(_imgGeometricAverageFiltering()));
	low->addAction(QString::fromLocal8Bit("г����ֵ�˲�"), this, SLOT(_imgHarmonicMeanFiltering()));
	low->addAction(QString::fromLocal8Bit("��ֵ�˲�"), this, SLOT(_imgMedianFiltering()));
	low->addAction(QString::fromLocal8Bit("��Сֵ�˲�"), this, SLOT(_imgMinFiltering()));
	low->addAction(QString::fromLocal8Bit("���ֵ�˲�"), this, SLOT(_imgMaxFiltering()));

	QMenu *morphologyMenu = editMenu->addMenu(QString::fromLocal8Bit("ͼ����̬ѧ����"));
	morphologyMenu->setFont(font);
	morphologyMenu->addAction(QString::fromLocal8Bit("����/��ʴ"), this, SLOT(_imgDilationOrErosion()));
	morphologyMenu->addAction(QString::fromLocal8Bit("������/������"), this, SLOT(_imgOpeningOrClosing()));
	morphologyMenu->addAction(QString::fromLocal8Bit("��ñ�任/��ñ�任"), this, SLOT(_imgTopOrBottomHatTrans()));

	QMenu *detectAndSegMenu = editMenu->addMenu(QString::fromLocal8Bit("ͼ��ָ�����"));
	detectAndSegMenu->setFont(font);

	QMenu *segmentMenu = detectAndSegMenu->addMenu(QString::fromLocal8Bit("�ָ�"));
	segmentMenu->setFont(font);
	QMenu *thsegmentMenu = segmentMenu->addMenu(QString::fromLocal8Bit("������ֵ�ָ�"));
	thsegmentMenu->setFont(font);
	thsegmentMenu->addAction(QString::fromLocal8Bit("ָ����ֵ�ָ�"), this, SLOT(_imgThresholdSegment()));
	thsegmentMenu->addAction(QString::fromLocal8Bit("ȫ���Զ���ֵ�ָ�"), this, SLOT(_imgIterativeThresholdSegment()));
	thsegmentMenu->addAction(QString::fromLocal8Bit("OTSU��ֵ�ָ�"), this, SLOT(_imgOTSUThresholdSegment()));
	segmentMenu->addAction(QString::fromLocal8Bit("���������ָ�"), this, SLOT(_imgRegionGrowth()));
	QMenu *detectMenu = detectAndSegMenu->addMenu(QString::fromLocal8Bit("���"));
	detectMenu->setFont(font);
	detectMenu->addAction(QString::fromLocal8Bit("����"), this, SLOT(_imgPointDetect()));
	detectMenu->addAction(QString::fromLocal8Bit("�߼��"), this, SLOT(_imgLineDetect()));
	detectMenu->addAction(QString::fromLocal8Bit("��Ե���"), this, SLOT(_imgEdgeDetect()));

	ui.menuBar->addAction(QString::fromLocal8Bit("����"), this, [this]
		{
			QMessageBox::about(this, QString::fromLocal8Bit("����"),
			QString::fromLocal8Bit("FPHOTO\t����: ������Fingsinz\n"));
		});
}

void FPHOTO::postProcess()
{
	ui.img->pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	ui.img->scene->addItem(ui.img->pixmapItem);
	ui.img->setScene(ui.img->scene);

	ui.stackSize->setText(QString::fromLocal8Bit("�ɳ�����") + QString::number(imageData.imgs.size()));
	ui.imginfo->setText(QString::fromLocal8Bit("ͼƬ��Ϣ��") + QString::number(imageData.nowImg.rows) + QString::fromLocal8Bit("��") + QString::number(imageData.nowImg.cols) + QString::fromLocal8Bit("��ͨ������") + QString::number(imageData.nowImg.channels()));
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
	QString filename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("ѡ��ͼ���ļ�"), "", tr("*.jpg *.png *.tif *.tiff *.bmp"));
	if (filename.isEmpty())
		return;

	imageData.nowImg = cv::imread(filename.toLocal8Bit().toStdString());
	if (!imageData.nowImg.empty())
	{
		openImg = true;
		imageData.keepTrace(imageData.nowImg);
		ui.info->append(QString::fromLocal8Bit("���ļ�"));
		postProcess();
	}
	else
		QMessageBox::warning(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��ͼƬʧ��"));
}

void FPHOTO::newFile()
{
	openImg = true;
	imageData.nowImg = cv::Mat(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));
	imageData.keepTrace(imageData.nowImg);
	ui.info->append(QString::fromLocal8Bit("�½��ļ�"));
	postProcess();
}

void FPHOTO::saveFile()
{
	if (!openImg or imageData.nowImg.empty())	return;
	QString filename = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("����ͼ���ļ�"), "", tr("*.jpg *.png *.tif *.tiff *.bmp"));
	if (filename.isEmpty())	return;
	cv::imwrite(filename.toLocal8Bit().toStdString(), imageData.nowImg);
	ui.info->append(QString::fromLocal8Bit("�����ļ�"));
	ui.statusBar->showMessage(QString::fromLocal8Bit("�ѱ���"));
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
	ui.info->append(QString::fromLocal8Bit("��������"));
	ui.stackSize->setText(QString::fromLocal8Bit("�ɳ�����") + QString::number(imageData.imgs.size()));
	ui.statusBar->showMessage(QString::fromLocal8Bit("�ѳ���"));
	ui.imginfo->setText(QString::fromLocal8Bit("ͼƬ��Ϣ��") + QString::number(imageData.nowImg.rows) + QString::fromLocal8Bit("��") + QString::number(imageData.nowImg.cols) + QString::fromLocal8Bit("��ͨ������") + QString::number(imageData.nowImg.channels()));
	if (imageData.imgs.empty())
	{
		imageData.nowImg.release();
		ui.img->setScene(nullptr);
		openImg = false;
		ui.info->append(QString::fromLocal8Bit("��������"));
		ui.stackSize->setText(QString::fromLocal8Bit("�ɳ�����") + QString::number(imageData.imgs.size()));
		ui.statusBar->showMessage(QString::fromLocal8Bit("�ѳ���"));
		ui.imginfo->clear();
	}
}

void FPHOTO::_imgResize()
{
	if (!openImg)	return;

	QFont font; font.setFamily(QString::fromLocal8Bit("΢���ź� Light")), font.setPixelSize(12);
	QDialog dlg(this);
	dlg.setWindowTitle(QString::fromLocal8Bit("����")), dlg.setFont(font), dlg.setFixedSize(600, 600);
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
	QLabel *info = new QLabel(QString::fromLocal8Bit("ͼ���С��") + QString::number((int)imageData.nowImg.rows) + QString::fromLocal8Bit("��") + QString::number((int)imageData.nowImg.rows), &dlg);
	form.addRow(info);

	connect(slider, &QSlider::valueChanged, &dlg, [slider, label, info] {
		double factor = slider->value() / 10.0;
		label->setText(QString::number(factor));
		info->setText(QString::fromLocal8Bit("ͼ���С��") + QString::number((int)imageData.nowImg.rows * factor) + QString::fromLocal8Bit("��") + QString::number((int)imageData.nowImg.rows * factor));
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

	// ���ô���
	ui.info->append(QString::fromLocal8Bit("ͼƬ����"));
	postProcess();
}

void FPHOTO::_imgRotate()
{
	if (!openImg)	return;

	double angle = QInputDialog::getDouble(this, QString::fromLocal8Bit("��ת"), QString::fromLocal8Bit("��ת�Ƕ�"), 0, -360, 360);
	if (angle == 0)	return;

	imageData.keepTrace(imageData.nowImg);
	// ���ܺ���
	imgRotate(imageData.nowImg, imageData.nowImg, angle);

	// ���ô���
	ui.info->append(QString::fromLocal8Bit("ͼƬ��ת") + QString::number(angle) + QString::fromLocal8Bit("��"));
	postProcess();
}

void FPHOTO::_imgHorizontalFlip()
{
	if (!openImg)	return;

	imageData.keepTrace(imageData.nowImg);
	// ���ܺ���
	imgHorizontalFlip(imageData.nowImg, imageData.nowImg);

	// ���ô���
	ui.info->append(QString::fromLocal8Bit("ͼƬˮƽ��ת"));
	postProcess();
}

void FPHOTO::_imgVerticalFlip()
{
	if (!openImg)	return;

	imageData.keepTrace(imageData.nowImg);
	// ���ܺ���
	imgVerticalFlip(imageData.nowImg, imageData.nowImg);

	// ���ô���
	ui.info->append(QString::fromLocal8Bit("ͼƬ��ֱ��ת"));
	postProcess();
}

void FPHOTO::_imgInversion()
{
	if (!openImg)	return;

	imageData.keepTrace(imageData.nowImg);
	// ���ܺ���
	imgInversion(imageData.nowImg, imageData.nowImg);

	// ���ô���
	ui.info->append(QString::fromLocal8Bit("ͼƬ��ɫ"));
	postProcess();
}

void FPHOTO::_imgGrayscale()
{
	if (!openImg or imageData.nowImg.channels() == 1)	return;

	bool ok;
	QString select = QInputDialog::getItem(this, QString::fromLocal8Bit("�ҶȻ�������ʽ"), QString::fromLocal8Bit(""), QStringList() << QString::fromLocal8Bit("�Ե�ͨ������") << QString::fromLocal8Bit("����ͨ������"), 0, false, &ok);

	bool type = select == QString::fromLocal8Bit("�Ե�ͨ������");
	if (!ok)return;

	imageData.keepTrace(imageData.nowImg);
	// ���ܺ���
	imgGrayscale(imageData.nowImg, imageData.nowImg, type);

	// ���ô���
	ui.info->append(QString::fromLocal8Bit("ͼƬ�ҶȻ�"));
	postProcess();
}

void FPHOTO::_imgCrop()
{
	if (!openImg)	return;
	imageData.keepTrace(imageData.nowImg);
	ui.img->type = 2, ui.img->_color = cv::Scalar(255, 255, 255), ui.img->_thickness = 1, ui.img->isCrop = 1;

	// ���ô���
	ui.info->append(QString::fromLocal8Bit("ͼƬ�ü�"));
	postProcess();
}

void FPHOTO::_drawHistogram()
{
	if (!openImg)	return;

	// ���ܺ���
	drawHistogram(imageData.nowImg, imageData.Hist);

	// ���ô���
	QFont font; font.setFamily(QString::fromLocal8Bit("΢���ź� Light")), font.setPixelSize(16);
	QDialog dlg(this);
	QFormLayout form(&dlg);
	dlg.setFont(font), dlg.setWindowTitle(QString::fromLocal8Bit("ͼ��ֱ��ͼ"));

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
	// ���ܺ���
	imgHistEqualization(imageData.nowImg, imageData.nowImg, imageData.Hist);

	// ���ô���
	ui.info->append(QString::fromLocal8Bit("ֱ��ͼ���⻯"));
	postProcess();
}

void FPHOTO::_imgArithmeticAverageFiltering()
{
	if (!openImg)	return;

	int ksize;

	QFont font; font.setFamily(QString::fromLocal8Bit("΢���ź� Light")), font.setPixelSize(16);
	QDialog dlg(this);	dlg.setWindowTitle(QString::fromLocal8Bit("����ƽ���˲�"));
	QFormLayout form(&dlg);
	dlg.setFont(font);

	QGraphicsView *gv = new QGraphicsView(&dlg);
	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	gv->setScene(gs);
	form.addRow(gv);

	QLineEdit *le = new QLineEdit(&dlg);
	QLabel *lb = new QLabel(&dlg);
	lb->setText(QString::fromLocal8Bit("�����˲��˴�С��"));
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

	// ���ô���
	ui.info->append(QString::fromLocal8Bit("ͼ������ƽ���˲���") + QString::number(ksize));
	postProcess();
}

void FPHOTO::_imgGeometricAverageFiltering()
{
	if (!openImg)	return;

	int ksize;

	QFont font; font.setFamily(QString::fromLocal8Bit("΢���ź� Light")), font.setPixelSize(16);
	QDialog dlg(this); dlg.setWindowTitle(QString::fromLocal8Bit("����ƽ���˲�"));
	QFormLayout form(&dlg);
	dlg.setFont(font);

	QGraphicsView *gv = new QGraphicsView(&dlg);
	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	gv->setScene(gs);
	form.addRow(gv);

	QLineEdit *le = new QLineEdit(&dlg);
	QLabel *lb = new QLabel(&dlg);
	lb->setText(QString::fromLocal8Bit("�����˲��˴�С��"));
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

	// ���ô���
	ui.info->append(QString::fromLocal8Bit("ͼ�񼸺�ƽ���˲���") + QString::number(ksize));
	postProcess();
}

void FPHOTO::_imgHarmonicMeanFiltering()
{
	if (!openImg)	return;

	int ksize;

	QFont font; font.setFamily(QString::fromLocal8Bit("΢���ź� Light")), font.setPixelSize(16);
	QDialog dlg(this); dlg.setWindowTitle(QString::fromLocal8Bit("г����ֵ�˲�"));
	QFormLayout form(&dlg);
	dlg.setFont(font);

	QGraphicsView *gv = new QGraphicsView(&dlg);
	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	gv->setScene(gs);
	form.addRow(gv);

	QLineEdit *le = new QLineEdit(&dlg);
	QLabel *lb = new QLabel(&dlg);
	lb->setText(QString::fromLocal8Bit("�����˲��˴�С��"));
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

	// ���ô���
	ui.info->append(QString::fromLocal8Bit("ͼ��г����ֵ�˲���") + QString::number(ksize));
	postProcess();
}

void FPHOTO::_imgMedianFiltering()
{
	if (!openImg)	return;

	int ksize;

	QFont font; font.setFamily(QString::fromLocal8Bit("΢���ź� Light")), font.setPixelSize(16);
	QDialog dlg(this); dlg.setWindowTitle(QString::fromLocal8Bit("��ֵ�˲�"));
	QFormLayout form(&dlg);
	dlg.setFont(font);

	QGraphicsView *gv = new QGraphicsView(&dlg);
	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	gv->setScene(gs);
	form.addRow(gv);

	QLineEdit *le = new QLineEdit(&dlg);
	QLabel *lb = new QLabel(&dlg);
	lb->setText(QString::fromLocal8Bit("�����˲��˴�С��"));
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

	// ���ô���
	ui.info->append(QString::fromLocal8Bit("ͼ����ֵ�˲���") + QString::number(ksize));
	postProcess();
}

void FPHOTO::_imgMinFiltering()
{
	if (!openImg)	return;

	int ksize;

	QFont font; font.setFamily(QString::fromLocal8Bit("΢���ź� Light")), font.setPixelSize(16);
	QDialog dlg(this); dlg.setWindowTitle(QString::fromLocal8Bit("��Сֵ�˲�"));
	QFormLayout form(&dlg);
	dlg.setFont(font);

	QGraphicsView *gv = new QGraphicsView(&dlg);
	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	gv->setScene(gs);
	form.addRow(gv);

	QLineEdit *le = new QLineEdit(&dlg);
	QLabel *lb = new QLabel(&dlg);
	lb->setText(QString::fromLocal8Bit("�����˲��˴�С��"));
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

	// ���ô���
	ui.info->append(QString::fromLocal8Bit("ͼ����Сֵ�˲���") + QString::number(ksize));
	postProcess();
}

void FPHOTO::_imgMaxFiltering()
{
	if (!openImg)	return;

	int ksize;

	QFont font; font.setFamily(QString::fromLocal8Bit("΢���ź� Light")), font.setPixelSize(16);
	QDialog dlg(this); dlg.setWindowTitle(QString::fromLocal8Bit("���ֵ�˲�"));
	QFormLayout form(&dlg);
	dlg.setFont(font);

	QGraphicsView *gv = new QGraphicsView(&dlg);
	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	gv->setScene(gs);
	form.addRow(gv);

	QLineEdit *le = new QLineEdit(&dlg);
	QLabel *lb = new QLabel(&dlg);
	lb->setText(QString::fromLocal8Bit("�����˲��˴�С��"));
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

	// ���ô���
	ui.info->append(QString::fromLocal8Bit(" ͼ�����ֵ�˲���") + QString::number(ksize));
	postProcess();
}

void FPHOTO::_imgLaplaceFiltering()
{
	if (!openImg)	return;

	QFont font; font.setFamily(QString::fromLocal8Bit("΢���ź� Light")), font.setPixelSize(16);
	QDialog dlg(this); dlg.setWindowTitle(QString::fromLocal8Bit("������˹��ͨ�˲�"));
	QFormLayout form(&dlg);
	dlg.setFont(font);

	QGraphicsView *gv = new QGraphicsView(&dlg);
	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	gv->setScene(gs);
	form.addRow(gv);

	QComboBox *cb = new QComboBox(&dlg);
	cb->addItem(QString::fromLocal8Bit("��������"));
	cb->addItem(QString::fromLocal8Bit("��������"));
	cb->addItem(QString::fromLocal8Bit("��ѡ��"));	cb->setCurrentIndex(2);
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

	// ���ô���
	ui.info->append(QString::fromLocal8Bit("ͼ����"));
	postProcess();
}

void FPHOTO::_imgDilationOrErosion()
{
	if (!openImg)	return;

	int op;
	int kernelSize;
	int kernelType;
	int cnt;

	// ִ������Ի�
	QFont font; font.setFamily(QString::fromLocal8Bit("΢���ź� Light")), font.setPixelSize(16);
	QDialog dlg(this);
	QFormLayout form(&dlg);
	dlg.setFont(font), dlg.setWindowTitle(QString::fromLocal8Bit("���͸�ʴԤ��"));

	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	QGraphicsView *gv = new QGraphicsView(&dlg);	gv->setScene(gs);
	form.addRow(gv);
	dlg.setFixedSize(max(400, imageData.nowImg.cols), max(400, imageData.nowImg.rows));

	QComboBox *cb1 = new QComboBox(&dlg);
	cb1->addItem(QString::fromLocal8Bit(" ����"));
	cb1->addItem(QString::fromLocal8Bit(" ��ʴ"));
	QString tips1 = QString::fromLocal8Bit(" ��ѡ�����ͻ�ʴ������");
	form.addRow(tips1, cb1);

	QSpinBox *sp1 = new QSpinBox(&dlg);
	sp1->setMinimum(1), sp1->setMaximum(10), sp1->setValue(1);
	QString tips2 = QString::fromLocal8Bit(" ���������ͻ�ʴ�ṹԪ��С��");
	form.addRow(tips2, sp1);

	QComboBox *cb2 = new QComboBox(&dlg);
	cb2->addItem(QString::fromLocal8Bit(" ����"));
	cb2->addItem(QString::fromLocal8Bit(" ʮ����"));
	cb2->addItem(QString::fromLocal8Bit(" ��Բ"));
	QString tips3 = QString::fromLocal8Bit(" ��ѡ�����ͻ�ʴ�ṹԪ��״��");
	form.addRow(tips3, cb2);

	QSpinBox *sp2 = new QSpinBox(&dlg);
	sp2->setMinimum(1), sp2->setMaximum(10), sp2->setValue(1);
	QString tips4 = QString::fromLocal8Bit(" ���������ͻ�ʴ������");
	form.addRow(tips4, sp2);

	QPushButton *pb = new QPushButton(&dlg);
	pb->setText(QString::fromLocal8Bit("Ӧ��"));
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
		ui.info->append(QString::fromLocal8Bit("ͼ������"));
	else if (op == 1)
		ui.info->append(QString::fromLocal8Bit("ͼ��ʴ"));
	postProcess();
}

void FPHOTO::_imgOpeningOrClosing()
{
	if (!openImg)	return;

	int op;
	int kernelSize;
	int kernelType;
	int cnt;

	// ִ������Ի�
	QFont font; font.setFamily(QString::fromLocal8Bit("΢���ź� Light")), font.setPixelSize(16);
	QDialog dlg(this);
	QFormLayout form(&dlg);
	dlg.setFont(font), dlg.setWindowTitle(QString::fromLocal8Bit("�����������Ԥ��"));

	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	QGraphicsView *gv = new QGraphicsView(&dlg);	gv->setScene(gs);
	form.addRow(gv);
	dlg.setFixedSize(max(400, imageData.nowImg.cols), max(400, imageData.nowImg.rows));

	QComboBox *cb1 = new QComboBox(&dlg);
	cb1->addItem(QString::fromLocal8Bit(" ������"));
	cb1->addItem(QString::fromLocal8Bit(" ������"));
	QString tips = QString::fromLocal8Bit(" ��ѡ�����������������");
	form.addRow(tips, cb1);

	QSpinBox *sp1 = new QSpinBox(&dlg);
	sp1->setMinimum(1), sp1->setMaximum(10), sp1->setValue(1);
	QString tips1 = QString::fromLocal8Bit(" �����뿪����������ṹԪ��С��");
	form.addRow(tips1, sp1);

	QComboBox *cb2 = new QComboBox(&dlg);
	cb2->addItem(QString::fromLocal8Bit(" ����"));
	cb2->addItem(QString::fromLocal8Bit(" ʮ����"));
	cb2->addItem(QString::fromLocal8Bit(" ��Բ"));
	QString tips3 = QString::fromLocal8Bit(" ��ѡ������������ṹԪ��״��");
	form.addRow(tips3, cb2);

	QSpinBox *sp2 = new QSpinBox(&dlg);
	sp2->setMinimum(1), sp2->setMaximum(10), sp2->setValue(1);
	QString tips2 = QString::fromLocal8Bit(" �����뿪���������������");
	form.addRow(tips2, sp2);

	QPushButton *pb = new QPushButton(&dlg);
	pb->setText(QString::fromLocal8Bit("Ӧ��"));
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
		ui.info->append(QString::fromLocal8Bit("ͼ������"));
	else if (op == 3)
		ui.info->append(QString::fromLocal8Bit("ͼ�������"));
	postProcess();
}

void FPHOTO::_imgTopOrBottomHatTrans()
{
	if (!openImg)	return;

	int op;
	int kernelSize;
	int kernelType;
	int cnt;

	// ִ������Ի�
	QFont font; font.setFamily(QString::fromLocal8Bit("΢���ź� Light")), font.setPixelSize(16);
	QDialog dlg(this);
	QFormLayout form(&dlg);
	dlg.setFont(font), dlg.setWindowTitle(QString::fromLocal8Bit("��ñ�任��ñ�任Ԥ��"));

	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	QGraphicsView *gv = new QGraphicsView(&dlg);	gv->setScene(gs);
	form.addRow(gv);
	dlg.setFixedSize(max(400, imageData.nowImg.cols), max(400, imageData.nowImg.rows));

	QComboBox *cb1 = new QComboBox(&dlg);
	cb1->addItem(QString::fromLocal8Bit(" ��ñ�任"));
	cb1->addItem(QString::fromLocal8Bit(" ��ñ�任"));
	QString tips = QString::fromLocal8Bit(" ��ѡ��ñ�任���ñ�任��");
	form.addRow(tips, cb1);

	QSpinBox *sp1 = new QSpinBox(&dlg);
	sp1->setMinimum(1), sp1->setMaximum(10), sp1->setValue(1);
	QString tips1 = QString::fromLocal8Bit(" �����붥ñ�任���ñ�任�ṹԪ��С��");
	form.addRow(tips1, sp1);

	QComboBox *cb2 = new QComboBox(&dlg);
	cb2->addItem(QString::fromLocal8Bit(" ����"));
	cb2->addItem(QString::fromLocal8Bit(" ʮ����"));
	cb2->addItem(QString::fromLocal8Bit(" ��Բ"));
	QString tips3 = QString::fromLocal8Bit(" ��ѡ��ñ�任���ñ�任�ṹԪ��״��");
	form.addRow(tips3, cb2);

	QSpinBox *sp2 = new QSpinBox(&dlg);
	sp2->setMinimum(1), sp2->setMaximum(10), sp2->setValue(1);
	QString tips2 = QString::fromLocal8Bit(" �����붥ñ�任���ñ�任������");
	form.addRow(tips2, sp2);

	QPushButton *pb = new QPushButton(&dlg);
	pb->setText(QString::fromLocal8Bit("Ӧ��"));
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
		ui.info->append(QString::fromLocal8Bit("ͼ��ñ�任"));
	else if (op == 6)
		ui.info->append(QString::fromLocal8Bit("ͼ���ñ�任"));
	postProcess();
}

void FPHOTO::_imgThresholdSegment()
{
	if (!openImg)	return;

	int t = QInputDialog::getInt(this, QString::fromLocal8Bit("��ֵ�ָ�"), QString::fromLocal8Bit("��������ֵ��"), 0, 0, 255);

	imgThresholdSegment(imageData.nowImg, imageData.gray, t);

	QDialog dlg(this);
	QFormLayout form(&dlg);
	dlg.setFixedSize(420, 400);
	dlg.setWindowTitle(QString::fromLocal8Bit("�Ƿ񱣴渲�ǣ���ͼ��Ϊ��ͨ��ͼ��"));

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
		ui.info->append(QString::fromLocal8Bit("ͼ��ָ����ֵ�ָ") + QString::number(t));
	}

	// ���ô���
	imageData.gray.release();
	ui.img->pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	ui.img->scene->addItem(ui.img->pixmapItem);
	ui.stackSize->setText(QString::fromLocal8Bit("�ɳ�����") + QString::number(imageData.imgs.size()));
	ui.imginfo->setText(QString::fromLocal8Bit("ͼƬ��Ϣ��") + QString::number(imageData.nowImg.rows) + QString::fromLocal8Bit("��") + QString::number(imageData.nowImg.cols) + QString::fromLocal8Bit("��ͨ������") + QString::number(imageData.nowImg.channels()));
}

void FPHOTO::_imgIterativeThresholdSegment()
{
	if (!openImg)	return;

	int t;
	imgIterativeThresholdSegment(imageData.nowImg, imageData.gray, t);

	QDialog dlg(this);
	QFormLayout form(&dlg);
	dlg.setFixedSize(420, 400);
	dlg.setWindowTitle(QString::fromLocal8Bit("�Ƿ񱣴渲�ǣ���ͼ��Ϊ��ͨ��ͼ��"));

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
		ui.info->append(QString::fromLocal8Bit("ͼ��ȫ���Զ���ֵ�ָ") + QString::number(t));
	}

	// ���ô���
	imageData.gray.release();
	ui.img->pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	ui.img->scene->addItem(ui.img->pixmapItem);
	ui.stackSize->setText(QString::fromLocal8Bit("�ɳ�����") + QString::number(imageData.imgs.size()));
	ui.imginfo->setText(QString::fromLocal8Bit("ͼƬ��Ϣ��") + QString::number(imageData.nowImg.rows) + QString::fromLocal8Bit("��") + QString::number(imageData.nowImg.cols) + QString::fromLocal8Bit("��ͨ������") + QString::number(imageData.nowImg.channels()));
}

void FPHOTO::_imgOTSUThresholdSegment()
{
	if (!openImg)	return;

	int t;
	imgOTSUThresholdSegment(imageData.nowImg, imageData.gray, t);

	QDialog dlg(this);
	QFormLayout form(&dlg);
	dlg.setFixedSize(420, 400);
	dlg.setWindowTitle(QString::fromLocal8Bit("�Ƿ񱣴渲�ǣ���ͼ��Ϊ��ͨ��ͼ��"));

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
		ui.info->append(QString::fromLocal8Bit("ͼ��OTSU��ֵ�ָ") + QString::number(t));
	}

	// ���ô���
	imageData.gray.release();
	ui.img->pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	ui.img->scene->addItem(ui.img->pixmapItem);
	ui.stackSize->setText(QString::fromLocal8Bit("�ɳ�����") + QString::number(imageData.imgs.size()));
	ui.imginfo->setText(QString::fromLocal8Bit("ͼƬ��Ϣ��") + QString::number(imageData.nowImg.rows) + QString::fromLocal8Bit("��") + QString::number(imageData.nowImg.cols) + QString::fromLocal8Bit("��ͨ������") + QString::number(imageData.nowImg.channels()));
}

void FPHOTO::_imgRegionGrowth()
{
	if (!openImg)	return;
	if (imageData.nowImg.channels() != 1)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("ͼ�����������ָ����ڵ�ͨ���½���!"));
		return;
	}

	int T = QInputDialog::getInt(this, QString::fromLocal8Bit("ͼ�����������ָ�"), QString::fromLocal8Bit("����������������ֵ:"));
	imgRegionGrowth(imageData.nowImg, imageData.gray, T);

	QDialog dlg(this);
	QFormLayout form(&dlg);
	dlg.setFixedSize(400, 400);
	dlg.setWindowTitle(QString::fromLocal8Bit("�Ƿ񱣴渲�ǣ���ͼ��Ϊ��ͨ��ͼ��"));

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
		ui.info->append(QString::fromLocal8Bit("ͼ�����������ָ�"));
	}

	// ���ô���
	imageData.gray.release();
	ui.img->pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	ui.img->scene->addItem(ui.img->pixmapItem);
	ui.stackSize->setText(QString::fromLocal8Bit("�ɳ�����") + QString::number(imageData.imgs.size()));
	ui.imginfo->setText(QString::fromLocal8Bit("ͼƬ��Ϣ��") + QString::number(imageData.nowImg.rows) + QString::fromLocal8Bit("��") + QString::number(imageData.nowImg.cols) + QString::fromLocal8Bit("��ͨ������") + QString::number(imageData.nowImg.channels()));
}

void FPHOTO::_imgPointDetect()
{
	if (!openImg)	return;

	QFont font; font.setFamily(QString::fromLocal8Bit("΢���ź� Light")), font.setPixelSize(16);
	QDialog dlg(this);
	dlg.setWindowTitle(QString::fromLocal8Bit("����")), dlg.setFont(font);
	QFormLayout form(&dlg);

	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	QGraphicsView *gv = new QGraphicsView(&dlg);	gv->setScene(gs);
	form.addRow(gv);

	QComboBox *cb = new QComboBox(&dlg);
	cb->addItem(QString::fromLocal8Bit("��������"));
	cb->addItem(QString::fromLocal8Bit("��������"));
	cb->addItem(QString::fromLocal8Bit("��ѡ��"));	cb->setCurrentIndex(2);
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

	// ���ô���
	ui.info->append(QString::fromLocal8Bit("ͼ���������"));
	postProcess();
}

void FPHOTO::_imgLineDetect()
{
	if (!openImg)	return;

	QFont font; font.setFamily(QString::fromLocal8Bit("΢���ź� Light")), font.setPixelSize(16);
	QDialog dlg(this);
	dlg.setWindowTitle(QString::fromLocal8Bit("�߼��")), dlg.setFont(font);
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

	// ���ô���
	ui.info->append(QString::fromLocal8Bit("ͼ���߼��"));
	postProcess();
}

void FPHOTO::_imgEdgeDetect()
{
	if (!openImg)	return;

	QFont font; font.setFamily(QString::fromLocal8Bit("΢���ź� Light")), font.setPixelSize(16);
	QDialog dlg(this);
	dlg.setWindowTitle(QString::fromLocal8Bit("�߼��")), dlg.setFont(font);
	QFormLayout form(&dlg);

	QGraphicsScene *gs = new QGraphicsScene(&dlg);
	gs->addPixmap(QPixmap::fromImage(MatToQImage(imageData.nowImg)));
	QGraphicsView *gv = new QGraphicsView(&dlg);	gv->setScene(gs);
	form.addRow(gv);

	QComboBox *cb = new QComboBox(&dlg);
	cb->addItem(QString::fromLocal8Bit("Sobel���ӱ�Ե���"));
	cb->addItem(QString::fromLocal8Bit("Robert���ӱ�Ե���"));
	cb->addItem(QString::fromLocal8Bit("Prewitt���ӱ�Ե���"));
	cb->addItem(QString::fromLocal8Bit("Kirsch���ӱ�Ե���"));
	cb->addItem(QString::fromLocal8Bit("��˹������˹���ӱ�Ե���"));
	cb->addItem(QString::fromLocal8Bit("Canny��Ե���"));
	cb->addItem(QString::fromLocal8Bit("��ѡ��"));	cb->setCurrentIndex(6);
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

	// ���ô���
	ui.info->append(QString::fromLocal8Bit("ͼ���Ե���"));
	postProcess();
}