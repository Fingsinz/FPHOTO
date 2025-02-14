#pragma once

#include <QtWidgets/QMainWindow>

#include "qgraphicsscene.h"
#include "ui_FPHOTO.h"

#include <opencv2/opencv.hpp>

#include "ImgInstance.h"
#include "ImgProcess.h"
#include "PhotoShow.h"

class FPHOTO : public QMainWindow {
    Q_OBJECT

public:
    FPHOTO(QWidget *parent = nullptr);
    ~FPHOTO();

    QGraphicsView *gs; // QGraphicsView 窗口

    /*
     * @brief 设置菜单
     * @return void
     */
    void SetMenu();

    /*
     * @brief 后处理
     * @return void
     */
    void PostProcess();

    bool m_bOpenImg = false; // 是否打开图片

public Q_SLOTS:

    /*
     * @brief 打开文件
     * @return void
     */
    void OpenFile();

    /*
     * @brief 新建文件
     * @return void
     */
    void NewFile();

    /*
     * @brief 保存文件
     * @return void
     */
    void SaveFile();

    /*
     * @brief 撤销动作
     * @return void
     */
    void Undo();

    /*
     * @brief 关闭主窗口
     * @return void
     */
    void CloseWindow();

    void ChangeBrightness(); // 修改亮度值
    void ChangeContrast();   // 修改对比度
    void Resize();           // 缩放图像
    void Rotate();           // 旋转图像
    void HorizontalFlip();   // 水平翻转
    void VerticalFlip();     // 垂直翻转
    void Inversion();        // 图片反色
    void Grayscale();        // 图片灰度化
    void Crop();             // 裁剪
    void PaintInImg();       // 图像上绘图

    void DrawImgHistogram();    // 绘制图像直方图
    void ImgHistEqualization(); // 直方图均衡化

    void ArithmeticAverageFilter(); // 算术平均滤波
    void GeometricAverageFilter();  // 几何平均滤波
    void HarmonicMeanFilter();      // 谐波均值滤波
    void MedianFilter();            // 中值滤波
    void MinFilter();               // 最小值滤波
    void MaxFilter();               // 最大值滤波
    void LaplaceFilter();           // 拉普拉斯滤波

    void DilationOrErosion();   // 膨胀或腐蚀
    void OpeningOrClosing();    // 开运算或闭运算
    void TopOrBottomHatTrans(); // 顶帽或底帽变换

    void ThresholdSegment();          // 阈值分割
    void IterativeThresholdSegment(); // 自动全局阈值分割
    void OTSUThresholdSegment();      // OTSU 阈值分割
    void RegionGrowth();              // 区域生长

    void PointDetect(); // 点检测
    void LineDetect();  // 线检测
    void EdgeDetect();  // 边缘检测

private:
    Ui::FPHOTOClass ui;
};
