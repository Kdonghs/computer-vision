#include "opencv2/opencv.hpp"
#include <iostream>

int main(void) {
	cv::Mat result_img;
	cv::Mat img = cv::imread("cat.bmp", cv::IMREAD_COLOR);

	img.copyTo(result_img);

	if (img.empty()) {
		std::cout << "img load fail" << std::endl;
		return -1;
	}

	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{
			int b = img.at<cv::Vec3b>(y, x)[0] - 255;
			int g = img.at<cv::Vec3b>(y, x)[1] - 255;
			int r = img.at<cv::Vec3b>(y, x)[2] - 255;

			if (b<0)
			{
				b *= -1;
			}

			if (g < 0)
			{
				g *= -1;
			}

			if (r < 0)
			{
				r *= -1;
			}
			result_img.at<cv::Vec3b>(y, x)[0] = cv::saturate_cast<uchar>(b);
			result_img.at<cv::Vec3b>(y, x)[1] = cv::saturate_cast<uchar>(g);
			result_img.at<cv::Vec3b>(y, x)[2] = cv::saturate_cast<uchar>(r);
		}
	}

	cv::imshow("cat_inverted", result_img);
	cv::waitKey(0);

	return 0;
}