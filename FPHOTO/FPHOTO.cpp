#include <opencv2/opencv.hpp>

#include "qcolordialog.h"
#include "qcombobox.h"
#include "qdialogbuttonbox.h"
#include "qfiledialog.h"
#include "qfontdialog.h"
#include "qformlayout.h"
#include "qgraphicsview.h"
#include "qinputdialog.h"
#include "qmessagebox.h"
#include "qpushbutton.h"
#include "qspinbox.h"

#include "FPHOTO.h"
#include "ImgInstance.h"
#include "PhotoShow.h"
#include "Utils.h"

FPHOTO::FPHOTO(QWidget *parent)
        : QMainWindow(parent) {
    ui.setupUi(this);
    this->setWindowTitle("FPHOTO");
    ui.img->setScene(ui.img->m_scene);
    SetMenu();
    m_bOpenImg = false;

    ui.light->setMaximum(255), ui.light->setMinimum(-255);
    ui.light->setValue(0), ui.light_num->setText(QString::number(0));
    ui.contrast->setMaximum(100), ui.contrast->setMinimum(0);
    ui.contrast->setValue(20), ui.contrast_num->setText(QString::number(0));
    ui.hist->setScaledContents(true);
    ui.stackSize->setText(QString::fromLocal8Bit("可撤销：") + QString::number(ImgInstance::GetImgs().size()));

    connect(ui.light, &QSlider::valueChanged, this, [this] {
        if (!m_bOpenImg) {
            ui.light_num->setText(QString::number(0));
            ui.light->setValue(0);
            return;
        }
        ui.light_num->setText(QString::number(ui.light->value()));
        ImgTools::ChangeBrightness(ImgInstance::GetNowImg(), ImgInstance::GetTmp(), ui.light->value());
        ui.img->m_pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetTmp())));
        ui.img->m_scene->addItem(ui.img->m_pixmapItem);
    });
    connect(ui.contrast, &QSlider::valueChanged, this, [this] {
        if (!m_bOpenImg)
            return;
        ui.contrast_num->setText(QString::number((ui.contrast->value() - 10) / 10.0));
        ImgTools::ChangeContrast(ImgInstance::GetNowImg(), ImgInstance::GetTmp(), ui.contrast->value() / 10.0);
        ui.img->m_pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetTmp())));
        ui.img->m_scene->addItem(ui.img->m_pixmapItem);
    });
}

FPHOTO::~FPHOTO() {}

void FPHOTO::SetMenu() {
    QFont font;
    font.setFamily(QString::fromLocal8Bit("微软雅黑 Light"));
    font.setPixelSize(16);

    ui.statusBar->setFont(font);

    QMenu *fileMenu = ui.menuBar->addMenu(QString::fromLocal8Bit("文件"));
    fileMenu->setFont(font);
    fileMenu->addAction(QString::fromLocal8Bit("打开"), this, SLOT(OpenFile()))->setShortcut(QKeySequence::Open);
    fileMenu->addAction(QString::fromLocal8Bit("新建"), this, SLOT(NewFile()))->setShortcut(QKeySequence::New);
    fileMenu->addAction(QString::fromLocal8Bit("保存"), this, SLOT(SaveFile()))->setShortcut(QKeySequence::Save);
    fileMenu->addAction(QString::fromLocal8Bit("退出"), this, SLOT(CloseWindow()))->setShortcut(QKeySequence::Close);

    QMenu *editMenu = ui.menuBar->addMenu(QString::fromLocal8Bit("编辑"));
    editMenu->setFont(font);
    editMenu->addAction(QString::fromLocal8Bit("撤销"), this, SLOT(Undo()))->setShortcut(QKeySequence::Undo);

    QMenu *basicMenu = editMenu->addMenu(QString::fromLocal8Bit("图像基本编辑"));
    basicMenu->setFont(font);
    basicMenu->addAction(QString::fromLocal8Bit("图像旋转"), this, SLOT(Rotate()));
    QMenu *flipMenu = basicMenu->addMenu(QString::fromLocal8Bit("图像翻转"));
    flipMenu->setFont(font);
    flipMenu->addAction(QString::fromLocal8Bit("水平翻转"), this, SLOT(HorizontalFlip()));
    flipMenu->addAction(QString::fromLocal8Bit("垂直翻转"), this, SLOT(VerticalFlip()));
    basicMenu->addAction(QString::fromLocal8Bit("图像反色"), this, SLOT(Inversion()));
    basicMenu->addAction(QString::fromLocal8Bit("图像灰度化"), this, SLOT(Grayscale()));
    basicMenu->addAction(QString::fromLocal8Bit("图像裁剪"), this, SLOT(Crop()));
    basicMenu->addAction(QString::fromLocal8Bit("图像直方图统计"), this, SLOT(DrawImgHistogram()));
    basicMenu->addAction(QString::fromLocal8Bit("图像直方图均衡"), this, SLOT(ImgHistEqualization()));

    QMenu *adjustMenu = ui.menuBar->addMenu(QString::fromLocal8Bit("调节"));
    adjustMenu->setFont(font);
    adjustMenu->addAction(QString::fromLocal8Bit("图像大小调节"), this, SLOT(Resize()));
    // adjustMenu->addAction(QString::fromLocal8Bit("色彩调节"), this, SLOT());

    QMenu *filterMenu = ui.menuBar->addMenu(QString::fromLocal8Bit("图像滤波"));
    filterMenu->setFont(font);
    QMenu *high = filterMenu->addMenu(QString::fromLocal8Bit("高通滤波"));
    high->setFont(font);
    high->addAction(QString::fromLocal8Bit("拉普拉斯锐化"), this, SLOT(LaplaceFilter()));
    QMenu *low = filterMenu->addMenu(QString::fromLocal8Bit("低通滤波"));
    low->setFont(font);
    low->addAction(QString::fromLocal8Bit("算术平均滤波"), this, SLOT(ArithmeticAverageFilter()));
    low->addAction(QString::fromLocal8Bit("几何平均滤波"), this, SLOT(GeometricAverageFilter()));
    low->addAction(QString::fromLocal8Bit("谐波均值滤波"), this, SLOT(HarmonicMeanFilter()));
    low->addAction(QString::fromLocal8Bit("中值滤波"), this, SLOT(MedianFilter()));
    low->addAction(QString::fromLocal8Bit("最小值滤波"), this, SLOT(MinFilter()));
    low->addAction(QString::fromLocal8Bit("最大值滤波"), this, SLOT(MaxFilter()));

    QMenu *morphologyMenu = ui.menuBar->addMenu(QString::fromLocal8Bit("图像形态学处理"));
    morphologyMenu->setFont(font);
    morphologyMenu->addAction(QString::fromLocal8Bit("膨胀/腐蚀"), this, SLOT(DilationOrErosion()));
    morphologyMenu->addAction(QString::fromLocal8Bit("开运算/闭运算"), this, SLOT(OpeningOrClosing()));
    morphologyMenu->addAction(QString::fromLocal8Bit("顶帽变换/底帽变换"), this, SLOT(TopOrBottomHatTrans()));

    QMenu *detectAndSegMenu = ui.menuBar->addMenu(QString::fromLocal8Bit("图像分割与检测"));
    detectAndSegMenu->setFont(font);

    QMenu *segmentMenu = detectAndSegMenu->addMenu(QString::fromLocal8Bit("分割"));
    segmentMenu->setFont(font);
    QMenu *thsegmentMenu = segmentMenu->addMenu(QString::fromLocal8Bit("区域阈值分割"));
    thsegmentMenu->setFont(font);
    thsegmentMenu->addAction(QString::fromLocal8Bit("指定阈值分割"), this, SLOT(ThresholdSegment()));
    thsegmentMenu->addAction(QString::fromLocal8Bit("全局自动阈值分割"), this, SLOT(IterativeThresholdSegment()));
    thsegmentMenu->addAction(QString::fromLocal8Bit("OTSU阈值分割"), this, SLOT(OTSUThresholdSegment()));
    segmentMenu->addAction(QString::fromLocal8Bit("区域生长分割"), this, SLOT(RegionGrowth()));
    QMenu *detectMenu = detectAndSegMenu->addMenu(QString::fromLocal8Bit("检测"));
    detectMenu->setFont(font);
    detectMenu->addAction(QString::fromLocal8Bit("点检测"), this, SLOT(PointDetect()));
    detectMenu->addAction(QString::fromLocal8Bit("线检测"), this, SLOT(LineDetect()));
    detectMenu->addAction(QString::fromLocal8Bit("边缘检测"), this, SLOT(EdgeDetect()));

    ui.menuBar->addAction(QString::fromLocal8Bit("关于"), this, [this] {
        QMessageBox::about(
            this, QString::fromLocal8Bit("关于"), QString::fromLocal8Bit("FPHOTO\t作者: 风信梓Fingsinz\n"));
    });

    connect(ui.light, &QSlider::sliderReleased, this, &FPHOTO::ChangeBrightness);
    connect(ui.contrast, &QSlider::sliderReleased, this, &FPHOTO::ChangeContrast);

    connect(ui.paint, &QComboBox::currentTextChanged, this, &FPHOTO::PaintInImg);
}

void FPHOTO::PostProcess() {
    ui.img->m_pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    ui.img->m_scene->addItem(ui.img->m_pixmapItem);
    ui.img->setScene(ui.img->m_scene);

    ui.stackSize->setText(QString::fromLocal8Bit("可撤销：") + QString::number(ImgInstance::GetImgs().size()));
    ui.imginfo->setText(
        QString::fromLocal8Bit("图片信息：") + QString::number(ImgInstance::GetNowImg().rows) +
        QString::fromLocal8Bit("×") + QString::number(ImgInstance::GetNowImg().cols) +
        QString::fromLocal8Bit("，通道数：") + QString::number(ImgInstance::GetNowImg().channels()));
}

void FPHOTO::CloseWindow() {
    QApplication *app;
    app->exit(0);
}

void FPHOTO::OpenFile() {
    ImgInstance::GetNowImg().release();
    m_bOpenImg = false;
    QString filename = QFileDialog::getOpenFileName(
        this, QString::fromLocal8Bit("选择图像文件"), "", tr("*.jpg *.png *.tif *.tiff *.bmp"));
    if (filename.isEmpty()) {
        // PostProcess();
        // undo();
        return;
    }

    ImgInstance::GetNowImg() = cv::imread(filename.toLocal8Bit().toStdString());

    if (!ImgInstance::GetNowImg().empty()) {
        m_bOpenImg = true;
        ImgInstance::KeepTrace(ImgInstance::GetNowImg());
        ui.info->append(QString::fromLocal8Bit("打开文件"));
        PostProcess();
    } else {
        QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("打开图片失败"));
    }
}

void FPHOTO::NewFile() {
    m_bOpenImg = true;
    ImgInstance::GetNowImg() = cv::Mat(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));
    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    ui.info->append(QString::fromLocal8Bit("新建文件"));
    PostProcess();
}

void FPHOTO::SaveFile() {
    if (!m_bOpenImg or ImgInstance::GetNowImg().empty())
        return;
    QString filename = QFileDialog::getSaveFileName(
        this, QString::fromLocal8Bit("保存图像文件"), "", tr("*.jpg *.png *.tif *.tiff *.bmp"));
    if (filename.isEmpty())
        return;
    cv::imwrite(filename.toLocal8Bit().toStdString(), ImgInstance::GetNowImg());
    ui.info->append(QString::fromLocal8Bit("保存文件"));
    ui.statusBar->showMessage(QString::fromLocal8Bit("已保存"));
}

void FPHOTO::Undo() {
    if (ImgInstance::GetImgs().empty()) {
        ImgInstance::GetNowImg().release();
        return;
    }

    ImgInstance::GetNowImg() = ImgInstance::GetImgs().back();

    ImgInstance::GetImgs().pop_back();
    ui.img->m_pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    ui.img->m_scene->addItem(ui.img->m_pixmapItem);
    ui.info->append(QString::fromLocal8Bit("撤销动作"));
    ui.stackSize->setText(QString::fromLocal8Bit("可撤销：") + QString::number(ImgInstance::GetImgs().size()));
    ui.statusBar->showMessage(QString::fromLocal8Bit("已撤销"));
    ui.imginfo->setText(
        QString::fromLocal8Bit("图片信息：") + QString::number(ImgInstance::GetNowImg().rows) +
        QString::fromLocal8Bit("×") + QString::number(ImgInstance::GetNowImg().cols) +
        QString::fromLocal8Bit("，通道数：") + QString::number(ImgInstance::GetNowImg().channels()));
    if (ImgInstance::GetImgs().empty()) {
        ImgInstance::GetNowImg().release();
        ui.img->setScene(nullptr);
        m_bOpenImg = false;
        ui.info->append(QString::fromLocal8Bit("撤销动作"));
        ui.stackSize->setText(QString::fromLocal8Bit("可撤销：") + QString::number(ImgInstance::GetImgs().size()));
        ui.statusBar->showMessage(QString::fromLocal8Bit("已撤销"));
        ui.imginfo->clear();
    }
}

void FPHOTO::ChangeBrightness() {
    if (!m_bOpenImg)
        return;
    int value = ui.light->value();

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    // 功能函数
    ImgTools::ChangeBrightness(ImgInstance::GetNowImg(), ImgInstance::GetNowImg(), value);

    // 后置处理
    ui.light->setValue(0);
    ui.info->append(QString::fromLocal8Bit("图片亮度调节"));
    PostProcess();
}

void FPHOTO::ChangeContrast() {
    if (!m_bOpenImg)
        return;
    double value = ui.contrast->value() / 10.0;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    // 功能函数
    ImgTools::ChangeContrast(ImgInstance::GetNowImg(), ImgInstance::GetNowImg(), value);

    // 后置处理
    ui.contrast->setValue(10);
    ui.info->append(QString::fromLocal8Bit("图片对比度调节"));
    PostProcess();
}

void FPHOTO::Resize() {
    if (!m_bOpenImg)
        return;

    QFont font;
    font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(12);
    QDialog dlg(this);
    dlg.setWindowTitle(QString::fromLocal8Bit("缩放")), dlg.setFont(font), dlg.setFixedSize(600, 600);
    QFormLayout form(&dlg);

    QGraphicsScene *showImg = new QGraphicsScene(&dlg);
    showImg->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    QGraphicsView *gv = new QGraphicsView(&dlg);
    gv->setScene(showImg);
    form.addRow(gv);

    QSlider *slider = new QSlider(Qt::Horizontal, &dlg);
    form.addRow(slider);
    slider->setMinimum(1), slider->setMaximum(100), slider->setValue(10);
    QLabel *label = new QLabel(QString::number(1), &dlg);
    form.addRow(label, slider);
    QLabel *info = new QLabel(
        QString::fromLocal8Bit("图像大小：") + QString::number((int)ImgInstance::GetNowImg().rows) +
            QString::fromLocal8Bit("×") + QString::number((int)ImgInstance::GetNowImg().rows),
        &dlg);
    form.addRow(info);

    connect(slider, &QSlider::valueChanged, &dlg, [slider, label, info] {
        double factor = slider->value() / 10.0;
        label->setText(QString::number(factor));
        info->setText(
            QString::fromLocal8Bit("图像大小：") + QString::number((int)ImgInstance::GetNowImg().rows * factor) +
            QString::fromLocal8Bit("×") + QString::number((int)ImgInstance::GetNowImg().rows * factor));
    });
    connect(slider, &QSlider::sliderReleased, &dlg, [slider, showImg] {
        showImg->clear();
        double factor = slider->value() / 10.0;
        ImgTools::Resize(ImgInstance::GetNowImg(), ImgInstance::GetTmp(), factor);
        showImg->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetTmp())));
    });

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Rejected)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    ImgInstance::GetNowImg() = ImgInstance::GetTmp().clone();

    // 后置处理
    ui.info->append(QString::fromLocal8Bit("图片缩放"));
    PostProcess();
}

void FPHOTO::Rotate() {
    if (!m_bOpenImg)
        return;

    double angle =
        QInputDialog::getDouble(this, QString::fromLocal8Bit("旋转"), QString::fromLocal8Bit("旋转角度"), 0, -360, 360);
    if (angle == 0)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    // 功能函数
    ImgTools::Rotate(ImgInstance::GetNowImg(), ImgInstance::GetNowImg(), angle);

    // 后置处理
    ui.info->append(QString::fromLocal8Bit("图片旋转") + QString::number(angle) + QString::fromLocal8Bit("°"));
    PostProcess();
}

void FPHOTO::HorizontalFlip() {
    if (!m_bOpenImg)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    // 功能函数
    ImgTools::HorizontalFlip(ImgInstance::GetNowImg(), ImgInstance::GetTmp());

    ImgInstance::GetNowImg() = ImgInstance::GetTmp().clone();
    ImgInstance::GetTmp().release();

    // 后置处理
    ui.info->append(QString::fromLocal8Bit("图片水平翻转"));
    PostProcess();
}

void FPHOTO::VerticalFlip() {
    if (!m_bOpenImg)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    // 功能函数
    ImgTools::VerticalFlip(ImgInstance::GetNowImg(), ImgInstance::GetTmp());

    ImgInstance::GetNowImg() = ImgInstance::GetTmp().clone();
    ImgInstance::GetTmp().release();

    // 后置处理
    ui.info->append(QString::fromLocal8Bit("图片垂直翻转"));
    PostProcess();
}

void FPHOTO::Inversion() {
    if (!m_bOpenImg)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    // 功能函数
    ImgTools::Inversion(ImgInstance::GetNowImg(), ImgInstance::GetNowImg());

    // 后置处理
    ui.info->append(QString::fromLocal8Bit("图片反色"));
    PostProcess();
}

void FPHOTO::Grayscale() {
    if (!m_bOpenImg or ImgInstance::GetNowImg().channels() == 1)
        return;

    bool ok;
    QString select = QInputDialog::getItem(
        this,
        QString::fromLocal8Bit("灰度化保存形式"),
        QString::fromLocal8Bit(""),
        QStringList() << QString::fromLocal8Bit("以单通道保存") << QString::fromLocal8Bit("以三通道保存"),
        0,
        false,
        &ok);

    bool m_iType = select == QString::fromLocal8Bit("以单通道保存");
    if (!ok)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    // 功能函数
    ImgTools::Grayscale(ImgInstance::GetNowImg(), ImgInstance::GetNowImg(), m_iType);

    // 后置处理
    ui.info->append(QString::fromLocal8Bit("图片灰度化"));
    PostProcess();
}

void FPHOTO::Crop() {
    if (!m_bOpenImg)
        return;
    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    ui.img->m_iType = 2, ui.img->m_color = cv::Scalar(255, 255, 255), ui.img->m_iThickness = 1, ui.img->m_bIsCrop = 1;

    // 后置处理
    ui.info->append(QString::fromLocal8Bit("图片裁剪"));
    PostProcess();
}

void FPHOTO::PaintInImg() {
    if (!m_bOpenImg) {
        ui.paint->setCurrentIndex(0);
        return;
    }

    int _type = ui.paint->currentIndex();

    if (_type == 0) {
        ui.img->m_iType = -1, ui.img->m_bIsPaint = false, ui.img->m_bIsCrop = false, ui.img->m_bIsTextMode = false;
        return;
    } else if (_type == 1) {
        ui.img->m_iType = 1, ui.img->m_bIsPaint = true, ui.img->m_bIsCrop = false, ui.img->m_bIsTextMode = false;
        QColor m_color = QColorDialog::getColor(QColor(255, 0, 0), this, QString::fromLocal8Bit("请选择颜色"));
        if (!m_color.isValid())
            return;
        int thickness =
            QInputDialog::getInt(this, QString::fromLocal8Bit("请选择线宽"), QString::fromLocal8Bit("线宽"), 1, 1, 100);
        ImgInstance::KeepTrace(ImgInstance::GetNowImg());

        cv::Scalar color = cv::Scalar(m_color.blue(), m_color.green(), m_color.red());
        ui.img->m_color = color, ui.img->m_iThickness = thickness;
        ui.info->append(QString::fromLocal8Bit("添加直线"));
    } else if (_type == 2) {
        ui.img->m_iType = 2, ui.img->m_bIsPaint = true, ui.img->m_bIsCrop = false, ui.img->m_bIsTextMode = false;
        QColor m_color = QColorDialog::getColor(QColor(255, 0, 0), this, QString::fromLocal8Bit("请选择颜色"));
        if (!m_color.isValid())
            return;
        int thickness = QInputDialog::getInt(
            this, QString::fromLocal8Bit("请选择矩形框粗细"), QString::fromLocal8Bit("粗细"), 1, 1, 100);
        ImgInstance::KeepTrace(ImgInstance::GetNowImg());

        cv::Scalar color = cv::Scalar(m_color.blue(), m_color.green(), m_color.red());
        ui.img->m_color = color, ui.img->m_iThickness = thickness;
        ui.info->append(QString::fromLocal8Bit("添加矩形"));
    } else if (_type == 3) {
        ui.img->m_iType = 3, ui.img->m_bIsPaint = true, ui.img->m_bIsCrop = false, ui.img->m_bIsTextMode = false;
        QColor m_color = QColorDialog::getColor(QColor(255, 0, 0), this, QString::fromLocal8Bit("请选择颜色"));
        if (!m_color.isValid())
            return;
        cv::Scalar color = cv::Scalar(m_color.blue(), m_color.green(), m_color.red());
        int thickness = QInputDialog::getInt(
            this, QString::fromLocal8Bit("请选择圆形框粗细"), QString::fromLocal8Bit("粗细"), 1, 1, 100);
        ImgInstance::KeepTrace(ImgInstance::GetNowImg());

        ui.img->m_color = color, ui.img->m_iThickness = thickness;
        ui.info->append(QString::fromLocal8Bit("添加圆"));
    } else if (_type == 4) {
        ui.img->m_iType = 0, ui.img->m_bIsPaint = true, ui.img->m_bIsCrop = false, ui.img->m_bIsTextMode = false;
        QColor m_color = QColorDialog::getColor(QColor(255, 0, 0), this, QString::fromLocal8Bit("请选择颜色"));
        if (!m_color.isValid())
            return;
        cv::Scalar color = cv::Scalar(m_color.blue(), m_color.green(), m_color.red());
        int thickness = QInputDialog::getInt(
            this, QString::fromLocal8Bit("请输入画笔大小"), QString::fromLocal8Bit("大小"), 1, 1, 100);
        ImgInstance::KeepTrace(ImgInstance::GetNowImg());

        ui.img->m_color = color, ui.img->m_iThickness = thickness;
        ui.info->append(QString::fromLocal8Bit("添加涂鸦"));
    } else if (_type == 5) {
        ui.img->m_iType = -2, ui.img->m_bIsPaint = false, ui.img->m_bIsCrop = false, ui.img->m_bIsTextMode = true;
        QColor m_color = QColorDialog::getColor(QColor(255, 0, 0), this, QString::fromLocal8Bit("请选择颜色"));
        if (!m_color.isValid())
            return;
        cv::Scalar color = cv::Scalar(m_color.blue(), m_color.green(), m_color.red());

        QFont font;
        font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
        QDialog dlg(this);
        QFormLayout form(&dlg);
        dlg.setFont(font);

        QSpinBox *sp1 = new QSpinBox(&dlg);
        sp1->setMinimum(1), sp1->setMaximum(200), sp1->setValue(1);
        QString tips1 = QString::fromLocal8Bit("请输入字体大小：");
        form.addRow(tips1, sp1);

        QLineEdit *le = new QLineEdit(&dlg);
        QString tips3 = QString::fromLocal8Bit("请输入文字：");
        form.addRow(tips3, le);

        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
        form.addRow(&buttonBox);
        QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
        QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

        if (dlg.exec() == QDialog::Rejected)
            return;
        ImgInstance::KeepTrace(ImgInstance::GetNowImg());

        int size = sp1->value();
        QString t = le->text();
        ui.img->m_color = color, ui.img->m_iThickness = size, ui.img->m_strText = t.toLocal8Bit().toStdString();
        ui.info->append(QString::fromLocal8Bit("添加文字"));
    }

    PostProcess();
}

void FPHOTO::DrawImgHistogram() {
    if (!m_bOpenImg)
        return;

    // 功能函数
    ImgTools::DrawHistogram(ImgInstance::GetNowImg(), ImgInstance::GetHist());

    // 后置处理
    ui.hist->setPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetHist())).scaled(ui.hist->size()));
}

void FPHOTO::ImgHistEqualization() {
    if (!m_bOpenImg)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    // 功能函数
    ImgTools::HistEqualization(ImgInstance::GetNowImg(), ImgInstance::GetNowImg(), ImgInstance::GetHist());

    // 后置处理
    ui.hist->setPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetHist())).scaled(ui.hist->size()));
    ui.info->append(QString::fromLocal8Bit("直方图均衡化"));
    PostProcess();
}

void FPHOTO::ArithmeticAverageFilter() {
    if (!m_bOpenImg)
        return;

    int ksize = 0;

    QFont font;
    font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
    QDialog dlg(this);
    dlg.setWindowTitle(QString::fromLocal8Bit("算术平均滤波"));
    QFormLayout form(&dlg);
    dlg.setFont(font);

    QGraphicsView *gv = new QGraphicsView(&dlg);
    QGraphicsScene *gs = new QGraphicsScene(&dlg);
    gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    gv->setScene(gs);
    form.addRow(gv);

    QLineEdit *le = new QLineEdit(&dlg);
    QLabel *lb = new QLabel(&dlg);
    lb->setText(QString::fromLocal8Bit("输入滤波核大小："));
    form.addRow(lb, le);

    connect(le, &QLineEdit::editingFinished, &dlg, [le, lb, gs, &ksize] {
        bool ok;
        ksize = le->text().toInt(&ok);
        if (!ok or ksize == 0)
            return;
        ksize = ksize % 2 ? ksize : ksize - 1;
        gs->clear();
        ImgTools::ArithmeticAverageFilter(ImgInstance::GetNowImg(), ImgInstance::GetTmp(), ksize);
        gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetTmp())));
    });

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Rejected)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    ImgInstance::GetNowImg() = ImgInstance::GetTmp().clone();

    // 后置处理
    ui.info->append(QString::fromLocal8Bit("图像算术平均滤波：") + QString::number(ksize));
    PostProcess();
}

void FPHOTO::GeometricAverageFilter() {
    if (!m_bOpenImg)
        return;

    int ksize = 0;

    QFont font;
    font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
    QDialog dlg(this);
    dlg.setWindowTitle(QString::fromLocal8Bit("几何平均滤波"));
    QFormLayout form(&dlg);
    dlg.setFont(font);

    QGraphicsView *gv = new QGraphicsView(&dlg);
    QGraphicsScene *gs = new QGraphicsScene(&dlg);
    gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    gv->setScene(gs);
    form.addRow(gv);

    QLineEdit *le = new QLineEdit(&dlg);
    QLabel *lb = new QLabel(&dlg);
    lb->setText(QString::fromLocal8Bit("输入滤波核大小："));
    form.addRow(lb, le);

    connect(le, &QLineEdit::editingFinished, &dlg, [le, lb, gs, &ksize] {
        bool ok;
        ksize = le->text().toInt(&ok);
        if (!ok or ksize == 0)
            return;
        ksize = ksize % 2 ? ksize : ksize - 1;
        gs->clear();
        ImgTools::GeometricAverageFilter(ImgInstance::GetNowImg(), ImgInstance::GetTmp(), ksize);
        gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetTmp())));
    });

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Rejected)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    ImgInstance::GetNowImg() = ImgInstance::GetTmp().clone();

    // 后置处理
    ui.info->append(QString::fromLocal8Bit("图像几何平均滤波：") + QString::number(ksize));
    PostProcess();
}

void FPHOTO::HarmonicMeanFilter() {
    if (!m_bOpenImg)
        return;

    int ksize;

    QFont font;
    font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
    QDialog dlg(this);
    dlg.setWindowTitle(QString::fromLocal8Bit("谐波均值滤波"));
    QFormLayout form(&dlg);
    dlg.setFont(font);

    QGraphicsView *gv = new QGraphicsView(&dlg);
    QGraphicsScene *gs = new QGraphicsScene(&dlg);
    gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    gv->setScene(gs);
    form.addRow(gv);

    QLineEdit *le = new QLineEdit(&dlg);
    QLabel *lb = new QLabel(&dlg);
    lb->setText(QString::fromLocal8Bit("输入滤波核大小："));
    form.addRow(lb, le);

    connect(le, &QLineEdit::editingFinished, &dlg, [le, lb, gs, &ksize] {
        bool ok;
        ksize = le->text().toInt(&ok);
        if (!ok or ksize == 0)
            return;
        ksize = ksize % 2 ? ksize : ksize - 1;
        gs->clear();
        ImgTools::HarmonicMeanFilter(ImgInstance::GetNowImg(), ImgInstance::GetTmp(), ksize);
        gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetTmp())));
    });

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Rejected)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    ImgInstance::GetNowImg() = ImgInstance::GetTmp().clone();

    // 后置处理
    ui.info->append(QString::fromLocal8Bit("图像谐波均值滤波：") + QString::number(ksize));
    PostProcess();
}

void FPHOTO::MedianFilter() {
    if (!m_bOpenImg)
        return;

    int ksize = 0;

    QFont font;
    font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
    QDialog dlg(this);
    dlg.setWindowTitle(QString::fromLocal8Bit("中值滤波"));
    QFormLayout form(&dlg);
    dlg.setFont(font);

    QGraphicsView *gv = new QGraphicsView(&dlg);
    QGraphicsScene *gs = new QGraphicsScene(&dlg);
    gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    gv->setScene(gs);
    form.addRow(gv);

    QLineEdit *le = new QLineEdit(&dlg);
    QLabel *lb = new QLabel(&dlg);
    lb->setText(QString::fromLocal8Bit("输入滤波核大小："));
    form.addRow(lb, le);

    connect(le, &QLineEdit::editingFinished, &dlg, [le, lb, gs, &ksize] {
        bool ok;
        ksize = le->text().toInt(&ok);
        if (!ok or ksize == 0)
            return;
        ksize = ksize % 2 ? ksize : ksize - 1;
        gs->clear();
        ImgTools::MedianFilter(ImgInstance::GetNowImg(), ImgInstance::GetTmp(), ksize);
        gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetTmp())));
    });

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Rejected)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    ImgInstance::GetNowImg() = ImgInstance::GetTmp().clone();

    // 后置处理
    ui.info->append(QString::fromLocal8Bit("图像中值滤波：") + QString::number(ksize));
    PostProcess();
}

void FPHOTO::MinFilter() {
    if (!m_bOpenImg)
        return;

    int ksize = 0;

    QFont font;
    font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
    QDialog dlg(this);
    dlg.setWindowTitle(QString::fromLocal8Bit("最小值滤波"));
    QFormLayout form(&dlg);
    dlg.setFont(font);

    QGraphicsView *gv = new QGraphicsView(&dlg);
    QGraphicsScene *gs = new QGraphicsScene(&dlg);
    gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    gv->setScene(gs);
    form.addRow(gv);

    QLineEdit *le = new QLineEdit(&dlg);
    QLabel *lb = new QLabel(&dlg);
    lb->setText(QString::fromLocal8Bit("输入滤波核大小："));
    form.addRow(lb, le);

    connect(le, &QLineEdit::editingFinished, &dlg, [le, lb, gs, &ksize] {
        bool ok;
        ksize = le->text().toInt(&ok);
        if (!ok or ksize == 0)
            return;
        ksize = ksize % 2 ? ksize : ksize - 1;
        gs->clear();
        ImgTools::MaxMinFilter(ImgInstance::GetNowImg(), ImgInstance::GetTmp(), ksize, 0);
        gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetTmp())));
    });

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Rejected)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    ImgInstance::GetNowImg() = ImgInstance::GetTmp().clone();

    // 后置处理
    ui.info->append(QString::fromLocal8Bit("图像最小值滤波：") + QString::number(ksize));
    PostProcess();
}

void FPHOTO::MaxFilter() {
    if (!m_bOpenImg)
        return;

    int ksize = 0;

    QFont font;
    font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
    QDialog dlg(this);
    dlg.setWindowTitle(QString::fromLocal8Bit("最大值滤波"));
    QFormLayout form(&dlg);
    dlg.setFont(font);

    QGraphicsView *gv = new QGraphicsView(&dlg);
    QGraphicsScene *gs = new QGraphicsScene(&dlg);
    gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    gv->setScene(gs);
    form.addRow(gv);

    QLineEdit *le = new QLineEdit(&dlg);
    QLabel *lb = new QLabel(&dlg);
    lb->setText(QString::fromLocal8Bit("输入滤波核大小："));
    form.addRow(lb, le);

    connect(le, &QLineEdit::editingFinished, &dlg, [le, lb, gs, &ksize] {
        bool ok;
        ksize = le->text().toInt(&ok);
        if (!ok or ksize == 0)
            return;
        ksize = ksize % 2 ? ksize : ksize - 1;
        gs->clear();
        ImgTools::MaxMinFilter(ImgInstance::GetNowImg(), ImgInstance::GetTmp(), ksize, 1);
        gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetTmp())));
    });

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Rejected)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    ImgInstance::GetNowImg() = ImgInstance::GetTmp().clone();

    // 后置处理
    ui.info->append(QString::fromLocal8Bit(" 图像最大值滤波：") + QString::number(ksize));
    PostProcess();
}

void FPHOTO::LaplaceFilter() {
    if (!m_bOpenImg)
        return;

    QFont font;
    font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
    QDialog dlg(this);
    dlg.setWindowTitle(QString::fromLocal8Bit("拉普拉斯高通滤波"));
    QFormLayout form(&dlg);
    dlg.setFont(font);

    QGraphicsView *gv = new QGraphicsView(&dlg);
    QGraphicsScene *gs = new QGraphicsScene(&dlg);
    gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    gv->setScene(gs);
    form.addRow(gv);

    QComboBox *cb = new QComboBox(&dlg);
    cb->addItem(QString::fromLocal8Bit("四邻域锐化"));
    cb->addItem(QString::fromLocal8Bit("八邻域锐化"));
    cb->addItem(QString::fromLocal8Bit("请选择"));
    cb->setCurrentIndex(2);
    form.addRow(cb);

    connect(cb, static_cast<void (QComboBox::*)(int idx)>(&QComboBox::currentIndexChanged), &dlg, [cb, gs](int idx) {
        gs->clear();
        if (idx == 2) {
            gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
            return;
        }
        ImgTools::LaplaceFilter(ImgInstance::GetNowImg(), ImgInstance::GetTmp(), idx);
        gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetTmp())));
    });

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Rejected)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    ImgInstance::GetNowImg() = ImgInstance::GetTmp().clone();

    // 后置处理
    ui.info->append(QString::fromLocal8Bit("图像锐化"));
    PostProcess();
}

void FPHOTO::DilationOrErosion() {
    if (!m_bOpenImg)
        return;

    int op = 0;
    int kernelSize = 0;
    int kernelType = 0;
    int cnt = 0;

    // 执行输入对话
    QFont font;
    font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
    QDialog dlg(this);
    QFormLayout form(&dlg);
    dlg.setFont(font), dlg.setFixedSize(600, 600), dlg.setWindowTitle(QString::fromLocal8Bit("膨胀腐蚀预览"));

    QGraphicsScene *gs = new QGraphicsScene(&dlg);
    gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    QGraphicsView *gv = new QGraphicsView(&dlg);
    gv->setScene(gs);
    form.addRow(gv);

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

    connect(pb, &QPushButton::clicked, &dlg, [gs, cb1, sp1, cb2, sp2, &kernelSize, &kernelType, &op, &cnt]() {
        gs->clear();
        kernelSize = sp1->value();
        kernelType = cb2->currentIndex();
        op = cb1->currentIndex(), cnt = sp2->value();
        ImgTools::DilationOrErosion(ImgInstance::GetNowImg(), ImgInstance::GetTmp(), kernelSize, kernelType, op, cnt);
        gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetTmp())));
    });

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Rejected)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    ImgInstance::GetNowImg() = ImgInstance::GetTmp().clone();

    if (op == 0)
        ui.info->append(QString::fromLocal8Bit("图像膨胀"));
    else if (op == 1)
        ui.info->append(QString::fromLocal8Bit("图像腐蚀"));
    PostProcess();
}

void FPHOTO::OpeningOrClosing() {
    if (!m_bOpenImg)
        return;

    int op = 0;
    int kernelSize = 0;
    int kernelType = 0;
    int cnt = 0;

    // 执行输入对话
    QFont font;
    font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
    QDialog dlg(this);
    QFormLayout form(&dlg);
    dlg.setFont(font), dlg.setFixedSize(600, 600), dlg.setWindowTitle(QString::fromLocal8Bit("开运算闭运算预览"));

    QGraphicsScene *gs = new QGraphicsScene(&dlg);
    gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    QGraphicsView *gv = new QGraphicsView(&dlg);
    gv->setScene(gs);
    form.addRow(gv);

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

    connect(pb, &QPushButton::clicked, &dlg, [gs, cb1, sp1, cb2, sp2, &kernelSize, &kernelType, &op, &cnt]() {
        gs->clear();
        kernelSize = sp1->value();
        kernelType = cb2->currentIndex();
        op = cb1->currentIndex() + 2, cnt = sp2->value();
        ImgTools::OpeningOrClosing(ImgInstance::GetNowImg(), ImgInstance::GetTmp(), kernelSize, kernelType, op, cnt);
        ;
        gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetTmp())));
    });

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Rejected)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    ImgInstance::GetNowImg() = ImgInstance::GetTmp().clone();

    if (op == 2)
        ui.info->append(QString::fromLocal8Bit("图像开运算"));
    else if (op == 3)
        ui.info->append(QString::fromLocal8Bit("图像闭运算"));
    PostProcess();
}

void FPHOTO::TopOrBottomHatTrans() {
    if (!m_bOpenImg)
        return;

    int op = 0;
    int kernelSize = 0;
    int kernelType = 0;
    int cnt = 0;

    // 执行输入对话
    QFont font;
    font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
    QDialog dlg(this);
    QFormLayout form(&dlg);
    dlg.setFont(font), dlg.setFixedSize(600, 600), dlg.setWindowTitle(QString::fromLocal8Bit("顶帽变换底帽变换预览"));

    QGraphicsScene *gs = new QGraphicsScene(&dlg);
    gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    QGraphicsView *gv = new QGraphicsView(&dlg);
    gv->setScene(gs);
    form.addRow(gv);

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

    connect(pb, &QPushButton::clicked, &dlg, [gs, cb1, sp1, cb2, sp2, &kernelSize, &kernelType, &op, &cnt]() {
        gs->clear();
        kernelSize = sp1->value();
        kernelType = cb2->currentIndex();
        op = cb1->currentIndex() + 5, cnt = sp2->value();
        ImgTools::TopOrBottomHatTrans(ImgInstance::GetNowImg(), ImgInstance::GetTmp(), kernelSize, kernelType, op, cnt);
        gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetTmp())));
    });

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Rejected)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    ImgInstance::GetNowImg() = ImgInstance::GetTmp().clone();

    if (op == 5)
        ui.info->append(QString::fromLocal8Bit("图像顶帽变换"));
    else if (op == 6)
        ui.info->append(QString::fromLocal8Bit("图像底帽变换"));
    PostProcess();
}

void FPHOTO::ThresholdSegment() {
    if (!m_bOpenImg)
        return;

    int t = QInputDialog::getInt(
        this, QString::fromLocal8Bit("阈值分割"), QString::fromLocal8Bit("请输入阈值："), 0, 0, 255);

    ImgTools::ThresholdSegment(ImgInstance::GetNowImg(), ImgInstance::GetGray(), t);

    QDialog dlg(this);
    QFormLayout form(&dlg);
    dlg.setFixedSize(420, 400);
    dlg.setWindowTitle(QString::fromLocal8Bit("是否保存覆盖？该图像为单通道图像！"));

    QGraphicsScene *showImg = new QGraphicsScene(&dlg);
    showImg->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetGray())));
    QGraphicsView *gv = new QGraphicsView(&dlg);
    gv->setScene(showImg);
    form.addRow(gv);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        ImgInstance::KeepTrace(ImgInstance::GetNowImg());
        ImgInstance::GetNowImg() = ImgInstance::GetGray().clone();
        ui.info->append(QString::fromLocal8Bit("图像指定阈值分割：") + QString::number(t));
    }

    // 后置处理
    ImgInstance::GetGray().release();
    ui.img->m_pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    ui.img->m_scene->addItem(ui.img->m_pixmapItem);
    ui.stackSize->setText(QString::fromLocal8Bit("可撤销：") + QString::number(ImgInstance::GetImgs().size()));
    ui.imginfo->setText(
        QString::fromLocal8Bit("图片信息：") + QString::number(ImgInstance::GetNowImg().rows) +
        QString::fromLocal8Bit("×") + QString::number(ImgInstance::GetNowImg().cols) +
        QString::fromLocal8Bit("，通道数：") + QString::number(ImgInstance::GetNowImg().channels()));
}

void FPHOTO::IterativeThresholdSegment() {
    if (!m_bOpenImg)
        return;

    int t;
    ImgTools::IterativeThresholdSegment(ImgInstance::GetNowImg(), ImgInstance::GetGray(), t);

    QDialog dlg(this);
    QFormLayout form(&dlg);
    dlg.setFixedSize(420, 400);
    dlg.setWindowTitle(QString::fromLocal8Bit("是否保存覆盖？该图像为单通道图像！"));

    QGraphicsScene *showImg = new QGraphicsScene(&dlg);
    showImg->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetGray())));
    QGraphicsView *gv = new QGraphicsView(&dlg);
    gv->setScene(showImg);
    form.addRow(gv);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        ImgInstance::KeepTrace(ImgInstance::GetNowImg());
        ImgInstance::GetNowImg() = ImgInstance::GetGray().clone();
        ui.info->append(QString::fromLocal8Bit("图像全局自动阈值分割：") + QString::number(t));
    }

    // 后置处理
    ImgInstance::GetGray().release();
    ui.img->m_pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    ui.img->m_scene->addItem(ui.img->m_pixmapItem);
    ui.stackSize->setText(QString::fromLocal8Bit("可撤销：") + QString::number(ImgInstance::GetImgs().size()));
    ui.imginfo->setText(
        QString::fromLocal8Bit("图片信息：") + QString::number(ImgInstance::GetNowImg().rows) +
        QString::fromLocal8Bit("×") + QString::number(ImgInstance::GetNowImg().cols) +
        QString::fromLocal8Bit("，通道数：") + QString::number(ImgInstance::GetNowImg().channels()));
}

void FPHOTO::OTSUThresholdSegment() {
    if (!m_bOpenImg)
        return;

    int t;
    ImgTools::OTSUThresholdSegment(ImgInstance::GetNowImg(), ImgInstance::GetGray(), t);

    QDialog dlg(this);
    QFormLayout form(&dlg);
    dlg.setFixedSize(420, 400);
    dlg.setWindowTitle(QString::fromLocal8Bit("是否保存覆盖？该图像为单通道图像！"));

    QGraphicsScene *showImg = new QGraphicsScene(&dlg);
    showImg->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetGray())));
    QGraphicsView *gv = new QGraphicsView(&dlg);
    gv->setScene(showImg);
    form.addRow(gv);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        ImgInstance::KeepTrace(ImgInstance::GetNowImg());
        ImgInstance::GetNowImg() = ImgInstance::GetGray().clone();
        ui.info->append(QString::fromLocal8Bit("图像OTSU阈值分割：") + QString::number(t));
    }

    // 后置处理
    ImgInstance::GetGray().release();
    ui.img->m_pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    ui.img->m_scene->addItem(ui.img->m_pixmapItem);
    ui.stackSize->setText(QString::fromLocal8Bit("可撤销：") + QString::number(ImgInstance::GetImgs().size()));
    ui.imginfo->setText(
        QString::fromLocal8Bit("图片信息：") + QString::number(ImgInstance::GetNowImg().rows) +
        QString::fromLocal8Bit("×") + QString::number(ImgInstance::GetNowImg().cols) +
        QString::fromLocal8Bit("，通道数：") + QString::number(ImgInstance::GetNowImg().channels()));
}

void FPHOTO::RegionGrowth() {
    if (!m_bOpenImg)
        return;
    if (ImgInstance::GetNowImg().channels() != 1) {
        QMessageBox::warning(
            this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("图像区域生长分割须在单通道下进行!"));
        return;
    }

    int T = QInputDialog::getInt(
        this, QString::fromLocal8Bit("图像区域生长分割"), QString::fromLocal8Bit("请输入区域生长阈值:"));
    ImgTools::RegionGrowth(ImgInstance::GetNowImg(), ImgInstance::GetGray(), T);

    QDialog dlg(this);
    QFormLayout form(&dlg);
    dlg.setFixedSize(400, 400);
    dlg.setWindowTitle(QString::fromLocal8Bit("是否保存覆盖？该图像为单通道图像！"));

    QGraphicsScene *showImg = new QGraphicsScene(&dlg);
    showImg->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetGray())));
    QGraphicsView *gv = new QGraphicsView(&dlg);
    gv->setScene(showImg);
    form.addRow(gv);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        ImgInstance::KeepTrace(ImgInstance::GetNowImg());
        ImgInstance::GetNowImg() = ImgInstance::GetGray().clone();
        ui.info->append(QString::fromLocal8Bit("图像区域生长分割"));
    }

    // 后置处理
    ImgInstance::GetGray().release();
    ui.img->m_pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    ui.img->m_scene->addItem(ui.img->m_pixmapItem);
    ui.stackSize->setText(QString::fromLocal8Bit("可撤销：") + QString::number(ImgInstance::GetImgs().size()));
    ui.imginfo->setText(
        QString::fromLocal8Bit("图片信息：") + QString::number(ImgInstance::GetNowImg().rows) +
        QString::fromLocal8Bit("×") + QString::number(ImgInstance::GetNowImg().cols) +
        QString::fromLocal8Bit("，通道数：") + QString::number(ImgInstance::GetNowImg().channels()));
}

void FPHOTO::PointDetect() {
    if (!m_bOpenImg)
        return;

    QFont font;
    font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
    QDialog dlg(this);
    dlg.setWindowTitle(QString::fromLocal8Bit("点检测")), dlg.setFont(font);
    QFormLayout form(&dlg);

    QGraphicsScene *gs = new QGraphicsScene(&dlg);
    gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    QGraphicsView *gv = new QGraphicsView(&dlg);
    gv->setScene(gs);
    form.addRow(gv);

    QComboBox *cb = new QComboBox(&dlg);
    cb->addItem(QString::fromLocal8Bit("四邻域检测"));
    cb->addItem(QString::fromLocal8Bit("八邻域检测"));
    cb->addItem(QString::fromLocal8Bit("请选择"));
    cb->setCurrentIndex(2);
    form.addRow(cb);

    connect(cb, static_cast<void (QComboBox::*)(int idx)>(&QComboBox::currentIndexChanged), &dlg, [cb, gs](int idx) {
        gs->clear();
        if (idx == 2) {
            gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
            return;
        }
        ImgTools::PointDetect(ImgInstance::GetNowImg(), ImgInstance::GetTmp(), idx);
        gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetTmp())));
    });

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Rejected)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    ImgInstance::GetNowImg() = ImgInstance::GetTmp().clone();

    // 后置处理
    ui.info->append(QString::fromLocal8Bit("图像孤立点检测"));
    PostProcess();
}

void FPHOTO::LineDetect() {
    if (!m_bOpenImg)
        return;

    QFont font;
    font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
    QDialog dlg(this);
    dlg.setWindowTitle(QString::fromLocal8Bit("线检测")), dlg.setFont(font);
    QFormLayout form(&dlg);

    ImgTools::LineDetect(ImgInstance::GetNowImg(), ImgInstance::GetTmp());
    QGraphicsScene *gs = new QGraphicsScene(&dlg);
    gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetTmp())));
    QGraphicsView *gv = new QGraphicsView(&dlg);
    gv->setScene(gs);
    form.addRow(gv);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Rejected)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    ImgInstance::GetNowImg() = ImgInstance::GetTmp().clone();

    // 后置处理
    ui.info->append(QString::fromLocal8Bit("图像线检测"));
    PostProcess();
}

void FPHOTO::EdgeDetect() {
    if (!m_bOpenImg)
        return;

    QFont font;
    font.setFamily(QString::fromLocal8Bit("微软雅黑 Light")), font.setPixelSize(16);
    QDialog dlg(this);
    dlg.setWindowTitle(QString::fromLocal8Bit("线检测")), dlg.setFont(font);
    QFormLayout form(&dlg);

    QGraphicsScene *gs = new QGraphicsScene(&dlg);
    gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    QGraphicsView *gv = new QGraphicsView(&dlg);
    gv->setScene(gs);
    form.addRow(gv);

    QComboBox *cb = new QComboBox(&dlg);
    cb->addItem(QString::fromLocal8Bit("Sobel算子边缘检测"));
    cb->addItem(QString::fromLocal8Bit("Robert算子边缘检测"));
    cb->addItem(QString::fromLocal8Bit("Prewitt算子边缘检测"));
    cb->addItem(QString::fromLocal8Bit("Kirsch算子边缘检测"));
    cb->addItem(QString::fromLocal8Bit("高斯拉普拉斯算子边缘检测"));
    cb->addItem(QString::fromLocal8Bit("Canny边缘检测"));
    cb->addItem(QString::fromLocal8Bit("请选择"));
    cb->setCurrentIndex(6);
    form.addRow(cb);

    connect(cb, static_cast<void (QComboBox::*)(int idx)>(&QComboBox::currentIndexChanged), &dlg, [cb, gs](int idx) {
        gs->clear();
        if (idx == 6) {
            gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
            return;
        }
        switch (idx) {
        case 0:
            ImgTools::Sobel(ImgInstance::GetNowImg(), ImgInstance::GetTmp());
            break;
        case 1:
            ImgTools::Roberts(ImgInstance::GetNowImg(), ImgInstance::GetTmp());
            break;
        case 2:
            ImgTools::Prewitt(ImgInstance::GetNowImg(), ImgInstance::GetTmp());
            break;
        case 3:
            ImgTools::Kirsch(ImgInstance::GetNowImg(), ImgInstance::GetTmp());
            break;
        case 4:
            ImgTools::LOG(ImgInstance::GetNowImg(), ImgInstance::GetTmp());
            break;
        case 5:
            cv::Canny(ImgInstance::GetNowImg(), ImgInstance::GetTmp(), 100, 150);
            break;
        default:
            break;
        }
        gs->addPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetTmp())));
    });

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Rejected)
        return;

    ImgInstance::KeepTrace(ImgInstance::GetNowImg());
    ImgInstance::GetNowImg() = ImgInstance::GetTmp().clone();

    // 后置处理
    ui.info->append(QString::fromLocal8Bit("图像边缘检测"));
    PostProcess();
}