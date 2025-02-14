#pragma once

#include <deque>

#include <opencv2/opencv.hpp>

/**
 * @brief 图像实例
 */
class ImgInstance {
public:
    ImgInstance(const ImgInstance &) = delete;
    ImgInstance &operator=(const ImgInstance &) = delete;

    /**
     * @brief 获取 ImgInstance 的单例实例。
     * @return 对单例实例的引用。
     */
    static ImgInstance &GetImgInstance();

    /**
     * @brief 获取 ImgInstance 的单例的当前图像。
     * @return Mat
     */
    static cv::Mat &GetNowImg();

    /**
     * @brief 获取 ImgInstance 的单例的图像记录。
     * @return std::deque
     */
    static std::deque<cv::Mat> &GetImgs();

    /**
     * @brief 获取 ImgInstance 的单例的当前图像的灰度图像。
     * @return Mat
     */
    static cv::Mat &GetGray();

    /**
     * @brief 获取 ImgInstance 的单例的当前图像的直方图。
     * @return Mat
     */
    static cv::Mat &GetHist();

    /**
     * @brief 获取 ImgInstance 的单例的临时图像。
     * @return Mat
     */
    static cv::Mat &GetTmp();

    /**
     * @brief 保留当前图像的历史记录
     * @param now 要保存的图像
     */
    static void KeepTrace(const cv::Mat &now);

private:
    ImgInstance() = default;

private:
    cv::Mat m_matNowImg;             // 当前图像
    std::deque<cv::Mat> m_dequeImgs; // 图像记录
    cv::Mat m_matHist;               // 当前图像的直方图
    cv::Mat m_matGray;               // 当前图像的灰度图
    cv::Mat m_matTmp;                // 临时数据
};