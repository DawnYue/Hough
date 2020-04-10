#include <opencv2/opencv.hpp>  
#include <iostream>
#include<cassert>
#include<opencv2/core/matx.hpp>
#include<vector>

//练习1
using namespace cv;
using namespace std;

int main()
{
	Mat srcImage, dstImage;
	srcImage = imread("E:\\lena.jpg");
	if (!srcImage.data) { return -1; }
	float angle = -10.0, scale = 1;
	cv::Point2f center(srcImage.cols / 2.0, srcImage.rows / 2.0);

	// 变换矩阵
	 cv::Mat rot = cv::getRotationMatrix2D(center, angle, scale);
	 cv::Rect bbox = cv::RotatedRect(center, srcImage.size(),angle).boundingRect();
	 rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
	 rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;

	// 仿射变换函数
	cv::warpAffine(srcImage, dstImage, rot, bbox.size());//,interMethod    INTER_LINEAR

	imshow("srcImage", srcImage);
	imshow("dstImage", dstImage);
	waitKey(0);

}