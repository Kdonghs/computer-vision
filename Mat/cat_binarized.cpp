#include "opencv2/opencv.hpp"
#include <iostream>

int main(void) {
	cv::Mat result_img;
	cv::Mat img = cv::imread("cat.bmp", cv::IMREAD_GRAYSCALE);

	img.copyTo(result_img);

	if (img.empty()) {
		std::cout << "img load fail" << std::endl;
		return -1;
	}

	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{
			int g = img.at<uchar>(y, x);

			if (g<128)
			{
				g = 0;
			}
			else {
				g = 255;
			}
			result_img.at<uchar>(y, x) = cv::saturate_cast<uchar>(g);
		}
	}

	cv::imshow("cat_binarized", result_img);
	cv::waitKey(0);

	return 0;
}