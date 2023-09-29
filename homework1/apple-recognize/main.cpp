#include <iostream>
#include <assert.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


int main()
{
	cv::Mat apple = cv::imread("../apple.png");
	assert(apple.channels() == 3);

	cv::Mat hsv;
	cv::Mat apple_channels[3];
	cv::split(apple,apple_channels);
	cv::cvtColor(apple, hsv, cv::COLOR_BGR2HSV); 

	apple_channels[2] -= apple_channels[1];
	cv::Mat normal_mat;
	cv::normalize(apple_channels[2], normal_mat, 0, 255., cv::NORM_MINMAX);
	cv::Mat red_result;
	cv::Mat red_result2;
	cv::threshold(normal_mat, red_result, 60, 255, cv::THRESH_BINARY); 
	cv::threshold(apple_channels[2], red_result2, 26, 255, cv::THRESH_BINARY); 


	cv::Mat yellow_part;
	cv::inRange(hsv, cv::Scalar(13, 160, 25), cv::Scalar(24, 255, 140),yellow_part);
	
	yellow_part = yellow_part & red_result2;
	red_result = yellow_part | red_result;

	cv::Mat kernel1 = getStructuringElement(cv::MORPH_CROSS, cv::Size(6, 6));
	cv::Mat kernel2 = getStructuringElement(cv::MORPH_CROSS, cv::Size(4, 4));
	cv::Mat final_result;
	cv::morphologyEx(red_result, final_result, cv::MORPH_CLOSE, kernel1); // 红色与黄色相连
	cv::morphologyEx(final_result,final_result, cv::MORPH_OPEN, kernel2,cv::Point(-1,-1),2); // 去除苹果上方枝干的一小部分
	
	std::vector< std::vector<cv::Point> > contours1;
	std::vector< cv::Vec4i > hierachy;
	cv::findContours( final_result, contours1, hierachy, cv::RETR_EXTERNAL,cv::CHAIN_APPROX_NONE);

    for (size_t i = 0; i < contours1.size(); i++) {
		if (cv::contourArea(contours1[i]) > 2000){
			cv::Rect rect = cv::boundingRect(contours1[i]);
        	cv::rectangle(apple, rect, cv::Scalar(0, 255, 0), 2);
			drawContours(apple, contours1, static_cast<int>(i), cv::Scalar(255, 0, 0), 2, cv::LINE_8, hierachy, 0);
		}
    }

	//cv::imshow("apple2",red_result);
	//cv::imshow("apple",final_result);
	cv::imshow("apple",apple);
	cv::imwrite("../apple_result.png",apple);
	cv::waitKey(0);
	return 0;
}