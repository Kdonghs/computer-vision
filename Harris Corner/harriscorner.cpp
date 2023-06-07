#include <algorithm>
#include "harriscorner.hpp"

transformations::HarrisCorner::HarrisCorner(cv::InputArray& original_image)
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


void transformations::HarrisCorner::FindConfidenceMap(cv::InputArray& original_image)
{
	cv::Mat src = original_image.getMat();

	/**
	 * src를 이용하여 해리스코너의  알고리즘의 Confidence를 계산한 뒤
	 * 이를 confidence_map에 저장하세요.
	 * (confidence_map은 클래스 멤버 변수로 선언되어 있습니다.)
	 * 가우시안 필터는 클래스의 멤버 변수 G로 선언되어 있습니다. 크기는 3x3입니다.
	 * 가장자리는 0으로 하시면 됩니다. 
	 * k값은 0.04로 하시면 됩니다. 
	 */
	 // ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
	float k = 0.04;
	int h = src.rows;
	int w = src.cols;
	
	float** dx2 = new float* [h];
	float** dy2 = new float* [h];
	float** dxy = new float* [h];

	float** gdx2 = new float* [h];
	float** gdy2 = new float* [h];
	float** gdxy = new float* [h];

	for (int i = 0; i < h; i++) {
		dx2[i] = new float[w];
		dy2[i] = new float[w];
		dxy[i] = new float[w];

		gdx2[i] = new float[w];
		gdy2[i] = new float[w];
		gdxy[i] = new float[w];
	}
	
	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			dx2[j][i] = 0;
			dy2[j][i] = 0;
			dxy[j][i] = 0;

			gdx2[j][i] = 0;
			gdy2[j][i] = 0;
			gdxy[j][i] = 0;
		}
	}

	float tx, ty;
	for (int j = 1; j < h - 1; j++)
		for (int i = 1; i < w - 1; i++)
		{
			tx = (src.at<uchar>(j - 1, i + 1) + src.at<uchar>(j,i+1)+ src.at<uchar>(j+1,i+1)
				- src.at<uchar>(j-1,i-1)- src.at<uchar>(j,i-1)- src.at<uchar>(j+1,i-1)) / 6.f;
			
			ty = (src.at<uchar>(j + 1, i - 1) + src.at<uchar>(j + 1, i) + src.at<uchar>(j + 1, i + 1)
				- src.at<uchar>(j - 1, i - 1) - src.at<uchar>(j - 1, i) - src.at<uchar>(j - 1, i + 1)) / 6.f;
			

			dx2[j][i] = tx * tx;
			dy2[j][i] = ty * ty;
			dxy[j][i] = tx * ty;
		}

	float tx2, ty2, txy;
	for (int j = 2; j < h - 2; j++)
		for (int i = 2; i < w - 2; i++)
		{
			tx2 = ty2 = txy = 0;

			for (int y = 0; y < 3; y++)
				for (int x = 0; x < 3; x++)
				{
					tx2 += (dx2[j + y - 2][i + x - 2] * G[y][x]);
					ty2 += (dy2[j + y - 2][i + x - 2] * G[y][x]);
					txy += (dxy[j + y - 2][i + x - 2] * G[y][x]);
				}

			gdx2[j][i] = tx2;
			gdy2[j][i] = ty2;
			gdxy[j][i] = txy;
		}
		
	for (int j = 2; j < h - 2; j++)
		for (int i = 2; i < w - 2; i++)
		{
			confidence_map.at<float>(j,i) = (gdx2[j][i] * gdy2[j][i] - gdxy[j][i] * gdxy[j][i])
				- k * (gdx2[j][i] + gdy2[j][i]) * (gdx2[j][i] + gdy2[j][i]);
		}


	// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
}




void transformations::HarrisCorner::DrawFeature(cv::OutputArray& result_image)
{
	cv::Mat dst = result_image.getMat();
	cv::Mat confidence_map_norm ;
	cv::normalize(confidence_map, confidence_map_norm, 0, 1, cv::NORM_MINMAX, CV_32FC1, cv::Mat());

	for (int h = 0; h < confidence_map_norm.rows; ++h)
	{
		for (int w = 0; w < confidence_map_norm.cols; ++w)
		{
			if (confidence_map_norm.at<float>(h, w) >= 0.3)
			{
				cv::circle(dst, cv::Point(w, h), 3, cv::Scalar(255, 0, 0));
			}
		}
	}
}
