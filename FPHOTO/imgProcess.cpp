#include <iostream>
#include <vector>

#include "ImgProcess.h"
#include "Utils.h"

using namespace cv;

namespace ImgTools {
void ChangeBrightness(const cv::Mat &iMat, cv::Mat &oMat, int iFactor) {
    oMat = iMat.clone();
    if (iMat.channels() == 3) {
        oMat = iMat + iFactor;

        // for (int i = 0; i < iMat.rows; i++) {
        //     for (int j = 0; j < iMat.cols; j++) {
        //         oMat.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(iMat.at<Vec3b>(i, j)[0] + iFactor);
        //         oMat.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(iMat.at<Vec3b>(i, j)[1] + iFactor);
        //         oMat.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(iMat.at<Vec3b>(i, j)[2] + iFactor);
        //     }
        // }
    }

    else if (iMat.channels() == 1) {
        for (int i = 0; i < iMat.rows; i++) {
            for (int j = 0; j < iMat.cols; j++) {
                oMat.at<uchar>(i, j) = saturate_cast<uchar>(iMat.at<uchar>(i, j) + iFactor);
            }
        }
    }
}

void ChangeContrast(const cv::Mat &iMat, cv::Mat &oMat, double idFactor) {
    oMat = iMat.clone();
    if (iMat.channels() == 3) {
        iMat.convertTo(oMat, CV_64F, 1.0 / 255, 0);
        pow(oMat, idFactor, oMat);
        oMat.convertTo(oMat, CV_8UC3, 255, 0);
    }

    else if (iMat.channels() == 1) {
        iMat.convertTo(oMat, CV_64F, 1.0 / 255, 0);
        pow(oMat, idFactor, oMat);
        oMat.convertTo(oMat, CV_8U, 255, 0);
    }
}

void Resize(const cv::Mat &iMat, cv::Mat &oMat, double idFactor) {
    if (iMat.channels() == 3) {
        oMat = Mat(iMat.rows * idFactor, iMat.cols * idFactor, CV_8UC3);
        for (int i = 0; i < oMat.rows; i++)
            for (int j = 0; j < oMat.cols; j++)
                oMat.at<Vec3b>(i, j) = iMat.at<Vec3b>(i / idFactor, j / idFactor);
    }

    else if (iMat.channels() == 1) {
        oMat = Mat(iMat.rows * idFactor, iMat.cols * idFactor, CV_8UC1);
        for (int i = 0; i < oMat.rows; i++)
            for (int j = 0; j < oMat.cols; j++)
                oMat.at<uchar>(i, j) = iMat.at<uchar>(i / idFactor, j / idFactor);
    }
}

void Rotate(const cv::Mat &iMat, cv::Mat &oMat, double idAngle) {
    double alpha = -idAngle * CV_PI / 180.0;

    // 定义仿射变换的输入点
    Point2f inp[3], outp[3];
    inp[0] = Point2f(0, iMat.rows);
    inp[1] = Point2f(iMat.cols, 0);
    inp[2] = Point2f(iMat.cols, iMat.rows);

    // 计算仿射变换的输出点
    for (int i = 0; i < 3; i++) {
        outp[i] = Point2f(inp[i].x * cos(alpha) - inp[i].y * sin(alpha), inp[i].x * sin(alpha) + inp[i].y * cos(alpha));
    }

    // 找到输出点的最小和最大x和y值
    double minx = min(min(min(outp[0].x, outp[1].x), outp[2].x), double(0.0));
    double miny = min(min(min(outp[0].y, outp[1].y), outp[2].y), double(0.0));
    double maxx = max(max(max(outp[0].x, outp[1].x), outp[2].x), double(0.0));
    double maxy = max(max(max(outp[0].y, outp[1].y), outp[2].y), double(0.0));

    // 计算输出图像的宽度和高度
    int w = maxx - minx, h = maxy - miny;

    // 必要时调整输出点
    for (int i = 0; i < 3; i++) {
        if (minx < 0)
            outp[i].x -= minx;
        if (miny < 0)
            outp[i].y -= miny;
    }

    // 创建仿射变换矩阵
    Mat warpMat = getAffineTransform(inp, outp);
    warpAffine(iMat, oMat, warpMat, Size(w, h));
}

void HorizontalFlip(const cv::Mat &iMat, cv::Mat &oMat) {
    oMat = iMat.clone();

    if (iMat.channels() == 3) {
        for (int i = 0; i < iMat.rows; i++) {
            for (int j = 0; j < iMat.cols; j++) {
                oMat.at<Vec3b>(i, j) = iMat.at<Vec3b>(i, iMat.cols - j - 1);
            }
        }
    }

    else if (iMat.channels() == 1) {
        for (int i = 0; i < iMat.rows; i++) {
            for (int j = 0; j < iMat.cols; j++) {
                oMat.at<uchar>(i, j) = iMat.at<uchar>(i, iMat.cols - j - 1);
            }
        }
    }
}

void VerticalFlip(const cv::Mat &iMat, cv::Mat &oMat) {
    oMat = iMat.clone();

    if (iMat.channels() == 3) {
        for (int i = 0; i < iMat.rows; i++) {
            for (int j = 0; j < iMat.cols; j++) {
                oMat.at<Vec3b>(i, j) = iMat.at<Vec3b>(iMat.rows - i - 1, j);
            }
        }
    }

    else if (iMat.channels() == 1) {
        for (int i = 0; i < iMat.rows; i++) {
            for (int j = 0; j < iMat.cols; j++) {
                oMat.at<uchar>(i, j) = iMat.at<uchar>(iMat.rows - i - 1, j);
            }
        }
    }
}

void Inversion(const cv::Mat &iMat, cv::Mat &oMat) {
    oMat = iMat.clone();

    if (iMat.channels() == 3)
        for (int i = 0; i < iMat.rows; i++)
            for (int j = 0; j < iMat.cols; j++)
                oMat.at<Vec3b>(i, j) = Vec3b(255, 255, 255) - iMat.at<Vec3b>(i, j);

    else if (iMat.channels() == 1)
        for (int i = 0; i < iMat.rows; i++)
            for (int j = 0; j < iMat.cols; j++)
                oMat.at<uchar>(i, j) = 255 - iMat.at<uchar>(i, j);
}

void Grayscale(const cv::Mat &iMat, cv::Mat &oMat, bool ibType) {
    if (iMat.channels() != 3)
        return;

    if (ibType) {
        oMat = Mat(iMat.size(), CV_8UC1, Scalar(0));
    } else if (!ibType) {
        oMat = iMat.clone();
    }

    for (int i = 0; i < iMat.rows; i++) {
        for (int j = 0; j < iMat.cols; j++) {
            int tmp = iMat.at<Vec3b>(i, j)[0] + iMat.at<Vec3b>(i, j)[1] + iMat.at<Vec3b>(i, j)[2];
            if (ibType) {
                oMat.at<uchar>(i, j) = tmp / 3;
            } else if (!ibType) {
                oMat.at<Vec3b>(i, j) = Vec3b(tmp / 3, tmp / 3, tmp / 3);
            }
        }
    }
}

void DrawHistogram(const cv::Mat &iMat, cv::Mat &oMat) {
    int size = iMat.rows * iMat.cols;

    if (iMat.channels() == 3) {
        oMat = Mat(Size(512, 500), CV_8UC3, Scalar(0));
        double dPxB[256]{0}, dPxG[256]{0}, dPxR[256]{0};
        double maxp = 0;
        for (int i = 0; i < iMat.rows; i++) {
            for (int j = 0; j < iMat.cols; j++) {
                dPxB[iMat.at<Vec3b>(i, j)[0]] += 1;
                dPxG[iMat.at<Vec3b>(i, j)[1]] += 1;
                dPxR[iMat.at<Vec3b>(i, j)[2]] += 1;
            }
        }

        for (int i = 0; i < 256; i++) {
            maxp = max(maxp, max(dPxB[i], max(dPxG[i], dPxR[i])));
        }

        for (int i = 0; i < 256; i++) {
            dPxB[i] = dPxB[i] / maxp * 500;
            dPxG[i] = dPxG[i] / maxp * 500;
            dPxR[i] = dPxR[i] / maxp * 500;
        }

        for (int i = 1; i < 256; i++) {
            line(oMat, Point((i - 1) * 2, 500 - dPxB[i - 1]), Point(i * 2, 500 - dPxB[i]), Scalar(255, 0, 0), 2);
            line(oMat, Point((i - 1) * 2, 500 - dPxG[i - 1]), Point(i * 2, 500 - dPxG[i]), Scalar(0, 255, 0), 2);
            line(oMat, Point((i - 1) * 2, 500 - dPxR[i - 1]), Point(i * 2, 500 - dPxR[i]), Scalar(0, 0, 255), 2);
        }
    }

    if (iMat.channels() == 1) {
        oMat = Mat(Size(512, 500), CV_8UC1, Scalar(0));
        double px[256]{0};
        double maxp = 0;
        for (int i = 0; i < iMat.rows; i++) {
            for (int j = 0; j < iMat.cols; j++) {
                px[iMat.at<uchar>(i, j)] += 1;
            }
        }

        for (int i = 0; i < 256; i++) {
            maxp = max(maxp, px[i]);
        }

        for (int i = 0; i < 256; i++) {
            px[i] = px[i] / maxp * 500;
        }

        for (int i = 1; i < 256; i++) {
            line(oMat, Point((i - 1) * 2, 500 - px[i - 1]), Point(i * 2, 500 - px[i]), Scalar(255), 2);
        }
    }
}

void HistEqualization(const cv::Mat &iMat, cv::Mat &oMat, cv::Mat &oMatHist) {
    int size = iMat.rows * iMat.cols;

    if (iMat.channels() == 3) {
        oMat = iMat.clone();

        std::vector<double> vdPxB(256, 0), vdPxG(256, 0), vdPxR(256, 0);
        std::vector<double> vdSkB(256, 0), vdSkG(256, 0), vdSkR(256, 0);
        std::vector<int> viSkB(256, 0), viSkG(256, 0), viSkR(256, 0);

        for (int i = 0; i < iMat.rows; i++) {
            for (int j = 0; j < iMat.cols; j++) {
                vdPxB[iMat.at<Vec3b>(i, j)[0]] += 1;
                vdPxG[iMat.at<Vec3b>(i, j)[1]] += 1;
                vdPxR[iMat.at<Vec3b>(i, j)[2]] += 1;
            }
        }

        vdSkB[0] = vdPxB[0] / size, vdSkG[0] = vdPxG[0] / size, vdSkR[0] = vdPxR[0] / size;
        for (int i = 1; i < 256; i++) {
            vdSkB[i] = vdSkB[i - 1] + vdPxB[i] / size;
            vdSkG[i] = vdSkG[i - 1] + vdPxG[i] / size;
            vdSkR[i] = vdSkR[i - 1] + vdPxR[i] / size;
        }

        for (int i = 0; i < 256; i++) {
            viSkB[i] = 255 * vdSkB[i] + 0.5;
            viSkG[i] = 255 * vdSkG[i] + 0.5;
            viSkR[i] = 255 * vdSkR[i] + 0.5;
        }

        for (int i = 0; i < iMat.rows; i++) {
            for (int j = 0; j < iMat.cols; j++) {
                oMat.at<Vec3b>(i, j)[0] = viSkB[iMat.at<Vec3b>(i, j)[0]];
                oMat.at<Vec3b>(i, j)[1] = viSkG[iMat.at<Vec3b>(i, j)[1]];
                oMat.at<Vec3b>(i, j)[2] = viSkR[iMat.at<Vec3b>(i, j)[2]];
            }
        }

        oMatHist = Mat(Size(512, 500), CV_8UC3, Scalar(0));
        std::vector<double> _vdPxB(256, 0), _vdPxG(256, 0), _vdPxR(256, 0);
        double maxp = 0;
        for (int i = 0; i < oMat.rows; i++) {
            for (int j = 0; j < oMat.cols; j++) {
                _vdPxB[oMat.at<Vec3b>(i, j)[0]] += 1;
                _vdPxG[oMat.at<Vec3b>(i, j)[1]] += 1;
                _vdPxR[oMat.at<Vec3b>(i, j)[2]] += 1;
            }
        }

        for (int i = 0; i < 256; i++)
            maxp = max(maxp, max(_vdPxB[i], max(_vdPxG[i], _vdPxR[i])));

        for (int i = 0; i < 256; i++) {
            _vdPxB[i] = _vdPxB[i] / maxp * 500;
            _vdPxG[i] = _vdPxG[i] / maxp * 500;
            _vdPxR[i] = _vdPxR[i] / maxp * 500;
        }

        for (int i = 1; i < 256; i++) {
            line(
                oMatHist, Point((i - 1) * 2, 500 - _vdPxB[i - 1]), Point(i * 2, 500 - _vdPxB[i]), Scalar(255, 0, 0), 2);
            line(
                oMatHist, Point((i - 1) * 2, 500 - _vdPxG[i - 1]), Point(i * 2, 500 - _vdPxG[i]), Scalar(0, 255, 0), 2);
            line(
                oMatHist, Point((i - 1) * 2, 500 - _vdPxR[i - 1]), Point(i * 2, 500 - _vdPxR[i]), Scalar(0, 0, 255), 2);
        }
    }

    else if (iMat.channels() == 1) {
        cv::equalizeHist(iMat, oMat);
        oMatHist = Mat(Size(512, 500), CV_8UC1, Scalar(0));
        std::vector<double> px(256, 0);
        double maxp = 0;
        for (int i = 0; i < oMat.rows; i++) {
            for (int j = 0; j < oMat.cols; j++) {
                px[oMat.at<uchar>(i, j)] += 1;
            }
        }

        for (int i = 0; i < 256; i++) {
            maxp = max(maxp, px[i]);
        }

        for (int i = 0; i < 256; i++) {
            px[i] = px[i] / maxp * 500;
        }

        for (int i = 1; i < 256; i++) {
            line(oMatHist, Point((i - 1) * 2, 500 - px[i - 1]), Point(i * 2, 500 - px[i]), Scalar(255), 2);
        }
    }
}

void ArithmeticAverageFilter(const cv::Mat &iMat, cv::Mat &oMat, int iKSize) {
    oMat = iMat.clone();
    cv::Mat tmp;
    copyMakeBorder(iMat, tmp, (iKSize - 1) / 2, (iKSize - 1) / 2, (iKSize - 1) / 2, (iKSize - 1) / 2, BORDER_REFLECT);

    if (iMat.channels() == 3) {
        for (int i = (iKSize - 1) / 2; i < tmp.rows - (iKSize - 1) / 2; i++) {
            for (int j = (iKSize - 1) / 2; j < tmp.cols - (iKSize - 1) / 2; j++) {
                int sum1 = 0, sum2 = 0, sum3 = 0;

                for (int x = -(iKSize - 1) / 2; x <= (iKSize - 1) / 2; x++) {
                    for (int y = -(iKSize - 1) / 2; y <= (iKSize - 1) / 2; y++) {
                        sum1 += tmp.at<Vec3b>(i + x, j + y)[0];
                        sum2 += tmp.at<Vec3b>(i + x, j + y)[1];
                        sum3 += tmp.at<Vec3b>(i + x, j + y)[2];
                    }
                }

                oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[0] = round(sum1 / (iKSize * iKSize));
                oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[1] = round(sum2 / (iKSize * iKSize));
                oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[2] = round(sum3 / (iKSize * iKSize));
            }
        }
    } else if (iMat.channels() == 1) {
        for (int i = (iKSize - 1) / 2; i < tmp.rows - (iKSize - 1) / 2; i++) {
            for (int j = (iKSize - 1) / 2; j < tmp.cols - (iKSize - 1) / 2; j++) {
                int sum = 0;

                for (int x = -(iKSize - 1) / 2; x <= (iKSize - 1) / 2; x++) {
                    for (int y = -(iKSize - 1) / 2; y <= (iKSize - 1) / 2; y++) {
                        sum += tmp.at<uchar>(i + x, j + y);
                    }
                }

                oMat.at<uchar>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2) = round(sum / (iKSize * iKSize));
            }
        }
    }
}

void GeometricAverageFilter(const cv::Mat &iMat, cv::Mat &oMat, int iKSize) {
    oMat = iMat.clone();
    cv::Mat tmp;
    cv::copyMakeBorder(
        iMat, tmp, (iKSize - 1) / 2, (iKSize - 1) / 2, (iKSize - 1) / 2, (iKSize - 1) / 2, BORDER_REFLECT);

    if (iMat.channels() == 3) {
        for (int i = (iKSize - 1) / 2; i < tmp.rows - (iKSize - 1) / 2; i++) {
            for (int j = (iKSize - 1) / 2; j < tmp.cols - (iKSize - 1) / 2; j++) {
                double sum1 = 0, sum2 = 0, sum3 = 0;

                for (int x = -(iKSize - 1) / 2; x <= (iKSize - 1) / 2; x++) {
                    for (int y = -(iKSize - 1) / 2; y <= (iKSize - 1) / 2; y++) {
                        sum1 += log10(tmp.at<Vec3b>(i + x, j + y)[0] + 0.1);
                        sum2 += log10(tmp.at<Vec3b>(i + x, j + y)[1] + 0.1);
                        sum3 += log10(tmp.at<Vec3b>(i + x, j + y)[2] + 0.1);
                    }
                }

                sum1 /= iKSize * iKSize, sum2 /= iKSize * iKSize, sum3 /= iKSize * iKSize;
                oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[0] = round(pow(10, sum1));
                oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[1] = round(pow(10, sum2));
                oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[2] = round(pow(10, sum3));
            }
        }
    } else if (iMat.channels() == 1) {
        for (int i = (iKSize - 1) / 2; i < tmp.rows - (iKSize - 1) / 2; i++) {
            for (int j = (iKSize - 1) / 2; j < tmp.cols - (iKSize - 1) / 2; j++) {
                double sum = 0;

                for (int x = -(iKSize - 1) / 2; x <= (iKSize - 1) / 2; x++) {
                    for (int y = -(iKSize - 1) / 2; y <= (iKSize - 1) / 2; y++) {
                        sum += log10(tmp.at<uchar>(i + x, j + y) + 0.1);
                    }
                }

                sum /= iKSize * iKSize;
                oMat.at<uchar>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2) = round(pow(10, sum));
            }
        }
    }
}

void HarmonicMeanFilter(const cv::Mat &iMat, cv::Mat &oMat, int iKSize) {
    oMat = iMat.clone();
    cv::Mat tmp;
    copyMakeBorder(iMat, tmp, (iKSize - 1) / 2, (iKSize - 1) / 2, (iKSize - 1) / 2, (iKSize - 1) / 2, BORDER_REFLECT);

    if (iMat.channels() == 3) {
        for (int i = (iKSize - 1) / 2; i < tmp.rows - (iKSize - 1) / 2; i++) {
            for (int j = (iKSize - 1) / 2; j < tmp.cols - (iKSize - 1) / 2; j++) {
                double sum1 = 0, sum2 = 0, sum3 = 0;

                for (int x = -(iKSize - 1) / 2; x <= (iKSize - 1) / 2; x++) {
                    for (int y = -(iKSize - 1) / 2; y <= (iKSize - 1) / 2; y++) {
                        sum1 += 1.0 / (tmp.at<Vec3b>(i + x, j + y)[0] + 0.1);
                        sum2 += 1.0 / (tmp.at<Vec3b>(i + x, j + y)[1] + 0.1);
                        sum3 += 1.0 / (tmp.at<Vec3b>(i + x, j + y)[2] + 0.1);
                    }
                }
                sum1 = iKSize * iKSize / sum1, sum2 = iKSize * iKSize / sum2, sum3 = iKSize * iKSize / sum3;
                oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[0] = sum1;
                oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[1] = sum2;
                oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[2] = sum3;
            }
        }
    } else if (iMat.channels() == 1) {
        for (int i = (iKSize - 1) / 2; i < tmp.rows - (iKSize - 1) / 2; i++) {
            for (int j = (iKSize - 1) / 2; j < tmp.cols - (iKSize - 1) / 2; j++) {
                double sum = 0;

                for (int x = -(iKSize - 1) / 2; x <= (iKSize - 1) / 2; x++) {
                    for (int y = -(iKSize - 1) / 2; y <= (iKSize - 1) / 2; y++) {
                        sum += 1.0 / (tmp.at<uchar>(i + x, j + y) + 0.1);
                    }
                }
                sum = iKSize * iKSize / sum;
                oMat.at<uchar>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2) = sum;
            }
        }
    }
}

void MedianFilter(const cv::Mat &iMat, cv::Mat &oMat, int iKSize) {
    oMat = iMat.clone();
    cv::Mat tmp;

    if (iMat.channels() == 3) {
        std::vector<uchar> vBs(iKSize * iKSize);
        std::vector<uchar> vGs(iKSize * iKSize);
        std::vector<uchar> vRs(iKSize * iKSize);

        copyMakeBorder(
            iMat, tmp, (iKSize - 1) / 2, (iKSize - 1) / 2, (iKSize - 1) / 2, (iKSize - 1) / 2, BORDER_REFLECT);

        for (int i = (iKSize - 1) / 2; i < tmp.rows - (iKSize - 1) / 2; i++) {
            for (int j = (iKSize - 1) / 2; j < tmp.cols - (iKSize - 1) / 2; j++) {
                int h = 0;

                for (int x = -(iKSize - 1) / 2; x <= (iKSize - 1) / 2; x++) {
                    for (int y = -(iKSize - 1) / 2; y <= (iKSize - 1) / 2; y++) {
                        vBs[h] = tmp.at<Vec3b>(i + x, j + y)[0];
                        vGs[h] = tmp.at<Vec3b>(i + x, j + y)[1];
                        vRs[h] = tmp.at<Vec3b>(i + x, j + y)[2];
                        h += 1;
                    }
                }

                std::sort(vBs.begin(), vBs.end());
                std::sort(vGs.begin(), vGs.end());
                std::sort(vRs.begin(), vRs.end());

                oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[0] = vBs[(iKSize * iKSize - 1) / 2];
                oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[1] = vGs[(iKSize * iKSize - 1) / 2];
                oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[2] = vRs[(iKSize * iKSize - 1) / 2];
            }
        }
    }

    if (iMat.channels() == 1) {
        std::vector<uchar> arry(iKSize * iKSize);

        copyMakeBorder(
            iMat, tmp, (iKSize - 1) / 2, (iKSize - 1) / 2, (iKSize - 1) / 2, (iKSize - 1) / 2, BORDER_REFLECT);

        for (int i = (iKSize - 1) / 2; i < tmp.rows - (iKSize - 1) / 2; i++) {
            for (int j = (iKSize - 1) / 2; j < tmp.cols - (iKSize - 1) / 2; j++) {
                int h = 0;

                for (int x = -(iKSize - 1) / 2; x <= (iKSize - 1) / 2; x++) {
                    for (int y = -(iKSize - 1) / 2; y <= (iKSize - 1) / 2; y++) {
                        arry[h] = tmp.at<uchar>(i + x, j + y);
                        h += 1;
                    }
                }

                std::sort(arry.begin(), arry.end());

                oMat.at<uchar>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2) = arry[(iKSize * iKSize - 1) / 2];
            }
        }
    }
}

void MaxMinFilter(const cv::Mat &iMat, cv::Mat &oMat, int iKSize, int iType) {
    oMat = iMat.clone();
    cv::Mat tmp;

    if (iMat.channels() == 3) {
        std::vector<uchar> vBs(iKSize * iKSize);
        std::vector<uchar> vGs(iKSize * iKSize);
        std::vector<uchar> vRs(iKSize * iKSize);

        copyMakeBorder(
            iMat, tmp, (iKSize - 1) / 2, (iKSize - 1) / 2, (iKSize - 1) / 2, (iKSize - 1) / 2, BORDER_REFLECT);

        for (int i = (iKSize - 1) / 2; i < tmp.rows - (iKSize - 1) / 2; i++) {
            for (int j = (iKSize - 1) / 2; j < tmp.cols - (iKSize - 1) / 2; j++) {
                int h = 0;

                for (int x = -(iKSize - 1) / 2; x <= (iKSize - 1) / 2; x++) {
                    for (int y = -(iKSize - 1) / 2; y <= (iKSize - 1) / 2; y++) {
                        vBs[h] = tmp.at<Vec3b>(i + x, j + y)[0];
                        vGs[h] = tmp.at<Vec3b>(i + x, j + y)[1];
                        vRs[h] = tmp.at<Vec3b>(i + x, j + y)[2];
                        h += 1;
                    }
                }

                std::sort(vBs.begin(), vBs.end());
                std::sort(vGs.begin(), vGs.end());
                std::sort(vRs.begin(), vRs.end());

                if (iType == 1) {
                    oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[0] = vBs[iKSize * iKSize - 1];
                    oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[1] = vGs[iKSize * iKSize - 1];
                    oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[2] = vRs[iKSize * iKSize - 1];
                } else if (iType == 0) {
                    oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[0] = vBs[0];
                    oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[1] = vGs[0];
                    oMat.at<Vec3b>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2)[2] = vRs[0];
                }
            }
        }
    }

    else if (iMat.channels() == 1) {
        std::vector<uchar> arry(iKSize * iKSize);
        copyMakeBorder(
            iMat, tmp, (iKSize - 1) / 2, (iKSize - 1) / 2, (iKSize - 1) / 2, (iKSize - 1) / 2, BORDER_REFLECT);
        for (int i = (iKSize - 1) / 2; i < tmp.rows - (iKSize - 1) / 2; i++) {
            for (int j = (iKSize - 1) / 2; j < tmp.cols - (iKSize - 1) / 2; j++) {
                int h = 0;

                for (int x = -(iKSize - 1) / 2; x <= (iKSize - 1) / 2; x++) {
                    for (int y = -(iKSize - 1) / 2; y <= (iKSize - 1) / 2; y++) {
                        arry[h] = tmp.at<uchar>(i + x, j + y);
                        h += 1;
                    }
                }

                std::sort(arry.begin(), arry.end());

                if (iType == 1) {
                    oMat.at<uchar>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2) = arry[iKSize * iKSize - 1];
                } else if (iType == 0) {
                    oMat.at<uchar>(i - (iKSize - 1) / 2, j - (iKSize - 1) / 2) = arry[0];
                }
            }
        }
    }
}

void LaplaceFilter(const cv::Mat &iMat, cv::Mat &oMat, int iType) {
    oMat = iMat.clone();

    if (iMat.channels() == 3) {
        int la[3]{0};
        for (int i = 1; i < iMat.rows - 1; i++) {
            for (int j = 1; j < iMat.cols - 1; j++) {
                if (iType == 0) {
                    la[0] = iMat.at<Vec3b>(i + 1, j)[0] + iMat.at<Vec3b>(i - 1, j)[0] + iMat.at<Vec3b>(i, j + 1)[0] +
                        iMat.at<Vec3b>(i, j - 1)[0] - 5 * iMat.at<Vec3b>(i, j)[0];
                    la[1] = iMat.at<Vec3b>(i + 1, j)[1] + iMat.at<Vec3b>(i - 1, j)[1] + iMat.at<Vec3b>(i, j + 1)[1] +
                        iMat.at<Vec3b>(i, j - 1)[1] - 5 * iMat.at<Vec3b>(i, j)[1];
                    la[2] = iMat.at<Vec3b>(i + 1, j)[2] + iMat.at<Vec3b>(i - 1, j)[2] + iMat.at<Vec3b>(i, j + 1)[2] +
                        iMat.at<Vec3b>(i, j - 1)[2] - 5 * iMat.at<Vec3b>(i, j)[2];
                } else if (iType == 1) {
                    la[0] = iMat.at<Vec3b>(i - 1, j - 1)[0] + iMat.at<Vec3b>(i - 1, j)[0] +
                        iMat.at<Vec3b>(i - 1, j + 1)[0] + iMat.at<Vec3b>(i, j - 1)[0] + iMat.at<Vec3b>(i, j + 1)[0] +
                        iMat.at<Vec3b>(i + 1, j - 1)[0] + iMat.at<Vec3b>(i + 1, j)[0] +
                        iMat.at<Vec3b>(i + 1, j + 1)[0] - 9 * iMat.at<Vec3b>(i, j)[0];
                    la[1] = iMat.at<Vec3b>(i - 1, j - 1)[1] + iMat.at<Vec3b>(i - 1, j)[1] +
                        iMat.at<Vec3b>(i - 1, j + 1)[1] + iMat.at<Vec3b>(i, j - 1)[1] + iMat.at<Vec3b>(i, j + 1)[1] +
                        iMat.at<Vec3b>(i + 1, j - 1)[1] + iMat.at<Vec3b>(i + 1, j)[1] +
                        iMat.at<Vec3b>(i + 1, j + 1)[1] - 9 * iMat.at<Vec3b>(i, j)[1];
                    la[2] = iMat.at<Vec3b>(i - 1, j - 1)[2] + iMat.at<Vec3b>(i - 1, j)[2] +
                        iMat.at<Vec3b>(i - 1, j + 1)[2] + iMat.at<Vec3b>(i, j - 1)[2] + iMat.at<Vec3b>(i, j + 1)[2] +
                        iMat.at<Vec3b>(i + 1, j - 1)[2] + iMat.at<Vec3b>(i + 1, j)[2] +
                        iMat.at<Vec3b>(i + 1, j + 1)[2] - 9 * iMat.at<Vec3b>(i, j)[2];
                }

                oMat.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(-la[0]);
                oMat.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(-la[1]);
                oMat.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(-la[2]);
            }
        }
    }

    else if (iMat.channels() == 1) {
        int la = 0;
        for (int i = 1; i < iMat.rows - 1; i++) {
            for (int j = 1; j < iMat.cols - 1; j++) {
                if (iType == 0) {
                    la = iMat.at<uchar>(i + 1, j) + iMat.at<uchar>(i - 1, j) + iMat.at<uchar>(i, j + 1) +
                        iMat.at<uchar>(i, j - 1) - 5 * iMat.at<uchar>(i, j);
                } else if (iType == 1) {
                    la = iMat.at<uchar>(i - 1, j - 1) + iMat.at<uchar>(i - 1, j) + iMat.at<uchar>(i - 1, j + 1) +
                        iMat.at<uchar>(i, j - 1) + iMat.at<uchar>(i, j + 1) + iMat.at<uchar>(i + 1, j - 1) +
                        iMat.at<uchar>(i + 1, j) + iMat.at<uchar>(i + 1, j + 1) - 9 * iMat.at<uchar>(i, j);
                }

                oMat.at<uchar>(i, j) = saturate_cast<uchar>(-la);
            }
        }
    }
}

void DilationOrErosion(const cv::Mat &iMat, cv::Mat &oMat, int iKSize, int iKType, int iOperation, int iCnt) {
    Mat kernel = getStructuringElement(iKType, Size(iKSize, iKSize));
    if (iOperation == 0) {
        dilate(iMat, oMat, kernel, Point(-1, -1), iCnt);
    } else if (iOperation == 1) {
        erode(iMat, oMat, kernel, Point(-1, -1), iCnt);
    }
}

void OpeningOrClosing(const Mat &iMat, Mat &oMat, int iKSize, int iKType, int iOperation, int iCnt) {
    Mat kernel = getStructuringElement(iKType, Size(iKSize, iKSize));
    morphologyEx(iMat, oMat, iOperation, kernel, Point(-1, -1), iCnt);
}

void TopOrBottomHatTrans(const cv::Mat &iMat, cv::Mat &oMat, int iKSize, int iKType, int iOperation, int iCnt) {
    Mat kernel = getStructuringElement(iKType, Size(iKSize, iKSize));
    morphologyEx(iMat, oMat, iOperation, kernel, Point(-1, -1), iCnt);
}

void ThresholdSegment(const cv::Mat &iMat, cv::Mat &oMat, int iValue) {
    Mat inGray;

    if (iMat.channels() == 3) {
        cvtColor(iMat, inGray, COLOR_BGR2GRAY);
    } else if (iMat.channels() == 1) {
        inGray = iMat.clone();
    }

    threshold(inGray, oMat, iValue, 255, THRESH_BINARY);
}

void IterativeThresholdSegment(const cv::Mat &iMat, cv::Mat &oMat, int &oValue) {
    Mat inGray;
    if (iMat.channels() == 3) {
        cvtColor(iMat, inGray, COLOR_BGR2GRAY);
    } else if (iMat.channels() == 1) {
        inGray = iMat.clone();
    }

    double th = 0.5;
    oValue = 0;
    double prev = 0;

    for (int i = 0; i < inGray.rows; i++)
        for (int j = 0; j < inGray.cols; j++)
            oValue += inGray.at<uchar>(i, j);

    oValue = 1.0 * oValue / (inGray.rows * inGray.cols);

    while (std::abs(prev - oValue) > th) {
        prev = oValue;
        int mean1 = 0, mean2 = 0, cnt1 = 0, cnt2 = 0;

        for (int i = 0; i < inGray.rows; i++) {
            for (int j = 0; j < inGray.cols; j++) {
                if (inGray.at<uchar>(i, j) > oValue) {
                    mean1 += inGray.at<uchar>(i, j);
                    cnt1++;
                } else {
                    mean2 += inGray.at<uchar>(i, j);
                    cnt2++;
                }
            }
        }

        mean1 = 1.0 * mean1 / cnt1;
        mean2 = 1.0 * mean2 / cnt2;
        oValue = (mean1 + mean2) / 2;
    }
    threshold(inGray, oMat, oValue, 255, THRESH_BINARY);
}

void OTSUThresholdSegment(const cv::Mat &iMat, cv::Mat &oMat, int &oValue) {
    Mat inGray;
    if (iMat.channels() == 3) {
        cvtColor(iMat, inGray, COLOR_BGR2GRAY);
    } else if (iMat.channels() == 1) {
        inGray = iMat.clone();
    }

    // 计数灰度直方图各像素值
    int hist[256] = {0};
    for (int i = 0; i < inGray.rows; i++) {
        for (int j = 0; j < inGray.cols; j++) {
            hist[inGray.at<uchar>(i, j)]++;
        }
    }

    // 计算灰度直方图各像素概率
    double phist[256] = {0};
    for (int i = 0; i < 256; i++) {
        phist[i] = 1.0 * hist[i] / (inGray.rows * inGray.cols);
    }

    double zeroCumuMoment[256] = {0}, oneCumuMoment[256] = {0};

    // 计算灰度直方图的零阶累积距和一阶累积距
    zeroCumuMoment[0] = phist[0];
    oneCumuMoment[0] = 0 * phist[0];
    for (int i = 1; i < 256; i++) {
        zeroCumuMoment[i] += zeroCumuMoment[i - 1] + phist[i];
        oneCumuMoment[i] = oneCumuMoment[i - 1] + i * phist[i];
    }

    double mean = oneCumuMoment[255];

    double _max = -1;
    oValue = 0;

    for (int i = 0; i < 256; i++) {
        double tmp = mean * zeroCumuMoment[i] - oneCumuMoment[i];
        double tmp1 = zeroCumuMoment[i] * (1 - zeroCumuMoment[i]);
        double ans = tmp * tmp / tmp1;
        if (_max < ans) {
            _max = ans;
            oValue = i;
        }
    }
    threshold(inGray, oMat, oValue, 255, THRESH_BINARY);
}

void RegionGrowth(const cv::Mat &iMat, Mat &oMat, int iValue) {
    Point pt = Point(iMat.cols / 2, iMat.rows / 2); // 开始生长点
    Point waitGrowing;                              // 待生长点位置
    int growLable = 0;                              // 标记是否生长过
    int startValue = 0;                             // 生长起点灰度值
    int nowValue = 0;                               // 当前生长点灰度值
    oMat = Mat(iMat.size(), CV_8UC1, Scalar(0));    // 创建一个空白区域，填充为黑色
    std::vector<Point> GrowPts;                     // 生长点栈
    GrowPts.push_back(pt);                          // 将生长点压入栈中
    oMat.at<uchar>(pt.y, pt.x) = 255;               // 标记生长点
    startValue = iMat.at<uchar>(pt.y, pt.x);        // 记录生长点的灰度值
    int DIR[8][2] = {                               // 生长方向顺序
                     {-1, -1},
                     {0, -1},
                     {1, -1},
                     {1, 0},
                     {1, 1},
                     {0, 1},
                     {-1, 1},
                     {-1, 0}};

    while (!GrowPts.empty()) {
        // 取出一个生长点
        pt = GrowPts.back();
        GrowPts.pop_back();

        // 分别对八个方向上的点进行生长
        for (int i = 0; i < 8; ++i) {
            waitGrowing.x = pt.x + DIR[i][0];
            waitGrowing.y = pt.y + DIR[i][1];

            // 检查是否是边缘点
            if (waitGrowing.x < 0 || waitGrowing.y < 0 || waitGrowing.x > (iMat.cols - 1) ||
                (waitGrowing.y > iMat.rows - 1))
                continue;

            growLable = oMat.at<uchar>(waitGrowing.y, waitGrowing.x); // 当前待生长点的灰度值

            if (growLable == 0) // 如果标记点还没有被生长
            {
                nowValue = iMat.at<uchar>(waitGrowing.y, waitGrowing.x);
                if (abs(startValue - nowValue) < iValue) // 在阈值范围内则生长
                {
                    oMat.at<uchar>(waitGrowing.y, waitGrowing.x) = 255; // 标记为白色
                    GrowPts.push_back(waitGrowing);                     // 将下一个生长点压入栈中
                }
            }
        }
    }
}

void Sobel(const cv::Mat &iMat, cv::Mat &oMat) {
    Mat kernel1 = (cv::Mat_<float>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
    Mat kernel2 = (cv::Mat_<float>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);

    Mat tmp1, tmp2;
    filter2D(iMat, tmp1, -1, kernel1);
    filter2D(iMat, tmp2, -1, kernel2);

    convertScaleAbs(tmp1, tmp1);
    convertScaleAbs(tmp2, tmp2);

    oMat = tmp1 + tmp2;

    if (oMat.channels() == 3) {
        cvtColor(oMat, oMat, COLOR_BGR2GRAY);
    }
    threshold(oMat, oMat, 130, 255, THRESH_BINARY | THRESH_OTSU);
}

void Roberts(const cv::Mat &iMat, cv::Mat &oMat) {
    Mat kernel1 = (cv::Mat_<float>(2, 2) << -1, 0, 0, 1);
    Mat kernel2 = (cv::Mat_<float>(2, 2) << 0, -1, 1, 0);

    Mat tmp1, tmp2;
    filter2D(iMat, tmp1, -1, kernel1);
    filter2D(iMat, tmp2, -1, kernel2);

    convertScaleAbs(tmp1, tmp1);
    convertScaleAbs(tmp2, tmp2);

    oMat = tmp1 + tmp2;
    if (oMat.channels() == 3) {
        cvtColor(oMat, oMat, COLOR_BGR2GRAY);
    }
    threshold(oMat, oMat, 130, 255, THRESH_BINARY | THRESH_OTSU);
}

void Prewitt(const cv::Mat &iMat, cv::Mat &oMat) {
    Mat kernel1 = (cv::Mat_<float>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
    Mat kernel2 = (cv::Mat_<float>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);

    Mat tmp1, tmp2;
    filter2D(iMat, tmp1, -1, kernel1);
    filter2D(iMat, tmp2, -1, kernel2);

    convertScaleAbs(tmp1, tmp1);
    convertScaleAbs(tmp2, tmp2);

    oMat = tmp1 + tmp2;
    if (oMat.channels() == 3) {
        cvtColor(oMat, oMat, COLOR_BGR2GRAY);
    }
    threshold(oMat, oMat, 130, 255, THRESH_BINARY | THRESH_OTSU);
}

void Kirsch(const cv::Mat &iMat, cv::Mat &oMat) {
    cv::Mat kernel1 = (cv::Mat_<float>(3, 3) << -3, -3, 5, -3, 0, 5, -3, -3, 5);
    cv::Mat kernel2 = (cv::Mat_<float>(3, 3) << 5, -3, -3, 5, 0, -3, 5, -3, -3);
    cv::Mat kernel3 = (cv::Mat_<float>(3, 3) << 5, 5, 5, -3, 0, -3, -3, -3, -3);
    cv::Mat kernel4 = (cv::Mat_<float>(3, 3) << -3, -3, -3, -3, 0, -3, 5, 5, 5);
    cv::Mat kernel5 = (cv::Mat_<float>(3, 3) << -3, 5, 5, -3, 0, 5, -3, -3, -3);
    cv::Mat kernel6 = (cv::Mat_<float>(3, 3) << 5, 5, -3, 5, 0, -3, -3, -3, -3);
    cv::Mat kernel7 = (cv::Mat_<float>(3, 3) << -3, -3, -3, -3, 0, 5, -3, 5, 5);
    cv::Mat kernel8 = (cv::Mat_<float>(3, 3) << -3, -3, -3, 5, 0, -3, 5, 5, -3);

    Mat tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
    filter2D(iMat, tmp1, -1, kernel1);
    filter2D(iMat, tmp2, -1, kernel2);
    filter2D(iMat, tmp3, -1, kernel3);
    filter2D(iMat, tmp4, -1, kernel4);
    filter2D(iMat, tmp5, -1, kernel5);
    filter2D(iMat, tmp6, -1, kernel6);
    filter2D(iMat, tmp7, -1, kernel7);
    filter2D(iMat, tmp8, -1, kernel8);

    convertScaleAbs(tmp1, tmp1);
    convertScaleAbs(tmp2, tmp2);
    convertScaleAbs(tmp3, tmp3);
    convertScaleAbs(tmp4, tmp4);
    convertScaleAbs(tmp5, tmp5);
    convertScaleAbs(tmp6, tmp6);
    convertScaleAbs(tmp7, tmp7);
    convertScaleAbs(tmp8, tmp8);

    oMat = tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7 + tmp8;
    if (oMat.channels() == 3) {
        cvtColor(oMat, oMat, COLOR_BGR2GRAY);
    }
    threshold(oMat, oMat, 130, 255, THRESH_BINARY | THRESH_OTSU);
}

void LOG(const cv::Mat &iMat, cv::Mat &oMat) {
    oMat = iMat.clone();
    cv::Mat kernel =
        (cv::Mat_<float>(5, 5) << 0, 0, 1, 0, 0, 0, 1, 2, 1, 0, 1, 2, -16, 2, 1, 0, 1, 2, 1, 0, 0, 0, 1, 0, 0);
    cv::filter2D(iMat, oMat, -1, kernel);

    if (oMat.channels() == 3) {
        cvtColor(oMat, oMat, COLOR_BGR2GRAY);
    }
    threshold(oMat, oMat, 130, 255, THRESH_BINARY | THRESH_OTSU);
}

void PointDetect(const cv::Mat &iMat, cv::Mat &oMat, int iType) {
    Mat kernel1 = (Mat_<float>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);
    Mat kernel2 = (Mat_<float>(3, 3) << -1, -1, -1, -1, 8, -1, -1, -1, -1);

    if (iType == 0) {
        filter2D(iMat, oMat, -1, kernel1);
    } else if (iType == 1) {
        filter2D(iMat, oMat, -1, kernel2);
    }

    if (oMat.channels() == 3) {
        cvtColor(oMat, oMat, COLOR_BGR2GRAY);
    }
    threshold(oMat, oMat, 130, 255, THRESH_BINARY | THRESH_OTSU);
}

void LineDetect(const cv::Mat &iMat, cv::Mat &oMat) {
    Mat kernel1 = (Mat_<float>(3, 3) << -1, -1, -1, 2, 2, 2, -1, -1, -1);
    Mat kernel2 = (Mat_<float>(3, 3) << -1, 2, -1, -1, 2, -1, -1, 2, -1);
    Mat kernel3 = (Mat_<float>(3, 3) << 2, -1, -1, -1, 2, -1, -1, -1, 2);
    Mat kernel4 = (Mat_<float>(3, 3) << -1, -1, 2, -1, 2, -1, 2, -1, -1);

    Mat tmp1, tmp2, tmp3, tmp4;
    filter2D(iMat, tmp1, -1, kernel1);
    filter2D(iMat, tmp2, -1, kernel2);
    filter2D(iMat, tmp3, -1, kernel3);
    filter2D(iMat, tmp4, -1, kernel4);

    convertScaleAbs(tmp1, tmp1);
    convertScaleAbs(tmp2, tmp2);
    convertScaleAbs(tmp3, tmp3);
    convertScaleAbs(tmp4, tmp4);

    oMat = tmp1 + tmp2 + tmp3 + tmp4;
    if (oMat.channels() == 3) {
        cvtColor(oMat, oMat, COLOR_BGR2GRAY);
    }
    threshold(oMat, oMat, 130, 255, THRESH_BINARY | THRESH_OTSU);
}
} // namespace ImgTools