#include <eigen3/Eigen/Dense>

class KalmanFilter {
public:
    KalmanFilter() {
        is_initialized_ = false;
    }
    Eigen::VectorXd GetX() {
        return x_;
    }
    bool IsInitialized() {
        return is_initialized_;
    }
    void Initialization(const Eigen::VectorXd& x_in) {
        x_ = x_in;
        is_initialized_ = true;
    }
    void SetF(Eigen::MatrixXd F_in) {
        F_ = F_in;
    }
    void SetP(Eigen::MatrixXd P_in) {
        P_ = P_in;
    }
    void SetQ(Eigen::MatrixXd Q_in) {
        Q_ = Q_in;
    }
    void SetH(Eigen::MatrixXd H_in){
        H_ = H_in;
    }
    void SetR(Eigen::MatrixXd R_in){
        R_ = R_in;
    }
    void Update(const Eigen::VectorXd& z) {
        Prediction();
        MeasurementUpdate(z);
    }

private:
    bool is_initialized_;
    Eigen::VectorXd x_;
    Eigen::MatrixXd F_;
    Eigen::MatrixXd P_;
    Eigen::MatrixXd Q_;
    Eigen::MatrixXd H_;
    Eigen::MatrixXd R_;
    void Prediction();
    void MeasurementUpdate(const Eigen::VectorXd& z);
};