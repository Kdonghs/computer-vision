#include "ransac.hpp"

using namespace algorithms;

int algorithms::LeastSquare::generate_random_number(int begin, int end)
{
	return std::uniform_int_distribution<int>(begin, end)(rnd);
}

void algorithms::LeastSquare::fill_random_points(int inliers, int outliers)
{
	// inlier
	for (int i = 0; i < inliers; i++)
	{
		int x = generate_random_number(0, 279);
		int y = generate_random_number(120, 125);

		point_vector_list.emplace_back(x, y);
	}

	// outlier
	for (int i = 0; i < outliers; i++)
	{
		int x = generate_random_number(0, 279);
		int y = generate_random_number(0, 279);

		point_vector_list.emplace_back(x, y);
	}
}

void algorithms::LeastSquare::draw_point(cv::InputOutputArray& canvas)
{
	cv::Mat canvas_mat = canvas.getMat();
	for (auto& point : point_vector_list)
		canvas_mat.at<uchar>(point.y, point.x) = 255;
}

EquationElement algorithms::LeastSquare::calculate_least_square()
{
	double sumX = 0.0;
	double sumY = 0.0;

	double avgX = 0.0;
	double avgY = 0.0;

	// ����� ������ ���ϼ���.
	for (auto& point : point_vector_list)
	{
		// ** ���ݺ��� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **
		sumX += point.x;
		sumY += point.y;
		// ** ������� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **
	}

	avgX = sumX / point_vector_list.size();
	avgY = sumY / point_vector_list.size();

	double sumNumerator = 0.0;
	double sumDenominator = 0.0;

	for (auto& point : point_vector_list)
	{
		// Numerator�� Denominator�� ���� ���ڿ� �и� �ǹ���
		// ** ���ݺ��� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **
		sumNumerator += (point.y - avgY) * (point.x - avgX);
		sumDenominator += pow((point.x - avgX), 2);
		// ** ������� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **
	}

	EquationElement elem;
	// Numerator(����)�� Denominator(�и�)�� ���� ������ ����� ������ ���ϼ���.
	// ** ���ݺ��� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **
	elem.m = sumNumerator / sumDenominator;
	elem.b = avgY - (sumNumerator / sumDenominator) * avgX;
	// ** ������� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **

	return elem;
}

void algorithms::LeastSquare::draw_line(EquationElement element, cv::InputOutputArray& canvas)
{
	cv::Mat canvas_mat = canvas.getMat();
	for (int i = 0; i < canvas_mat.cols; i++) //������ �������� ���� ���
	{
		double sum3 = (element.m * i) + element.b;

		if (sum3 < 0)
			sum3 = 0;
		if (sum3 > canvas_mat.cols - 1)
			sum3 = canvas_mat.cols - 1;

		canvas_mat.at<uchar>(sum3, i) = 255;
	}
}

void CRANSAC::get_two_different_points()
{
	pt1Idx = generate_random_number(0, point_vector_list.size() - 1);
	pt1 = point_vector_list[pt1Idx];

	pt2Idx = generate_random_number(0, point_vector_list.size() - 1);
	pt2 = point_vector_list[pt2Idx];

	// ���� ���� ��� �ٽ� ����.
	while (pt1 == pt2)
	{
		pt2Idx = generate_random_number(0, point_vector_list.size() - 1);
		pt2 = point_vector_list[pt2Idx];
	}
}

void CRANSAC::convert_two_point_to_line()
{
	// Point pt1�� Point pt2�� �̿��ؼ� ����� ������ ����ϼ���
	// ** ���ݺ��� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **
	if (pt2.x ==  pt1.x)
	{
		lineEquation.m = (pt2.y - pt1.y) / (pt2.x - pt1.x + 0.0001);
		lineEquation.b = pt1.y - (lineEquation.m * pt1.x);
	}
	else {
		lineEquation.m = (pt2.y - pt1.y) / (pt2.x - pt1.x);
		lineEquation.b = pt1.y - (lineEquation.m * pt1.x);
	}
	// ** ������� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **
}

void CRANSAC::calculate_inlier()
{
	std::vector<cv::Point> liner;
	for (int i = 0; i < point_vector_list.size(); i++)
	{
		if (i != pt1Idx && i != pt2Idx)
		{
			// distance = |(-(x * m) - b + y) / root(m^2 + 1)|
			// point_vector_list[i]�� �̿��ؼ� �Ÿ��� ����ϼ���
			// ** ���ݺ��� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **
			double distance = abs(
				((point_vector_list[i].x * lineEquation.m) * -1 - lineEquation.b + point_vector_list[i].y) / 
				sqrt(lineEquation.m * lineEquation.m + 1)
			);
			// ** ������� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **
			if (distance <= 5)
				liner.push_back(point_vector_list[i]);
		}
	}
	if (maxInlier < liner.size())
	{
		maxInlier = liner.size();
		optimalEquation.m = lineEquation.m;
		optimalEquation.b = lineEquation.b;
	}
}

void algorithms::CRANSAC::draw_optimal_line(cv::InputOutputArray& canvas)
{
	draw_line(optimalEquation, canvas);
}
