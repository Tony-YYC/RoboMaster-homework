#include "camera.hpp"
#include <iostream>

Camera::Camera()
{
    inParam << 400., 0., 190., 0.,
            0., 400., 160., 0.,
            0., 0., 1., 0.;
    camLoca << 2,2,2;
    imuSense = Eigen::Quaterniond(0.5,0.5,-0.5,-0.5);
    SensorCenter << 1000,750;
}

void Camera::calcOutParam()
{
    outParam << imuSense.toRotationMatrix() , camLoca,
            Eigen::Matrix<double,1,3>::Zero() , 1;
}

Eigen::Vector2i Camera::world2camera(Eigen::Vector3d objWldLoca)
{
    calcOutParam();
    Eigen::Vector4d objWldCalc = objWldLoca.homogeneous();
    Eigen::Vector3d objCamCalc = inParam * outParam * objWldCalc;
    Eigen::Vector2i objCamLoca = (objCamCalc.head(2) / objCamCalc[2]).cast<int>();
    return objCamLoca;
}

void Camera::showObjLoca(cv::Mat & picture,Eigen::Vector3d objWldLoca)
{
    Eigen::Vector2i objCamLoca = world2camera(objWldLoca);
    Eigen::Vector2i objPicLoca = objCamLoca + SensorCenter;
    std::cout << objCamLoca.transpose() << std::endl;
    std::cout << objPicLoca.transpose() << std::endl;
    cv::circle(picture,cv::Point(SensorCenter[0],SensorCenter[1]),5,cv::Scalar(255,0,0),2);
    cv::putText(picture, "center" ,cv::Point(SensorCenter[0],SensorCenter[1]), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255,0,0));
    cv::circle(picture,cv::Point(objPicLoca[0],objPicLoca[1]),5,cv::Scalar(0,0,255),2);
    cv::putText(picture, "(" +std::to_string(objCamLoca[0]) + "," + std::to_string(objCamLoca[1]) + ")",cv::Point(objPicLoca[0],objPicLoca[1]), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 255,0));
}