#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
using namespace std;
using namespace cv;

//课前准备
int main()
{
	std::string img_path;
	cv::Mat mat_color;
	cv::Mat mat_gray;
	cv::Mat mat_binary;
	cv::Mat mat_canny;

	img_path = "../pictures/000177.png";
	mat_color = cv::imread("E:\\4.png", 1);
	mat_gray = cv::imread("E:\\4.png", 0);

	//形态学闭运算
	cv::Mat elementRect;
	elementRect = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));
	cv::morphologyEx(mat_gray, mat_gray, cv::MORPH_CLOSE, elementRect);

	// binary
	cv::threshold(mat_gray, mat_binary, 125, 255.0, cv::THRESH_BINARY);

	// detect edge
	cv::Canny(mat_binary, mat_canny, 50, 125, 3);

	// detect line
	std::vector<cv::Vec2f> lines;
	//std::vector<cv::Vec4i> lines;
	
	cv::HoughLines(mat_canny, lines, 1, CV_PI / 180, 150, 0, 0);
	//cv::HoughLinesP(mat_canny, lines, 1, CV_PI / 180, 100, 10, 50);

	// draw line
	std::cout << "line number: " << lines.size() << std::endl;
	for (size_t i = 0; i < lines.size(); i++)
	{
		cv::Vec2f linex = lines[i];
		std::cout << "radius: " << linex[0] << ", angle: " << 180 / CV_PI * linex[1] << std::endl;
		float rho = lines[i][0], theta = lines[i][1];
		cv::Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(mat_color, pt1, pt2, cv::Scalar(255, 0, 0), 1);
	}       

/*	for (size_t i = 0; i < lines.size(); i++)
	{
		cv::Vec4i& linex = lines[i];
		int dx = linex[2] - linex[0];
		int dy = linex[2] - linex[1];
		double angle = atan2(double(dy), dx) * 180 / CV_PI;
		//if (abs(angle) <= 20)
		//    continue;

		line(mat_color, cv::Point(linex[0], linex[1]), cv::Point(linex[2], linex[3]), cv::Scalar(255, 0, 0), 1);
	}  */


	cv::imshow("gray", mat_gray);
	cv::imshow("binary", mat_binary);
	cv::imshow("canny", mat_canny);
	cv::imshow("color", mat_color);





	std::vector<Point>points;
	//(27 39) (8 5) (8 9) (16 22) (44 71) (35 44) (43 57) (19 24) (27 39) (37 52)

	points.push_back(Point(27, 39));
	points.push_back(Point(8, 5));
	points.push_back(Point(8, 9));
	points.push_back(Point(16, 22));
	points.push_back(Point(44, 71));
	points.push_back(Point(35, 44));
	points.push_back(Point(43, 57));
	points.push_back(Point(19, 24));
	points.push_back(Point(27, 39));
	points.push_back(Point(37, 52));
	cv::Mat src = cv::Mat::zeros(400, 400, CV_8UC3);

	for (int i = 0; i < points.size(); i++)
	{
		//在原图上画出点
		circle(src, points[i], 3, Scalar(0, 0, 255), 1, 8);
	}
	//构建A矩阵 
	int N = 2;
	cv::Mat A = cv::Mat::zeros(N, N, CV_64FC1);

	for (int row = 0; row < A.rows; row++)
	{
		for (int col = 0; col < A.cols; col++)
		{
			for (int k = 0; k < points.size(); k++)
			{
				A.at<double>(row, col) = A.at<double>(row, col) + pow(points[k].x, row + col);
			}
		}
	}
	//构建B矩阵
	cv::Mat B = cv::Mat::zeros(N, 1, CV_64FC1);
	for (int row = 0; row < B.rows; row++)
	{

		for (int k = 0; k < points.size(); k++)
		{
			B.at<double>(row, 0) = B.at<double>(row, 0) + pow(points[k].x, row)*points[k].y;
		}
	}
	//A*X=B
	cv::Mat X;
	//cout << A << endl << B << endl;
	solve(A, B, X, DECOMP_LU);
	std::cout << X << endl;
	std::vector<Point>lines2;
	for (int x = 0; x < src.size().width; x++)
	{				// y = b + ax;
		double y = X.at<double>(0, 0) + X.at<double>(1, 0)*x;
		printf("(%d,%lf)\n", x, y);
		lines2.push_back(Point(x, y));
	}
	polylines(src, lines2, false, Scalar(255, 0, 0), 1, 8);
	cv::imshow("src", src);

	//cv::imshow("src", A);








	cv::waitKey(0);

	return 0;
}

