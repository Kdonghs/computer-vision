#include "opencv2/opencv.hpp"
#include <iostream>

int main(void) {
	double a = 0.7;
	
	cv::Mat result;
	cv::Mat img = cv::imread("salt_pepper.bmp", cv::IMREAD_GRAYSCALE);
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
					temp += img.at<uchar>(y+i, x+j) / 9;
				}
			}
			result.at<uchar>(y, x) = int(temp);
		}
	}

	cv::imshow("salt_pepper_lpf", result);
	cv::waitKey(0);

	return 0;
}