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
        Camera cam;
        cv::Mat picture = cv::Mat::zeros(1000,2000,CV_8UC3);
        ftm >> pointNum;
        for(int i = 0;i<pointNum;i++)
        {
            ftm >> x >> y >> z;
            Eigen::Vector3d objWldLoca(x,y,z);
            cam.showObjLoca(picture,objWldLoca);
        }
        cv::imshow("picture",picture);
        cv::waitKey(0);
        cv::imwrite("../picture.png",picture);
    }
    else
    {
        std::cout << "can't open the file" << std::endl;
    }

   return 0;
}