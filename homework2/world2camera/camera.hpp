#include <eigen3/Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class Camera
{
private:
    Eigen::Matrix<double,3,4> inParam;
    Eigen::Quaterniond imuSense;
    Eigen::Matrix4d outParam;
    Eigen::Vector3d camLoca;
    Eigen::Vector2i SensorCenter;
    void calcOutParam();
public:
    Eigen::Vector2i world2camera(Eigen::Vector3d objWldSLoca);
    void showObjLoca(cv::Mat & picture,Eigen::Vector3d objWldLoca);
    Camera();
};