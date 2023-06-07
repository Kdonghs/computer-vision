#include "opencv2/opencv.hpp"
#include <iostream>

int main(void) {
	double a = 0.3;
	
	cv::Mat result;
	cv::Mat cat = cv::imread("cat.bmp", cv::IMREAD_COLOR);
	cv::Mat fox = cv::imread("tibetfox.bmp", cv::IMREAD_COLOR);
	cat.copyTo(result);

	if (cat.empty()) {
		std::cout << "cat img load fail" << std::endl;
		return -1;
	}

	if (fox.empty()) {
		std::cout << "fox img load fail" << std::endl;
		return -1;
	}

	for (int y = 0; y < cat.rows; y++)
	{
		for (int x = 0; x < fox.cols; x++)
		{
			double b= 0, g = 0, r=0;
			b = cat.at<cv::Vec3b>(y, x)[0] * a + fox.at<cv::Vec3b>(y, x)[0] * (1-a);
			g = cat.at<cv::Vec3b>(y, x)[1] * a + fox.at<cv::Vec3b>(y, x)[1] * (1-a);
			r = cat.at<cv::Vec3b>(y, x)[2] * a + fox.at<cv::Vec3b>(y, x)[2] * (1-a);
			
			result.at<cv::Vec3b>(y, x)[0] = int(b);
			result.at<cv::Vec3b>(y, x)[1] = int(g);
			result.at<cv::Vec3b>(y, x)[2] = int(r);
		}
	}

	cv::imshow("dissolve_3", result);
	cv::waitKey(0);

	return 0;
}