#include <iostream>
#include <eigen3/Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

const std::vector<cv::Point3d> PW_BIG{// 灯条坐标，单位：m
                                             {-0.115, 0.0265, 0.},
                                             {-0.115, -0.0265, 0.},
                                             {0.115, -0.0265, 0.},
                                             {0.115, 0.0265, 0.}};

int main()
{
    cv::Mat camMatrix,distCoeff;
    cv::FileStorage reader("../src/f_mat_and_c_mat.yml",cv::FileStorage::READ);
    reader["F"] >> camMatrix;
    reader["C"] >> distCoeff;

    std::vector<cv::Point2f> armor_corners = {{575.508,282.175},
                                        {573.93,331.819},
                                        {764.518,337.652},
                                        {765.729,286.741}
                                        };
    cv::Mat rvec,tvec,rMatrix;
    cv::solvePnP(PW_BIG,armor_corners,camMatrix,distCoeff,rvec,tvec);
    cv::Rodrigues(rvec,rMatrix);
    Eigen::MatrixXd R = Eigen::Map<Eigen::MatrixXd>(rMatrix.ptr<double>(), rMatrix.rows, rMatrix.cols);
    Eigen::MatrixXd T = Eigen::Map<Eigen::MatrixXd>(tvec.ptr<double>(), tvec.rows, tvec.cols);
    //std::cout << R << std::endl;
    //std::cout << T << std::endl;
    Eigen::Quaterniond q(-0.0816168, 0.994363, -0.0676645, -0.00122528);
    Eigen::Vector3d result = Eigen::Vector3d::Zero();
    for(const cv::Point3d & armor_point: PW_BIG)
    {
        Eigen::Vector3d imgpoint,objpoint;
        imgpoint(0) = armor_point.x;
        imgpoint(1) = armor_point.y;
        imgpoint(2) = armor_point.z;
        objpoint = q.matrix() * (R * imgpoint + T);
        //std::cout << objpoint.transpose() << std::endl;
        result += objpoint / 4;
    }
    std::cout << result << std::endl;
    return 0;
}