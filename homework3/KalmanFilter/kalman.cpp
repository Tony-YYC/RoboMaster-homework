#include "kalman.h"
#include <iostream>
#include <eigen3/Eigen/src/Core/Matrix.h>

void KalmanFilter::Prediction(){
    x_ = F_ * x_;
    P_ = F_ * P_ * F_.transpose() + Q_;
}

void KalmanFilter::MeasurementUpdate(const Eigen::VectorXd& z){
    Eigen::VectorXd y = z - H_ * x_;
    Eigen::MatrixXd S = H_ * P_ * H_.transpose() + R_;
    Eigen::MatrixXd K = P_ * H_.transpose() * S.inverse();
    x_ = x_ + K * y;
    Eigen::MatrixXd I = Eigen::MatrixXd::Identity(x_.size(),x_.size());
    P_ = (I - K * H_) * P_;
}