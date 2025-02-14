#pragma once

#include "QGraphicsPixmapItem"
#include "qevent.h"
#include "qmouseeventtransition.h"
#include <QGraphicsView>
#include <opencv2/opencv.hpp>

class PhotoShow : public QGraphicsView {
public:
    explicit PhotoShow(QWidget *parent = 0);

    /**
     * @brief 处理鼠标按压事件。
     * @param event 鼠标事件。
     * @return void
     */
    void mousePressEvent(QMouseEvent *event);

    /**
     * @brief 处理鼠标移动事件。
     * @param event 鼠标事件。
     * @return void
     */
    void mouseMoveEvent(QMouseEvent *event);

    /**
     * @brief 处理鼠标释放事件。
     * @param event 鼠标事件。
     * @return void
     */
    void mouseReleaseEvent(QMouseEvent *event);

public:
    QGraphicsPixmapItem *m_pixmapItem; // 用于显示图像的QGraphicsPixmapItem。
    QGraphicsScene *m_scene;           // 用于显示图像的QGraphicsScene。
    QPointF m_point;                   // 当前鼠标位置
    QPointF m_prePoint;                // 上一次鼠标位置
    cv::Rect m_cropBox;                // 要裁剪的图像区域
    cv::Scalar m_color;                // 绘图工具的颜色
    int m_iThickness;                  // 绘图工具的粗细
    int m_iType;                       // 正在使用的绘图工具的类型
    bool m_bIsPaint;                   // 是否绘图模式
    bool m_bIsCrop;                    // 是否裁剪模式
    bool m_bIsTextMode;                // 是否在文本模式
    std::string m_strText;             // 要添加到图像的文本
};