#include "ArmorBox.h"
#include <iostream>
#include <cmath>
ArmorBox::ArmorBox(cv::Mat frame_): frame(frame_) {}

void ArmorBox::preProc()
{
    cv::Mat binary,Gaussian,hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV); 
    cv::inRange(hsv,cv::Scalar(10,50,100),cv::Scalar(26,255,255),binary);
    cv::Mat kernel = getStructuringElement(cv::MORPH_CROSS, cv::Size(4, 4));
    cv::morphologyEx(binary, binary, cv::MORPH_CLOSE, kernel);
    cv::GaussianBlur(binary, Gaussian, kGaussianBlueSize, 0);
    procImg = Gaussian;
    cv::imshow("procImg",Gaussian);
}

void ArmorBox::getlightContour()
{
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(procImg, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    for (size_t i = 0; i < contours.size(); i++) {
        cv::Rect boundRect;
        boundRect = cv::boundingRect(cv::Mat(contours[i]));
        if(contourJudge(boundRect))
        {
            //cv::rectangle(frame, boundRect.tl(), boundRect.br(), cv::Scalar(0, 255, 0), 2,8 ,0);
            //cv::drawContours(frame, contours, static_cast<int>(i), cv::Scalar(0, 255, 0), 2, cv::LINE_8, hierarchy, 0);
            lightBars.push_back(boundRect);
        }
    }
    //cv::imshow("contours",frame);
}

bool ArmorBox::contourJudge(const cv::Rect & boundRect)
{
    bool flag = true;
    if(boundRect.height / boundRect.width < heightWidthRatioLowerLimit || boundRect.height / boundRect.width > heightWidthRatioUpperLimit) flag = false;
    if(boundRect.width * boundRect.height < sizeLimit) flag = false;
    return flag;
}

void ArmorBox::findArmor()
{
    if(lightBars.size() >= 2)
    {
        for (size_t i = 0; i < lightBars.size() - 1; i++)
        {
            for (size_t j = i + 1; j < lightBars.size(); j++) {
                if(armorJudge(lightBars[i],lightBars[j]))
                {
                    std::vector<cv::Rect> temp {lightBars[i],lightBars[j]};
                    Armors.push_back(temp);
                }
            } 
        }
    }
    else
    {
        //std::cout << "not enough rectangles" << std::endl;
    }
}

bool ArmorBox::armorJudge(const cv::Rect & Rect1,const cv::Rect & Rect2)
{
    bool flag = true;
    if(abs(Rect1.area() - Rect1.area()) > areaDiffential) flag = false;
    if(sqrt(pow(Rect1.x - Rect2.x,2) + pow(Rect1.y-Rect2.y,2)) / double((Rect1.height + Rect2.height)/2) > XYRatioLimit) flag = false;
    if(flag == true) std::cout << sqrt(pow(Rect1.x - Rect2.x,2) + pow(Rect1.y-Rect2.y,2)) / double((Rect1.height + Rect2.height)/2) << std::endl;
    return flag;
}

void ArmorBox::drawArmor()
{
    for(const std::vector<cv::Rect> armor : Armors)
    {
        cv::Point point1 = cv::Point(armor[0].x + armor[0].width / 2, armor[0].y);
        cv::Point point2 = cv::Point(armor[0].x + armor[0].width / 2, armor[0].y + armor[0].height);
        cv::Point point3 = cv::Point(armor[1].x + armor[1].width / 2, armor[1].y);
        cv::Point point4 = cv::Point(armor[1].x + armor[1].width / 2, armor[1].y + armor[1].height);
        cv::line(frame, point1, point2, cv::Scalar(0, 255, 0), 2);
        cv::line(frame, point3, point4, cv::Scalar(0, 255, 0), 2);
        cv::line(frame, point1, point3, cv::Scalar(0, 255, 0), 2);
        cv::line(frame, point2, point4, cv::Scalar(0, 255, 0), 2);
        int cX = (point1.x + point2.x + point3.x + point4.x) / 4;
        int cY = (point1.y + point2.y + point3.y + point4.y) / 4;
        cv::circle(frame,cv::Point(cX,cY),5,cv::Scalar(0,0,255),2);
    }
    //cv::imshow("result",frame);
}

cv::Mat ArmorBox::getResultImg()
{
    preProc();
    getlightContour();
    findArmor();
    drawArmor();
    return frame;
}