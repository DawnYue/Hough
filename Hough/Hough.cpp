#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
using namespace std;
using namespace cv;

//练习2
int main()
{
	cv::Mat mat_gray;
	cv::Mat mat_binary;
	cv::Mat mat_canny;
	cv::Mat srcImage = imread("E:\\metal-part-distorted-03.png");

	cvtColor(srcImage, mat_gray, COLOR_BGR2GRAY);//转化边缘检测后的图为灰度图
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
	cv::HoughLines(mat_canny, lines, 1, CV_PI / 180, 100);

	cv::Mat lineMat;
	cv::HoughLines(mat_canny, lineMat, 1, CV_PI / 180, 100);
	//float r = lines[0][0];
	//float t = lines[0][1];

	// draw line
	std::vector<cv::Vec2f>::iterator it = lines.begin();
	for (; it != lines.end(); ++it)
	{
		float rho = (*it)[0], theta = (*it)[1];
		cv::Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;		
		pt1.x = cv::saturate_cast<int>(x0 + 1000 * (-b));
		pt1.y = cv::saturate_cast<int>(y0 + 1000 * (a));
		pt2.x = cv::saturate_cast<int>(x0 - 1000 * (-b));
		pt2.y = cv::saturate_cast<int>(y0 - 1000 * (a));
		cv::line(srcImage, pt1, pt2, cv::Scalar(0, 0, 255),1, CV_AA);
	}       

	cv::imshow("canny", mat_canny);
	cv::imshow("mat", srcImage);
	cv::waitKey(0);

	return 0;
}

