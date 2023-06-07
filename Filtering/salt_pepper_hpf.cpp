#include "opencv2/opencv.hpp"
#include <iostream>

int main(void) {
	double a = 0.7;
	
	cv::Mat result;
	cv::Mat img = cv::imread("tibetfox.bmp", cv::IMREAD_GRAYSCALE);
	img.copyTo(result);

	if (img.empty()) {
		std::cout << "img load fail" << std::endl;
		return -1;
	}

	for (int y = 1; y < img.rows-1; y++)
	{
		for (int x = 1; x < img.cols-1; x++)
		{
			double temp=0;
			for (int i = -1; i < 2; i++)
			{
				for (int j = -1; j < 2; j++)
				{
					temp -= img.at<uchar>(y+i, x+j);
				}
			}
			temp += img.at<uchar>(y, x)*9;
			if (temp>255)
			{
				temp = 255;
			}
			else if (temp<0)
			{
				temp = 0;
			}
			result.at<uchar>(y, x) = unsigned char(temp);
		}
	}

	cv::imshow("salt_pepper_hpf", result);
	cv::waitKey(0);

	return 0;
}