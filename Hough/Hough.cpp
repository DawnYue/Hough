#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
using namespace std;
using namespace cv;

//练习3
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
	std::vector<cv::Vec4i> lines;
	cv::HoughLinesP(mat_canny, lines, 1, CV_PI / 180, 35, 10,10);

	// draw line
	for (size_t i = 0; i < lines.size(); i++)
	{
		cv::Vec4i linex = lines[i];
		line(srcImage, cv::Point(linex[0], linex[1]), cv::Point(linex[2], linex[3]), cv::Scalar(0, 0, 255), 1, CV_AA);
	}  


	cv::imshow("canny", mat_canny);
	cv::imshow("mat", srcImage);
	cv::waitKey(0);

	return 0;
}

