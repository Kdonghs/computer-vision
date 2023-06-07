#include "filter.hpp"

Filter::~Filter()
{
	if (mask != nullptr)
	{
		for (int i = 0; i < maskSize; ++i)
		{
			delete[] mask[i];
		}
		delete[] mask;
	}
}

void Filter::AllocateMask()
{
	mask = new double* [maskSize];

	for (int i = 0; i < maskSize; ++i)
	{
		mask[i] = new double[maskSize] { 0.0, };
	}
}

void Filter::Convolute(cv::InputArray& input_image, cv::OutputArray& output_image)
{
	output_image.create(input_image.size(), CV_8UC1);

	cv::Mat input_mat = input_image.getMat();
	cv::Mat output_mat = output_image.getMat();

	int halfSize = maskSize / 2;
	for (int y = halfSize; y < input_mat.rows - halfSize; y++)
	{
		for (int x = halfSize; x < input_mat.cols - halfSize; x++)
		{
			double sum = 0;

			for (int dy = -halfSize; dy <= halfSize; ++dy)
				for (int dx = -halfSize; dx <= halfSize; ++dx)
					sum += input_mat.at<uchar>(y + dy, x + dx) * mask[dy + halfSize][dx + halfSize];

			output_mat.at<uchar>(y, x) = cv::saturate_cast<uchar>(sum);
		}
	}
}


void SobelFilterY::CreateMask()
{
	maskSize = 3;

	AllocateMask();

	/**
	 * Y방향 Sobel Filter를 구현합니다.
	 * mask 변수에 Sobel Filter Mask를 할당합니다.
	 */
	 // ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
	mask[0][0] = -1;
	mask[0][1] = -2;
	mask[0][2] = -1;
	mask[1][0] = 0;
	mask[1][1] = 0;
	mask[1][2] = 0;
	mask[2][0] = 1;
	mask[2][1] = 2;
	mask[2][2] = 1;
	// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
}

void SobelFilterX::CreateMask()
{
	maskSize = 3;

	AllocateMask();

	/**
	 * X방향 Sobel Filter를 구현합니다.
	 * mask 변수에 Sobel Filter Mask를 할당합니다.
	 */
	 // ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
	mask[0][0] = -1;
	mask[0][1] = 0;
	mask[0][2] = 1;
	mask[1][0] = -2;
	mask[1][1] = 0;
	mask[1][2] = 2;
	mask[2][0] = -1;
	mask[2][1] = 0;
	mask[2][2] = 1;
	// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
}
