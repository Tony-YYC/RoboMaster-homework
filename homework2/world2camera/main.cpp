#include "camera.hpp"
#include <iostream>
#include <fstream>

int main()
{
    std::string filename = "../points.txt";
    std::fstream ftm(filename);
    if(ftm.is_open()){
        int pointNum;
        double x,y,z;
        cv::VideoWriter videoOutput("../output_camera.mp4", cv::VideoWriter::fourcc('H','2','6','4'), 25, cv::Size(2000, 1500));
        ftm >> pointNum;
        for(int i = 0;i<pointNum;i++)
        {
            Camera cam;
            ftm >> x >> y >> z;
            Eigen::Vector3d objWldLoca(x,y,z);
            cv::Mat picture = cv::Mat::zeros(1500,2000,CV_8UC3);
            cam.showObjLoca(picture,objWldLoca);
            cv::imshow("picture",picture);
            videoOutput.write(picture);
            if (cv::waitKey(15) >= 0) {
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