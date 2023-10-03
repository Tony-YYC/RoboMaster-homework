#include <iostream>
#include <assert.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat extractBlue(const cv::Mat & img);
cv::RotatedRect getRect(const cv::Mat & p_img);
cv::Mat drawRotatedRect(const cv::Mat & background, const cv::RotatedRect & rotate_rect);
std::vector<cv::Point> getPoint(const cv::Mat & p_img);

int main()
{
    std::string filename[5] = {"../001.jpg","../002.jpg","../003.jpg","../004.jpg","../005.jpg"};
    for(int i = 0;i<5;i++)
    {
        cv::Mat img = cv::imread(filename[i]);
        cv::Mat bluepart = extractBlue(img);

        cv::RotatedRect plateRect = getRect(bluepart);
        cv::Mat result = drawRotatedRect(img,plateRect);

        std::vector<cv::Point> result2 = getPoint(bluepart);
        cv::polylines(result,result2,true,cv::Scalar(0, 255, 0),2);
        cv::imshow("resultapprox",result);
        cv::imwrite("../result"+std::to_string(i+1)+".png",result);
        cv::waitKey(0);
    }
}

cv::Mat extractBlue(const cv::Mat & img)
{
    cv::Mat hsv;
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV); 

    cv::Mat blue_part;
	cv::inRange(hsv, cv::Scalar(100, 200, 125), cv::Scalar(124, 255, 255),blue_part);
    
    cv::Mat kernel1 = getStructuringElement(cv::MORPH_CROSS, cv::Size(12, 12));
	cv::Mat kernel2 = getStructuringElement(cv::MORPH_CROSS, cv::Size(2, 2));
	cv::morphologyEx(blue_part,blue_part, cv::MORPH_OPEN, kernel2); // remove the noise in picture 5
    cv::morphologyEx(blue_part, blue_part, cv::MORPH_CLOSE, kernel1,cv::Point(-1,-1),2); // make the plate a solid rectangle
    //cv::imshow("bluepart",blue_part);
    return blue_part;
}

cv::RotatedRect getRect(const cv::Mat & p_img)
{
    std::vector<std::vector<cv::Point>> contours;
    cv::RotatedRect result;
    cv::findContours(p_img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    for(const auto & contour: contours)
    {
        if(cv::contourArea(contour) > 3500)
        {
            result = cv::minAreaRect(contour);
        }
    }
    return result;
}

std::vector<cv::Point> getPoint(const cv::Mat & p_img)
{
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Point> result;
    double cnt_len;
    cv::findContours(p_img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    for(const auto & contour: contours)
    {
        if(cv::contourArea(contour) > 3500)
        {
            cnt_len = cv::arcLength(contour, true);
            cv::approxPolyDP(contour,result,0.06*cnt_len,true);
        }
    }
    return result;
}

cv::Mat drawRotatedRect(const cv::Mat & background, const cv::RotatedRect & rotate_rect)
{
    //获取旋转矩形的四个顶点
	cv::Mat result = background;
    cv::Point2f* vertices = new cv::Point2f[4];
	rotate_rect.points(vertices);
 
	//逐条边绘制
	for (int j = 0; j < 4; j++)
	{
		cv::line(result, vertices[j], vertices[(j + 1) % 4], cv::Scalar(0, 0, 255),2);
	}
    return result;
}