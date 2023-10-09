#include <iostream>
#include "calibrate.hpp"
int main()
{
    CalibrateCam cam1(cv::Size(9,6),cv::Size(10,10),"../chess/",40);
    cam1.printresult();
    return 0;
}