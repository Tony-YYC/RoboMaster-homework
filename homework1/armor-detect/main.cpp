#include<iostream> 
#include "ArmorBox.h"


int main()
{
    cv::VideoCapture video;
    video.open("../armor.mp4");
    cv::Mat frame;
    int frame_width = static_cast<int>(video.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(video.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps = video.get(cv::CAP_PROP_FPS);
    cv::VideoWriter videoOutput("../output_armor.mp4", cv::VideoWriter::fourcc('H','2','6','4'), fps, cv::Size(frame_width, frame_height));
    while (true) {
        video >> frame;
        if (frame.empty()) {
            break;
        }
        ArmorBox armor(frame);
        cv::Mat resultframe = armor.getResultImg();
        cv::imshow("video", resultframe);
        videoOutput.write(resultframe);
        if (cv::waitKey(25) >= 0) {
            break;
        }
    }
    video.release();
    videoOutput.release();
    cv::destroyAllWindows();
    return 0;
}