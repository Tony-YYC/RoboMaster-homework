#include "camera.hpp"
#include <iostream>
#include <fstream>
#include <random>

int main()
{
    std::string filename = "../points.txt";
    std::fstream ftm(filename);
    if(ftm.is_open()){
        int pointNum;
        double x,y,z;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(1, 3);
        cv::VideoWriter videoOutput("../output_camera.mp4", cv::VideoWriter::fourcc('A','V','C','1'), 25, cv::Size(1920, 1080));
        std::vector<Eigen::Vector3d> points;
        std::vector<int> point_radius;
        Camera cam;
        ftm >> pointNum;
        for(int i = 0;i<pointNum;i++)
        {
            ftm >> x >> y >> z;
            Eigen::Vector3d objWldLoca(x,y,z);
            points.push_back(objWldLoca);
            point_radius.push_back(distribution(gen));
        }
        for(int i = 0;i<=100;i++)
        {
            cv::Mat picture = cv::Mat::zeros(1080,1920,CV_8UC3);
            Eigen::AngleAxisd rollAngle(-90 * M_PI / 180.0, Eigen::Vector3d::UnitX());
            Eigen::AngleAxisd pitchAngle(-0.9 * i * M_PI/ 180.0, Eigen::Vector3d::UnitY());
            Eigen::AngleAxisd yawAngle((90 - 0.9 * i) * M_PI / 180.0, Eigen::Vector3d::UnitZ());
            Eigen::Quaterniond q = rollAngle * pitchAngle * yawAngle;
            //std::cout << q.w() << q.x() << q.y() << q.z() << std::endl;
            //std::cout << q.matrix().eulerAngles(2,1,0);
            cam.relocate(Eigen::Vector3d(0+ i * 0.02,4 - i * 0.02,4-i*0.02),q);
            cam.showObjsLoca(picture,points,point_radius);
            cv::imshow("picture",picture);
            videoOutput.write(picture);
            if (cv::waitKey(25) >= 0) {
                break;
            }
        }
        videoOutput.release();
    }
    else
    {
        std::cout << "can't open the file" << std::endl;
    }

   return 0;
}