#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
class ArmorBox
{
private:
    cv::Mat frame;
    cv::Mat procImg;
    std::vector<cv::Rect> lightBars;
    std::vector<std::vector<cv::Rect>> Armors;

    const int procThreshold = 60;
    const double heightWidthRatioLowerLimit = 1.5;
    const double heightWidthRatioUpperLimit = 4;
    const int sizeLimit = 300;
    const int areaDiffential = 10000;
    const double XYRatioLimit = 1.93;
    const cv::Size kGaussianBlueSize = cv::Size(5, 5);
    
    bool contourJudge(const cv::Rect & boundRect);
    bool armorJudge(const cv::Rect & Rect1,const cv::Rect & Rect2);
    void preProc();
    void getlightContour();
    void findArmor();
    void drawArmor();
public:
    ArmorBox(cv::Mat frame);
    cv::Mat getResultImg();
};