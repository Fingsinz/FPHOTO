#include "imgProcess.h"
#include "util.h"
#include "iostream"
using namespace cv;

void imgBrightnessChange(Mat in, Mat &out, int factor)
{
	out = in.clone();
	if (in.channels() == 3)
		for (int i = 0; i < in.rows; i++)
			for (int j = 0; j < in.cols; j++)
			{
				out.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(in.at<Vec3b>(i, j)[0] + factor);
				out.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(in.at<Vec3b>(i, j)[1] + factor);
				out.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(in.at<Vec3b>(i, j)[2] + factor);
			}

	else if (in.channels() == 1)
		for (int i = 0; i < in.rows; i++)
			for (int j = 0; j < in.cols; j++)
				out.at<uchar>(i, j) = saturate_cast<uchar>(in.at<uchar>(i, j) + factor);
}

void imgContrastChange(Mat in, Mat &out, double factor)
{
	out = in.clone();
	if (in.channels() == 3)
	{
		in.convertTo(out, CV_64F, 1.0 / 255, 0);
		pow(out, factor, out);
		out.convertTo(out, CV_8UC3, 255, 0);
	}

	else if (in.channels() == 1)
	{
		in.convertTo(out, CV_64F, 1.0 / 255, 0);
		pow(out, factor, out);
		out.convertTo(out, CV_8U, 255, 0);
	}
}

void imgResize(Mat in, Mat &out, double factor)
{
	if (in.channels() == 3)
	{
		out = Mat(in.rows * factor, in.cols * factor, CV_8UC3);
		for (int i = 0; i < out.rows; i++)
			for (int j = 0; j < out.cols; j++)
				out.at<Vec3b>(i, j) = in.at<Vec3b>(i / factor, j / factor);
	}

	else if (in.channels() == 1)
	{
		out = Mat(in.rows * factor, in.cols * factor, CV_8UC1);
		for (int i = 0; i < out.rows; i++)
			for (int j = 0; j < out.cols; j++)
				out.at<uchar>(i, j) = in.at<uchar>(i / factor, j / factor);
	}
}

void imgRotate(Mat in, Mat &out, double angle)
{
	double alpha = -angle * CV_PI / 180.0;
	Point2f inp[3], outp[3];
	inp[0] = Point2f(0, in.rows);
	inp[1] = Point2f(in.cols, 0);
	inp[2] = Point2f(in.cols, in.rows);

	for (int i = 0; i < 3; i ++)
		outp[i] = Point2f(inp[i].x * cos(alpha) - inp[i].y * sin(alpha), inp[i].x * sin(alpha) + inp[i].y * cos(alpha));
	double minx, miny, maxx, maxy;

	minx = min(min(min(outp[0].x, outp[1].x), outp[2].x), double(0.0));
	miny = min(min(min(outp[0].y, outp[1].y), outp[2].y), double(0.0));
	maxx = max(max(max(outp[0].x, outp[1].x), outp[2].x), double(0.0));
	maxy = max(max(max(outp[0].y, outp[1].y), outp[2].y), double(0.0));

	int w = maxx - minx, h = maxy - miny;

	for (int i = 0; i < 3; i ++)
	{
		if (minx < 0)	outp[i].x -= minx;
		if (miny < 0)	outp[i].y -= miny;
	}

	Mat warpMat = getAffineTransform(inp, outp);
	warpAffine(in, out, warpMat, Size(w, h));
}

void imgHorizontalFlip(Mat in, Mat &out)
{
	out = in.clone();

	if (in.channels() == 3)
		for (int i = 0; i < in.rows; i++)
			for (int j = 0; j < in.cols; j++)
				out.at<Vec3b>(i, j) = in.at<Vec3b>(i, in.cols - j - 1);

	else if (in.channels() == 1)
		for (int i = 0; i < in.rows; i++)
			for (int j = 0; j < in.cols; j++)
				out.at<uchar>(i, j) = in.at<uchar>(i, in.cols - j - 1);
}

void imgVerticalFlip(Mat in, Mat &out)
{
	out = in.clone();

	if (in.channels() == 3)
		for (int i = 0; i < in.rows; i++)
			for (int j = 0; j < in.cols; j++)
				out.at<Vec3b>(i, j) = in.at<Vec3b>(in.rows - i - 1, j);

	else if (in.channels() == 1)
		for (int i = 0; i < in.rows; i++)
			for (int j = 0; j < in.cols; j++)
				out.at<uchar>(i, j) = in.at<uchar>(in.rows - i - 1, j);
}

void imgInversion(Mat in, Mat &out)
{
	out = in.clone();

	if (in.channels() == 3)
		for (int i = 0; i < in.rows; i++)
			for (int j = 0; j < in.cols; j++)
				out.at<Vec3b>(i, j) = Vec3b(255, 255, 255) - in.at<Vec3b>(i, j);

	else if (in.channels() == 1)
		for (int i = 0; i < in.rows; i++)
			for (int j = 0; j < in.cols; j++)
				out.at<uchar>(i, j) = 255 - in.at<uchar>(i, j);
}

void imgGrayscale(Mat in, Mat &out, bool type)
{
	if (in.channels() != 3)	return;

	if (type)
		out = Mat(in.size(), CV_8UC1, Scalar(0));
	else if (!type)
		out = in.clone();

	for (int i = 0; i < in.rows; i++)
	{
		for (int j = 0; j < in.cols; j++)
		{
			int tmp = in.at<Vec3b>(i, j)[0] + in.at<Vec3b>(i, j)[1] + in.at<Vec3b>(i, j)[2];
			if (type)
				out.at<uchar>(i, j) = tmp / 3;
			else if (!type)
				out.at<Vec3b>(i, j) = Vec3b(tmp / 3, tmp / 3, tmp / 3);
		}
	}
}

void drawHistogram(Mat in, Mat &out)
{
	int size = in.rows * in.cols;

	if (in.channels() == 3)
	{
		out = Mat(Size(512, 500), CV_8UC3, Scalar(0));
		float px_b[256]{ 0 }, px_g[256]{ 0 }, px_r[256]{ 0 };
		float maxp = 0;
		for (int i = 0; i < in.rows; i++)
		{
			for (int j = 0; j < in.cols; j++)
			{
				px_b[in.at<Vec3b>(i, j)[0]] += 1;
				px_g[in.at<Vec3b>(i, j)[1]] += 1;
				px_r[in.at<Vec3b>(i, j)[2]] += 1;
			}
		}

		for (int i = 0; i < 256; i ++)
		{
			maxp = max(maxp, max(px_b[i], max(px_g[i], px_r[i])));
		}

		for (int i = 0; i < 256; i++)
		{
			px_b[i] = px_b[i] / maxp * 500;
			px_g[i] = px_g[i] / maxp * 500;
			px_r[i] = px_r[i] / maxp * 500;
		}

		for (int i = 1; i < 256; i ++)
		{
			line(out, Point((i - 1) * 2, 500 - px_b[i - 1]), Point(i * 2, 500 - px_b[i]), Scalar(255, 0, 0), 2);
			line(out, Point((i - 1) * 2, 500 - px_g[i - 1]), Point(i * 2, 500 - px_g[i]), Scalar(0, 255, 0), 2);
			line(out, Point((i - 1) * 2, 500 - px_r[i - 1]), Point(i * 2, 500 - px_r[i]), Scalar(0, 0, 255), 2);
		}
	}

	if (in.channels() == 1)
	{
		out = Mat(Size(512, 500), CV_8UC1, Scalar(0));
		float px[256]{ 0 };
		float maxp = 0;
		for (int i = 0; i < in.rows; i++)
			for (int j = 0; j < in.cols; j++)
				px[in.at<uchar>(i, j)] += 1;

		for (int i = 0; i < 256; i ++)
			maxp = max(maxp, px[i]);

		for (int i = 0; i < 256; i++)
			px[i] = px[i] / maxp * 500;

		for (int i = 1; i < 256; i ++)
		{
			line(out, Point((i - 1) * 2, 500 - px[i - 1]), Point(i * 2, 500 - px[i]), Scalar(255), 2);
		}
	}
}

void imgHistEqualization(Mat in, Mat &out, Mat &hist)
{
	int size = in.rows * in.cols;

	if (in.channels() == 3)
	{
		out = in.clone();

		float px_b[256]{ 0 }, px_g[256]{ 0 }, px_r[256]{ 0 };
		float sk_b[256]{ 0 }, sk_g[256]{ 0 }, sk_r[256]{ 0 };
		int Sk_b[256]{ 0 }, Sk_g[256]{ 0 }, Sk_r[256]{ 0 };

		for (int i = 0; i < in.rows; i++)
		{
			for (int j = 0; j < in.cols; j++)
			{
				px_b[in.at<Vec3b>(i, j)[0]] += 1;
				px_g[in.at<Vec3b>(i, j)[1]] += 1;
				px_r[in.at<Vec3b>(i, j)[2]] += 1;
			}
		}

		sk_b[0] = px_b[0] / size, sk_g[0] = px_g[0] / size, sk_r[0] = px_r[0] / size;
		for (int i = 1; i < 256; i ++)
		{
			sk_b[i] = sk_b[i - 1] + px_b[i] / size;
			sk_g[i] = sk_g[i - 1] + px_g[i] / size;
			sk_r[i] = sk_r[i - 1] + px_r[i] / size;
		}

		for (int i = 0; i < 256; i ++)
		{
			Sk_b[i] = 255 * sk_b[i] + 0.5;
			Sk_g[i] = 255 * sk_g[i] + 0.5;
			Sk_r[i] = 255 * sk_r[i] + 0.5;
		}

		for (int i = 0; i < in.rows; i++)
		{
			for (int j = 0; j < in.cols; j++)
			{
				out.at<Vec3b>(i, j)[0] = Sk_b[in.at<Vec3b>(i, j)[0]];
				out.at<Vec3b>(i, j)[1] = Sk_g[in.at<Vec3b>(i, j)[1]];
				out.at<Vec3b>(i, j)[2] = Sk_r[in.at<Vec3b>(i, j)[2]];
			}
		}

		hist = Mat(Size(512, 500), CV_8UC3, Scalar(0));
		float _px_b[256]{ 0 }, _px_g[256]{ 0 }, _px_r[256]{ 0 };
		float maxp = 0;
		for (int i = 0; i < out.rows; i++)
		{
			for (int j = 0; j < out.cols; j++)
			{
				_px_b[out.at<Vec3b>(i, j)[0]] += 1;
				_px_g[out.at<Vec3b>(i, j)[1]] += 1;
				_px_r[out.at<Vec3b>(i, j)[2]] += 1;
			}
		}

		for (int i = 0; i < 256; i ++)
			maxp = max(maxp, max(_px_b[i], max(_px_g[i], _px_r[i])));

		for (int i = 0; i < 256; i++)
		{
			_px_b[i] = _px_b[i] / maxp * 500;
			_px_g[i] = _px_g[i] / maxp * 500;
			_px_r[i] = _px_r[i] / maxp * 500;
		}

		for (int i = 1; i < 256; i ++)
		{
			line(hist, Point((i - 1) * 2, 500 - _px_b[i - 1]), Point(i * 2, 500 - _px_b[i]), Scalar(255, 0, 0), 2);
			line(hist, Point((i - 1) * 2, 500 - _px_g[i - 1]), Point(i * 2, 500 - _px_g[i]), Scalar(0, 255, 0), 2);
			line(hist, Point((i - 1) * 2, 500 - _px_r[i - 1]), Point(i * 2, 500 - _px_r[i]), Scalar(0, 0, 255), 2);
		}
	}

	else if (in.channels() == 1)
	{
		equalizeHist(in, out);
		hist = Mat(Size(512, 500), CV_8UC1, Scalar(0));
		float px[256]{ 0 };
		float maxp = 0;
		for (int i = 0; i < out.rows; i++)
			for (int j = 0; j < out.cols; j++)
				px[out.at<uchar>(i, j)] += 1;

		for (int i = 0; i < 256; i ++)
			maxp = max(maxp, px[i]);

		for (int i = 0; i < 256; i++)
			px[i] = px[i] / maxp * 500;

		for (int i = 1; i < 256; i ++)
		{
			line(hist, Point((i - 1) * 2, 500 - px[i - 1]), Point(i * 2, 500 - px[i]), Scalar(255), 2);
		}
	}
}

void imgArithmeticAverageFiltering(Mat in, Mat &out, int ksize)
{
	out = in.clone();
	copyMakeBorder(in, in, (ksize - 1) / 2, (ksize - 1) / 2, (ksize - 1) / 2, (ksize - 1) / 2, BORDER_REFLECT);

	if (in.channels() == 3)
	{
		for (int i = (ksize - 1) / 2; i < in.rows - (ksize - 1) / 2; i ++)
			for (int j = (ksize - 1) / 2; j < in.cols - (ksize - 1) / 2; j ++)
			{
				int sum1 = 0, sum2 = 0, sum3 = 0;
				for (int x = -(ksize - 1) / 2; x <= (ksize - 1) / 2; x++)
					for (int y = -(ksize - 1) / 2; y <= (ksize - 1) / 2; y ++)
					{
						sum1 += in.at<Vec3b>(i + x, j + y)[0];
						sum2 += in.at<Vec3b>(i + x, j + y)[1];
						sum3 += in.at<Vec3b>(i + x, j + y)[2];
					}

				out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[0] = round(sum1 / (ksize * ksize));
				out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[1] = round(sum2 / (ksize * ksize));
				out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[2] = round(sum3 / (ksize * ksize));
			}
	}
	else if (in.channels() == 1)
	{
		for (int i = (ksize - 1) / 2; i < in.rows - (ksize - 1) / 2; i ++)
			for (int j = (ksize - 1) / 2; j < in.cols - (ksize - 1) / 2; j ++)
			{
				int sum = 0;
				for (int x = -(ksize - 1) / 2; x <= (ksize - 1) / 2; x++)
					for (int y = -(ksize - 1) / 2; y <= (ksize - 1) / 2; y ++)
						sum += in.at<uchar>(i + x, j + y);
				out.at<uchar>(i - (ksize - 1) / 2, j - (ksize - 1) / 2) = round(sum / (ksize * ksize));
			}
	}
}

void imgGeometricAverageFiltering(Mat in, Mat &out, int ksize)
{
	out = in.clone();
	copyMakeBorder(in, in, (ksize - 1) / 2, (ksize - 1) / 2, (ksize - 1) / 2, (ksize - 1) / 2, BORDER_REFLECT);

	if (in.channels() == 3)
	{
		for (int i = (ksize - 1) / 2; i < in.rows - (ksize - 1) / 2; i ++)
			for (int j = (ksize - 1) / 2; j < in.cols - (ksize - 1) / 2; j ++)
			{
				double sum1 = 0, sum2 = 0, sum3 = 0;
				for (int x = -(ksize - 1) / 2; x <= (ksize - 1) / 2; x++)
					for (int y = -(ksize - 1) / 2; y <= (ksize - 1) / 2; y ++)
					{
						sum1 += log10(in.at<Vec3b>(i + x, j + y)[0] + 0.1);
						sum2 += log10(in.at<Vec3b>(i + x, j + y)[1] + 0.1);
						sum3 += log10(in.at<Vec3b>(i + x, j + y)[2] + 0.1);
					}
				sum1 /= ksize * ksize, sum2 /= ksize * ksize, sum3 /= ksize * ksize;
				out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[0] = round(pow(10, sum1));
				out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[1] = round(pow(10, sum2));
				out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[2] = round(pow(10, sum3));
			}
	}
	else if (in.channels() == 1)
	{
		for (int i = (ksize - 1) / 2; i < in.rows - (ksize - 1) / 2; i ++)
			for (int j = (ksize - 1) / 2; j < in.cols - (ksize - 1) / 2; j ++)
			{
				double sum = 0;
				for (int x = -(ksize - 1) / 2; x <= (ksize - 1) / 2; x++)
					for (int y = -(ksize - 1) / 2; y <= (ksize - 1) / 2; y ++)
						sum += log10(in.at<uchar>(i + x, j + y) + 0.1);
				sum /= ksize * ksize;
				out.at<uchar>(i - (ksize - 1) / 2, j - (ksize - 1) / 2) = round(pow(10, sum));
			}
	}
}

void imgHarmonicMeanFiltering(Mat in, Mat &out, int ksize)
{
	out = in.clone();
	copyMakeBorder(in, in, (ksize - 1) / 2, (ksize - 1) / 2, (ksize - 1) / 2, (ksize - 1) / 2, BORDER_REFLECT);

	if (in.channels() == 3)
	{
		for (int i = (ksize - 1) / 2; i < in.rows - (ksize - 1) / 2; i ++)
			for (int j = (ksize - 1) / 2; j < in.cols - (ksize - 1) / 2; j ++)
			{
				double sum1 = 0, sum2 = 0, sum3 = 0;
				for (int x = -(ksize - 1) / 2; x <= (ksize - 1) / 2; x++)
					for (int y = -(ksize - 1) / 2; y <= (ksize - 1) / 2; y ++)
					{
						sum1 += 1.0 / (in.at<Vec3b>(i + x, j + y)[0] + 0.1);
						sum2 += 1.0 / (in.at<Vec3b>(i + x, j + y)[1] + 0.1);
						sum3 += 1.0 / (in.at<Vec3b>(i + x, j + y)[2] + 0.1);
					}
				sum1 = ksize * ksize / sum1, sum2 = ksize * ksize / sum2, sum3 = ksize * ksize / sum3;
				out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[0] = sum1;
				out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[1] = sum2;
				out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[2] = sum3;
			}
	}
	else if (in.channels() == 1)
	{
		for (int i = (ksize - 1) / 2; i < in.rows - (ksize - 1) / 2; i ++)
			for (int j = (ksize - 1) / 2; j < in.cols - (ksize - 1) / 2; j ++)
			{
				double sum = 0;
				for (int x = -(ksize - 1) / 2; x <= (ksize - 1) / 2; x++)
					for (int y = -(ksize - 1) / 2; y <= (ksize - 1) / 2; y ++)
						sum += 1.0 / (in.at<uchar>(i + x, j + y) + 0.1);
				sum = ksize * ksize / sum;
				out.at<uchar>(i - (ksize - 1) / 2, j - (ksize - 1) / 2) = sum;
			}
	}
}

void imgMedianFiltering(Mat in, Mat &out, int ksize)
{
	out = in.clone();

	if (in.channels() == 3)
	{
		std::vector<uchar> arry_b(ksize * ksize);
		std::vector<uchar> arry_g(ksize * ksize);
		std::vector<uchar> arry_r(ksize * ksize);

		copyMakeBorder(in, in, (ksize - 1) / 2, (ksize - 1) / 2, (ksize - 1) / 2, (ksize - 1) / 2, BORDER_REFLECT);

		for (int i = (ksize - 1) / 2; i < in.rows - (ksize - 1) / 2; i ++)
			for (int j = (ksize - 1) / 2; j < in.cols - (ksize - 1) / 2; j ++)
			{
				int h = 0;
				for (int x = -(ksize - 1) / 2; x <= (ksize - 1) / 2; x++)
					for (int y = -(ksize - 1) / 2; y <= (ksize - 1) / 2; y ++)
					{
						arry_b[h] = in.at<Vec3b>(i + x, j + y)[0];
						arry_g[h] = in.at<Vec3b>(i + x, j + y)[1];
						arry_r[h] = in.at<Vec3b>(i + x, j + y)[2];
						h += 1;
					}
				std::sort(arry_b.begin(), arry_b.end());
				std::sort(arry_g.begin(), arry_g.end());
				std::sort(arry_r.begin(), arry_r.end());
				out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[0] = arry_b[(ksize * ksize - 1) / 2];
				out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[1] = arry_g[(ksize * ksize - 1) / 2];
				out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[2] = arry_r[(ksize * ksize - 1) / 2];
			}
	}

	if (in.channels() == 1)
	{
		std::vector<uchar> arry(ksize * ksize);

		copyMakeBorder(in, in, (ksize - 1) / 2, (ksize - 1) / 2, (ksize - 1) / 2, (ksize - 1) / 2, BORDER_REFLECT);

		for (int i = (ksize - 1) / 2; i < in.rows - (ksize - 1) / 2; i ++)
			for (int j = (ksize - 1) / 2; j < in.cols - (ksize - 1) / 2; j ++)
			{
				int h = 0;
				for (int x = -(ksize - 1) / 2; x <= (ksize - 1) / 2; x++)
					for (int y = -(ksize - 1) / 2; y <= (ksize - 1) / 2; y ++)
					{
						arry[h] = in.at<uchar>(i + x, j + y);
						h += 1;
					}
				std::sort(arry.begin(), arry.end());
				out.at<uchar>(i - (ksize - 1) / 2, j - (ksize - 1) / 2) = arry[(ksize * ksize - 1) / 2];
			}
	}
}

void imgMaxMinFiltering(Mat in, Mat &out, int ksize, int type)
{
	out = in.clone();

	if (in.channels() == 3)
	{
		std::vector<uchar> arry_b(ksize * ksize);
		std::vector<uchar> arry_g(ksize * ksize);
		std::vector<uchar> arry_r(ksize * ksize);

		copyMakeBorder(in, in, (ksize - 1) / 2, (ksize - 1) / 2, (ksize - 1) / 2, (ksize - 1) / 2, BORDER_REFLECT);

		for (int i = (ksize - 1) / 2; i < in.rows - (ksize - 1) / 2; i ++)
			for (int j = (ksize - 1) / 2; j < in.cols - (ksize - 1) / 2; j ++)
			{
				int h = 0;
				for (int x = -(ksize - 1) / 2; x <= (ksize - 1) / 2; x++)
					for (int y = -(ksize - 1) / 2; y <= (ksize - 1) / 2; y ++)
					{
						arry_b[h] = in.at<Vec3b>(i + x, j + y)[0];
						arry_g[h] = in.at<Vec3b>(i + x, j + y)[1];
						arry_r[h] = in.at<Vec3b>(i + x, j + y)[2];
						h += 1;
					}
				std::sort(arry_b.begin(), arry_b.end());
				std::sort(arry_g.begin(), arry_g.end());
				std::sort(arry_r.begin(), arry_r.end());
				if (type == 1)
				{
					out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[0] = arry_b[ksize * ksize - 1];
					out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[1] = arry_g[ksize * ksize - 1];
					out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[2] = arry_r[ksize * ksize - 1];
				}
				else if (type == 0)
				{
					out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[0] = arry_b[0];
					out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[1] = arry_g[0];
					out.at<Vec3b>(i - (ksize - 1) / 2, j - (ksize - 1) / 2)[2] = arry_r[0];
				}
			}
	}

	else if (in.channels() == 1)
	{
		std::vector<uchar> arry(ksize * ksize);
		copyMakeBorder(in, in, (ksize - 1) / 2, (ksize - 1) / 2, (ksize - 1) / 2, (ksize - 1) / 2, BORDER_REFLECT);
		for (int i = (ksize - 1) / 2; i < in.rows - (ksize - 1) / 2; i ++)
			for (int j = (ksize - 1) / 2; j < in.cols - (ksize - 1) / 2; j ++)
			{
				int h = 0;
				for (int x = -(ksize - 1) / 2; x <= (ksize - 1) / 2; x++)
					for (int y = -(ksize - 1) / 2; y <= (ksize - 1) / 2; y ++)
					{
						arry[h] = in.at<uchar>(i + x, j + y);
						h += 1;
					}
				std::sort(arry.begin(), arry.end());
				if (type == 1)
					out.at<uchar>(i - (ksize - 1) / 2, j - (ksize - 1) / 2) = arry[ksize * ksize - 1];
				else if (type == 0)
					out.at<uchar>(i - (ksize - 1) / 2, j - (ksize - 1) / 2) = arry[0];
			}
	}
}

void imgLaplaceFiltering(Mat in, Mat &out, int type)
{
	out = in.clone();

	if (in.channels() == 3)
	{
		int la[3];
		for (int i = 1; i < in.rows - 1; i ++)
			for (int j = 1; j < in.cols - 1; j ++)
			{
				if (type == 0)
				{
					la[0] = in.at<Vec3b>(i + 1, j)[0] + in.at<Vec3b>(i - 1, j)[0] + in.at<Vec3b>(i, j + 1)[0] + in.at<Vec3b>(i, j - 1)[0] - 5 * in.at<Vec3b>(i, j)[0];
					la[1] = in.at<Vec3b>(i + 1, j)[1] + in.at<Vec3b>(i - 1, j)[1] + in.at<Vec3b>(i, j + 1)[1] + in.at<Vec3b>(i, j - 1)[1] - 5 * in.at<Vec3b>(i, j)[1];
					la[2] = in.at<Vec3b>(i + 1, j)[2] + in.at<Vec3b>(i - 1, j)[2] + in.at<Vec3b>(i, j + 1)[2] + in.at<Vec3b>(i, j - 1)[2] - 5 * in.at<Vec3b>(i, j)[2];
				}
				else if (type == 1)
				{
					la[0] = in.at<Vec3b>(i - 1, j - 1)[0] + in.at<Vec3b>(i - 1, j)[0] + in.at<Vec3b>(i - 1, j + 1)[0] + in.at<Vec3b>(i, j - 1)[0] + in.at<Vec3b>(i, j + 1)[0] + in.at<Vec3b>(i + 1, j - 1)[0] + in.at<Vec3b>(i + 1, j)[0] + in.at<Vec3b>(i + 1, j + 1)[0] - 9 * in.at<Vec3b>(i, j)[0];
					la[1] = in.at<Vec3b>(i - 1, j - 1)[1] + in.at<Vec3b>(i - 1, j)[1] + in.at<Vec3b>(i - 1, j + 1)[1] + in.at<Vec3b>(i, j - 1)[1] + in.at<Vec3b>(i, j + 1)[1] + in.at<Vec3b>(i + 1, j - 1)[1] + in.at<Vec3b>(i + 1, j)[1] + in.at<Vec3b>(i + 1, j + 1)[1] - 9 * in.at<Vec3b>(i, j)[1];
					la[2] = in.at<Vec3b>(i - 1, j - 1)[2] + in.at<Vec3b>(i - 1, j)[2] + in.at<Vec3b>(i - 1, j + 1)[2] + in.at<Vec3b>(i, j - 1)[2] + in.at<Vec3b>(i, j + 1)[2] + in.at<Vec3b>(i + 1, j - 1)[2] + in.at<Vec3b>(i + 1, j)[2] + in.at<Vec3b>(i + 1, j + 1)[2] - 9 * in.at<Vec3b>(i, j)[2];
				}
				out.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(-la[0]);
				out.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(-la[1]);
				out.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(-la[2]);
			}
	}

	else if (in.channels() == 1)
	{
		int la;
		for (int i = 1; i < in.rows - 1; i ++)
			for (int j = 1; j < in.cols - 1; j ++)
			{
				if (type == 0)
					la = in.at<uchar>(i + 1, j) + in.at<uchar>(i - 1, j)
					+ in.at<uchar>(i, j + 1) + in.at<uchar>(i, j - 1)
					- 5 * in.at<uchar>(i, j);
				else if (type == 1)
					la = in.at<uchar>(i - 1, j - 1) + in.at<uchar>(i - 1, j) + in.at<uchar>(i - 1, j + 1) + in.at<uchar>(i, j - 1) + in.at<uchar>(i, j + 1) + in.at<uchar>(i + 1, j - 1) + in.at<uchar>(i + 1, j) + in.at<uchar>(i + 1, j + 1) - 9 * in.at<uchar>(i, j);

				out.at<uchar>(i, j) = saturate_cast<uchar>(-la);
			}
	}
}

void imgDilationOrErosion(Mat in, Mat &out, int kernelSize, int kernelType, int op, int cnt)
{
	Mat kernel = getStructuringElement(kernelType, Size(kernelSize, kernelSize));
	if (op == 0)
		dilate(in, out, kernel, Point(-1, -1), cnt);
	else
		erode(in, out, kernel, Point(-1, -1), cnt);
}

void imgOpeningOrClosing(Mat in, Mat &out, int kernelSize, int kernelType, int op, int cnt)
{
	Mat kernel = getStructuringElement(kernelType, Size(kernelSize, kernelSize));
	morphologyEx(in, out, op, kernel, Point(-1, -1), cnt);
}

void imgTopOrBottomHatTrans(Mat in, Mat &out, int kernelSize, int kernelType, int op, int cnt)
{
	Mat kernel = getStructuringElement(kernelType, Size(kernelSize, kernelSize));
	morphologyEx(in, out, op, kernel, Point(-1, -1), cnt);
}

void imgThresholdSegment(Mat in, Mat &out, int T)
{
	Mat inGray;
	if (in.channels() == 3)
		cvtColor(in, inGray, COLOR_BGR2GRAY);
	else if (in.channels() == 1)
		inGray = in.clone();

	threshold(inGray, out, T, 255, THRESH_BINARY);
}

void imgIterativeThresholdSegment(Mat in, Mat &out, int &T)
{
	Mat inGray;
	if (in.channels() == 3)
		cvtColor(in, inGray, COLOR_BGR2GRAY);
	else if (in.channels() == 1)
		inGray = in.clone();

	double th = 0.5;
	T = 0;
	double preT = 0;

	for (int i = 0; i < in.rows; i ++)
		for (int j = 0; j < in.cols; j ++)
			T += in.at<uchar>(i, j);

	T = 1.0 * T / (in.rows * in.cols);

	while (std::abs(preT - T) > th)
	{
		preT = T;
		int mean1 = 0, mean2 = 0, cnt1 = 0, cnt2 = 0;
		for (int i = 0; i < in.rows; i ++)
			for (int j = 0; j < in.cols; j ++)
			{
				if (in.at<uchar>(i, j) > T)
					mean1 += in.at<uchar>(i, j), cnt1 ++;
				else
					mean2 += in.at<uchar>(i, j), cnt2 ++;
			}

		mean1 = 1.0 * mean1 / cnt1;
		mean2 = 1.0 * mean2 / cnt2;
		T = (mean1 + mean2) / 2;
	}
	threshold(inGray, out, T, 255, THRESH_BINARY);
}

void imgOTSUThresholdSegment(Mat in, Mat &out, int &T)
{
	Mat inGray;
	cvtColor(in, inGray, COLOR_BGR2GRAY);

	// 计数灰度直方图各像素值
	int hist[256] = { 0 };
	for (int i = 0; i < in.rows; i ++)
		for (int j = 0; j < in.cols; j ++)
			hist[in.at<uchar>(i, j)] ++;

	// 计算灰度直方图各像素概率
	double phist[256] = { 0 };
	for (int i = 0; i < 256; i ++)
		phist[i] = 1.0 * hist[i] / (in.rows * in.cols);

	double zeroCumuMoment[256] = { 0 }, oneCumuMoment[256] = { 0 };

	// 计算灰度直方图的零阶累积距和一阶累积距
	zeroCumuMoment[0] = phist[0];
	oneCumuMoment[0] = 0 * phist[0];
	for (int i = 1; i < 256; i ++)
	{
		zeroCumuMoment[i] += zeroCumuMoment[i - 1] + phist[i];
		oneCumuMoment[i] = oneCumuMoment[i - 1] + i * phist[i];
	}

	double mean = oneCumuMoment[255];

	double _max = -1;
	T = 0;

	for (int i = 0; i < 256; i ++)
	{
		double tmp = mean * zeroCumuMoment[i] - oneCumuMoment[i];
		double tmp1 = zeroCumuMoment[i] * (1 - zeroCumuMoment[i]);
		double ans = tmp * tmp / tmp1;
		if (_max < ans)
		{
			_max = ans;
			T = i;
		}
	}
	threshold(inGray, out, T, 255, THRESH_BINARY);
}

void imgRegionGrowth(Mat in, Mat &out, int T)
{
	Point pt = Point(in.cols / 2, in.rows / 2);			// 开始生长点
	Point waitGrowing;									// 待生长点位置
	int growLable = 0;									// 标记是否生长过
	int startValue = 0;									// 生长起点灰度值
	int nowValue = 0;									// 当前生长点灰度值
	out = Mat(in.size(), CV_8UC1, Scalar(0));			// 创建一个空白区域，填充为黑色
	std::vector<Point> GrowPts;							// 生长点栈
	GrowPts.push_back(pt);								// 将生长点压入栈中
	out.at<uchar>(pt.y, pt.x) = 255;					// 标记生长点
	startValue = in.at<uchar>(pt.y, pt.x);				// 记录生长点的灰度值
	int DIR[8][2] = {									// 生长方向顺序
		{ -1, -1 }, { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 } };

	while (!GrowPts.empty())
	{
		// 取出一个生长点
		pt = GrowPts.back();
		GrowPts.pop_back();

		//分别对八个方向上的点进行生长
		for (int i = 0; i < 9; ++i)
		{
			waitGrowing.x = pt.x + DIR[i][0];
			waitGrowing.y = pt.y + DIR[i][1];

			//检查是否是边缘点
			if (waitGrowing.x < 0 || waitGrowing.y < 0 || waitGrowing.x >(in.cols - 1) || (waitGrowing.y > in.rows - 1))
				continue;

			growLable = out.at<uchar>(waitGrowing.y, waitGrowing.x);        //当前待生长点的灰度值

			if (growLable == 0)                    //如果标记点还没有被生长
			{
				nowValue = in.at<uchar>(waitGrowing.y, waitGrowing.x);
				if (abs(startValue - nowValue) < T)                    //在阈值范围内则生长
				{
					out.at<uchar>(waitGrowing.y, waitGrowing.x) = 255;        //标记为白色
					GrowPts.push_back(waitGrowing);                    //将下一个生长点压入栈中
				}
			}
		}
	}
}

void sobel(Mat in, Mat &out)
{
	Mat kernel1 = (cv::Mat_<float>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
	Mat kernel2 = (cv::Mat_<float>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);

	Mat tmp1, tmp2;
	filter2D(in, tmp1, -1, kernel1);
	filter2D(in, tmp2, -1, kernel2);

	convertScaleAbs(tmp1, tmp1);
	convertScaleAbs(tmp2, tmp2);

	out = tmp1 + tmp2;

	if (out.channels() == 3)
		cvtColor(out, out, COLOR_BGR2GRAY);
	threshold(out, out, 130, 255, THRESH_BINARY | THRESH_OTSU);
}

void roberts(Mat in, Mat &out)
{
	Mat kernel1 = (cv::Mat_<float>(2, 2) << -1, 0, 0, 1);
	Mat kernel2 = (cv::Mat_<float>(2, 2) << 0, -1, 1, 0);

	Mat tmp1, tmp2;
	filter2D(in, tmp1, -1, kernel1);
	filter2D(in, tmp2, -1, kernel2);

	convertScaleAbs(tmp1, tmp1);
	convertScaleAbs(tmp2, tmp2);

	out = tmp1 + tmp2;
	if (out.channels() == 3)
		cvtColor(out, out, COLOR_BGR2GRAY);
	threshold(out, out, 130, 255, THRESH_BINARY | THRESH_OTSU);
}

void prewitt(Mat in, Mat &out)
{
	Mat kernel1 = (cv::Mat_<float>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
	Mat kernel2 = (cv::Mat_<float>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);

	Mat tmp1, tmp2;
	filter2D(in, tmp1, -1, kernel1);
	filter2D(in, tmp2, -1, kernel2);

	convertScaleAbs(tmp1, tmp1);
	convertScaleAbs(tmp2, tmp2);

	out = tmp1 + tmp2;
	if (out.channels() == 3)
		cvtColor(out, out, COLOR_BGR2GRAY);
	threshold(out, out, 130, 255, THRESH_BINARY | THRESH_OTSU);
}

void kirsch(Mat in, Mat &out)
{
	cv::Mat kernel1 = (cv::Mat_<float>(3, 3) << -3, -3, 5, -3, 0, 5, -3, -3, 5);
	cv::Mat kernel2 = (cv::Mat_<float>(3, 3) << 5, -3, -3, 5, 0, -3, 5, -3, -3);
	cv::Mat kernel3 = (cv::Mat_<float>(3, 3) << 5, 5, 5, -3, 0, -3, -3, -3, -3);
	cv::Mat kernel4 = (cv::Mat_<float>(3, 3) << -3, -3, -3, -3, 0, -3, 5, 5, 5);
	cv::Mat kernel5 = (cv::Mat_<float>(3, 3) << -3, 5, 5, -3, 0, 5, -3, -3, -3);
	cv::Mat kernel6 = (cv::Mat_<float>(3, 3) << 5, 5, -3, 5, 0, -3, -3, -3, -3);
	cv::Mat kernel7 = (cv::Mat_<float>(3, 3) << -3, -3, -3, -3, 0, 5, -3, 5, 5);
	cv::Mat kernel8 = (cv::Mat_<float>(3, 3) << -3, -3, -3, 5, 0, -3, 5, 5, -3);

	Mat tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
	filter2D(in, tmp1, -1, kernel1); filter2D(in, tmp2, -1, kernel2);
	filter2D(in, tmp3, -1, kernel3); filter2D(in, tmp4, -1, kernel4);
	filter2D(in, tmp5, -1, kernel5); filter2D(in, tmp6, -1, kernel6);
	filter2D(in, tmp7, -1, kernel7); filter2D(in, tmp8, -1, kernel8);

	convertScaleAbs(tmp1, tmp1); convertScaleAbs(tmp2, tmp2);
	convertScaleAbs(tmp3, tmp3); convertScaleAbs(tmp4, tmp4);
	convertScaleAbs(tmp5, tmp5); convertScaleAbs(tmp6, tmp6);
	convertScaleAbs(tmp7, tmp7); convertScaleAbs(tmp8, tmp8);

	out = tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7 + tmp8;
	if (out.channels() == 3)
		cvtColor(out, out, COLOR_BGR2GRAY);
	threshold(out, out, 130, 255, THRESH_BINARY | THRESH_OTSU);
}

void LOG(Mat in, Mat &out)
{
	out = in.clone();
	cv::Mat kernel = (cv::Mat_<float>(5, 5) <<
		0, 0, 1, 0, 0,
		0, 1, 2, 1, 0,
		1, 2, -16, 2, 1,
		0, 1, 2, 1, 0,
		0, 0, 1, 0, 0);
	cv::filter2D(in, out, -1, kernel);
	if (out.channels() == 3)
		cvtColor(out, out, COLOR_BGR2GRAY);
	threshold(out, out, 130, 255, THRESH_BINARY | THRESH_OTSU);
}

void imgPointDetect(Mat in, Mat &out, int type)
{
	Mat kernel1 = (Mat_<float>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);
	Mat kernel2 = (Mat_<float>(3, 3) << -1, -1, -1, -1, 8, -1, -1, -1, -1);

	if (type == 0)
		filter2D(in, out, -1, kernel1);
	else if (type == 1)
		filter2D(in, out, -1, kernel2);

	if (out.channels() == 3)
		cvtColor(out, out, COLOR_BGR2GRAY);
	threshold(out, out, 130, 255, THRESH_BINARY | THRESH_OTSU);
}

void imgLineDetect(Mat in, Mat &out)
{
	Mat kernel1 = (Mat_<float>(3, 3) << -1, -1, -1, 2, 2, 2, -1, -1, -1);
	Mat kernel2 = (Mat_<float>(3, 3) << -1, 2, -1, -1, 2, -1, -1, 2, -1);
	Mat kernel3 = (Mat_<float>(3, 3) << 2, -1, -1, -1, 2, -1, -1, -1, 2);
	Mat kernel4 = (Mat_<float>(3, 3) << -1, -1, 2, -1, 2, -1, 2, -1, -1);

	Mat tmp1, tmp2, tmp3, tmp4;
	filter2D(in, tmp1, -1, kernel1);
	filter2D(in, tmp2, -1, kernel2);
	filter2D(in, tmp3, -1, kernel3);
	filter2D(in, tmp4, -1, kernel4);

	convertScaleAbs(tmp1, tmp1);
	convertScaleAbs(tmp2, tmp2);
	convertScaleAbs(tmp3, tmp3);
	convertScaleAbs(tmp4, tmp4);

	out = tmp1 + tmp2 + tmp3 + tmp4;
	if (out.channels() == 3)
		cvtColor(out, out, COLOR_BGR2GRAY);
	threshold(out, out, 130, 255, THRESH_BINARY | THRESH_OTSU);
}