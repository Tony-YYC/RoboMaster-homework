#include "camera.hpp"
#include <iostream>

Camera::Camera()
{
    inParam << 400., 0., 190., 0.,
            0., 400., 160., 0.,
            0., 0., 1., 0.;
    camLoca << 2,2,2;
    imuSense = Eigen::Quaterniond{-0.5,0.5,0.5,-0.5};
    outParam = Eigen::Matrix4d::Zero();
    SensorCenter << 300,100;
    calcOutParam();
}

void Camera::relocate(Eigen::Vector3d newcamLoca,Eigen::Quaterniond newimuSense)
{
    camLoca = newcamLoca;
    imuSense = newimuSense;
    calcOutParam();
}

void Camera::calcOutParam()
{
    Eigen::Matrix3d Rotate = imuSense.matrix();
    outParam.block(0, 0, 3, 3) = Rotate.transpose();
    outParam.block(0, 3, 3, 1) = -Rotate.transpose() * camLoca;
    outParam(3,3) = 1;
    //std::cout << outParam << std::endl;
}

Eigen::Vector2i Camera::world2camera(Eigen::Vector3d objWldLoca)
{
    Eigen::Vector4d objWldCalc;
    objWldCalc << objWldLoca , 1;
    Eigen::Vector3d objCamCalc = inParam * outParam * objWldCalc;
    //std::cout << objCamCalc.transpose() << std::endl;
    Eigen::Vector2i objCamLoca = (objCamCalc.head(2) / objCamCalc[2]).cast<int>();
    return objCamLoca;
}

void Camera::showObjLoca(cv::Mat & picture,Eigen::Vector3d objWldLoca)
{
    Eigen::Vector2i objCamLoca = world2camera(objWldLoca);
    Eigen::Vector2i objPicLoca = objCamLoca + SensorCenter;
    //std::cout << objCamLoca.transpose() << std::endl;
    //std::cout << objPicLoca.transpose() << std::endl;
    cv::circle(picture,cv::Point(objPicLoca[0],objPicLoca[1]),2,cv::Scalar(255,255,255),3);
}

void Camera::showObjsLoca(cv::Mat & picture,std::vector<Eigen::Vector3d> objWldLocas,std::vector<int> radius)
{
    int num = 0;
    for(const auto & objWldLoca: objWldLocas)
    {
        Eigen::Vector2i objCamLoca = world2camera(objWldLoca);
        Eigen::Vector2i objPicLoca = objCamLoca + SensorCenter;
        cv::circle(picture,cv::Point(objPicLoca[0],objPicLoca[1]),radius[num],cv::Scalar(255,255,255),radius[num]*2 - 1);
        num++;
    }
}