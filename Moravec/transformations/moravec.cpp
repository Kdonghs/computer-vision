#include <algorithm>
#include "moravec.hpp"

transformations::Moravec::Moravec(cv::InputArray& original_image)
{
	cv::Mat src = original_image.getMat();
	confidence_map.create(src.size(), CV_32FC1);

	// 외곽 2개 픽셀의 Confidence는 0으로 가정
	for (int w = 0; w < src.cols; ++w)
	{
		confidence_map.at<float>(0, w) = 0.0;
		confidence_map.at<float>(1, w) = 0.0;
		confidence_map.at<float>(src.rows - 1, w) = 0.0;
		confidence_map.at<float>(src.rows - 2, w) = 0.0;
	}

	for (int h = 0; h < src.rows; ++h)
	{
		confidence_map.at<float>(h, 0) = 0.0;
		confidence_map.at<float>(h, 1) = 0.0;
		confidence_map.at<float>(h, src.cols - 1) = 0.0;
		confidence_map.at<float>(h, src.cols - 2) = 0.0;
	}
}


void transformations::Moravec::FindConfidenceMap(cv::InputArray& original_image)
{
	cv::Mat src = original_image.getMat();
	
	/**
	 * src를 이용하여 모라벡 알고리즘의 Confidence를 계산한 뒤
	 * 이를 confidence_map에 저장하세요.
	 * (confidence_map은 클래스 멤버 변수로 선언되어 있습니다.)
	 */
	// ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
	for (int y = 2; y < src.rows-2; y++)
	{
		for (int x = 2; x < src.cols-2; x++)
		{
			int up = 0;
			int down = 0;
			int left = 0;
			int right = 0;

			for (int j = y-1; j < y+1; j++)
			{
				for (int i = x-1; i < x+1; i++)
				{
					up += pow(src.at<uchar>(j + 1, i) - src.at<uchar>(j, i),2);
					down += pow(src.at<uchar>(j - 1, i) - src.at<uchar>(j, i), 2);
					left += pow(src.at<uchar>(j, i-1) - src.at<uchar>(j, i), 2);
					right += pow(src.at<uchar>(j, i+1) - src.at<uchar>(j, i), 2);
				}
			}

			confidence_map.at<float>(y, x) = std::min({ up, down, left, right });
		}
	}
	// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
}


void transformations::Moravec::DrawFeature(cv::OutputArray& result_image)
{
	cv::Mat dst = result_image.getMat();

	for (int h = 0; h < confidence_map.rows; ++h)
	{
		for (int w = 0; w < confidence_map.cols; ++w)
		{
			if (confidence_map.at<float>(h, w) >= 15000)
			{
				cv::circle(dst, cv::Point(w, h), 3, cv::Scalar(255, 0, 0));
			}
		}
	}
}