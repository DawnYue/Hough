#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
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
	cv::waitKey(0);

	return 0;
}

