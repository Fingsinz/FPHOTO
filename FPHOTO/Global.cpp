#include "Global.h"

void Global::keepTrace(cv::Mat &now)
{
	if (imgs.size() == 10)	imgs.pop_front();
	imgs.push_back(now.clone());
}