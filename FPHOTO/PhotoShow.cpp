#include "PhotoShow.h"
#include "FPHOTO.h"
#include "ImgInstance.h"
#include "ImgProcess.h"
#include "Utils.h"

PhotoShow::PhotoShow(QWidget *parent)
        : QGraphicsView(parent) {
    m_iType = -1, m_bIsCrop = 0, m_bIsTextMode = 0, m_color = cv::Scalar(255, 255, 255), m_iThickness = 1;
    m_bIsPaint = false;
    m_pixmapItem = new QGraphicsPixmapItem;
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);
}

void PhotoShow::mousePressEvent(QMouseEvent *e) {
    if (m_iType == -1)
        return;
    m_prePoint = m_pixmapItem->mapFromScene(mapToScene(e->pos()));

    if (m_bIsTextMode) {
        putTextZH(
            ImgInstance::GetNowImg(),
            m_strText.c_str(),
            cv::Point(m_prePoint.x(), m_prePoint.y()),
            m_color,
            m_iThickness,
            "微软雅黑",
            false,
            false);
        m_pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
        m_scene->addItem(m_pixmapItem);
        return;
    }

    m_bIsPaint = true;
}

void PhotoShow::mouseMoveEvent(QMouseEvent *e) {
    if (!m_bIsPaint or m_iType == -1)
        return;

    m_point = m_pixmapItem->mapFromScene(mapToScene(e->pos()));
    ImgInstance::GetTmp() = ImgInstance::GetNowImg().clone();

    switch (m_iType) {
    case 0:
        circle(ImgInstance::GetNowImg(), cv::Point(m_point.x(), m_point.y()), m_iThickness, m_color, -2);
        break;

    case 1:
        line(
            ImgInstance::GetTmp(),
            cv::Point(m_prePoint.x(), m_prePoint.y()),
            cv::Point(m_point.x(), m_point.y()),
            m_color,
            m_iThickness);
        break;

    case 2:
        rectangle(
            ImgInstance::GetTmp(),
            cv::Point(m_prePoint.x(), m_prePoint.y()),
            cv::Point(m_point.x(), m_point.y()),
            m_color,
            m_iThickness);
        break;

    case 3:
        circle(
            ImgInstance::GetTmp(),
            cv::Point(m_prePoint.x(), m_prePoint.y()),
            abs(m_prePoint.x() - m_point.x()) >= 0 ? abs(m_prePoint.x() - m_point.x()) / 2 : 0,
            m_color,
            m_iThickness);
        break;

    default:
        break;
    }

    m_pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetTmp())));
    m_scene->addItem(m_pixmapItem);
}

void PhotoShow::mouseReleaseEvent(QMouseEvent *e) {
    if (m_bIsCrop) {
        m_cropBox = cv::Rect(cv::Point(m_prePoint.x(), m_prePoint.y()), cv::Point(m_point.x(), m_point.y()));
        ImgInstance::GetNowImg() = cv::Mat(ImgInstance::GetNowImg(), m_cropBox);
        m_pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
        m_scene->addItem(m_pixmapItem);
        m_bIsCrop = false;
        m_iType = -1;
        return;
    }

    if (!m_bIsPaint or m_iType == -1)
        return;

    m_point = m_pixmapItem->mapFromScene(mapToScene(e->pos()));
    switch (m_iType) {
    case 0:
        break;

    case 1:
        line(
            ImgInstance::GetNowImg(),
            cv::Point(m_prePoint.x(), m_prePoint.y()),
            cv::Point(m_point.x(), m_point.y()),
            m_color,
            m_iThickness);
        break;

    case 2:
        rectangle(
            ImgInstance::GetNowImg(),
            cv::Point(m_prePoint.x(), m_prePoint.y()),
            cv::Point(m_point.x(), m_point.y()),
            m_color,
            m_iThickness);
        break;

    case 3:
        circle(
            ImgInstance::GetNowImg(),
            cv::Point(m_prePoint.x(), m_prePoint.y()),
            abs(m_prePoint.x() - m_point.x()) >= 0 ? abs(m_prePoint.x() - m_point.x()) / 2 : 0,
            m_color,
            m_iThickness);
        break;

    default:
        break;
    }

    m_bIsPaint = false;
    // ImgInstance::GetNowImg() = ImgInstance::GetTmp().clone(); ?
    m_pixmapItem->setPixmap(QPixmap::fromImage(MatToQImage(ImgInstance::GetNowImg())));
    m_scene->addItem(m_pixmapItem);
}