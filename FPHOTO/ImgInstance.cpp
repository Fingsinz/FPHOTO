#include "ImgInstance.h"

ImgInstance &ImgInstance::GetImgInstance() {
    static ImgInstance instance;
    return instance;
}

void ImgInstance::KeepTrace(const cv::Mat &now) {
    if (GetImgInstance().m_dequeImgs.size() == 10) {
        GetImgInstance().m_dequeImgs.pop_front();
    }

    GetImgInstance().m_dequeImgs.push_back(now.clone());
}

cv::Mat &ImgInstance::GetNowImg() {
    return GetImgInstance().m_matNowImg;
}

std::deque<cv::Mat> &ImgInstance::GetImgs() {
    return GetImgInstance().m_dequeImgs;
}

cv::Mat &ImgInstance::GetGray() {
    return GetImgInstance().m_matGray;
}

cv::Mat &ImgInstance::GetHist() {
    return GetImgInstance().m_matHist;
}

cv::Mat &ImgInstance::GetTmp() {
    return GetImgInstance().m_matTmp;
}