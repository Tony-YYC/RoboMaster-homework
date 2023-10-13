#include "kalman.h"
#include <eigen3/Eigen/src/Core/Matrix.h>
#include <iostream>
#include <fstream>

int main()
{
    KalmanFilter kf;
    std::string filename = "../dollar.txt";
    std::fstream ftm(filename);
    double delta_t = 1;
    double x_temp;
    while(ftm)
    {
        ftm >> x_temp;
        if(!kf.IsInitialized())
        {
            Eigen::VectorXd x_in(2,1);
            x_in << x_temp,0.0;
            kf.Initialization(x_in);
            Eigen::MatrixXd P_in(2,2);
            P_in << 20.0,0.0,
                    0.0,100.0;
            kf.SetP(P_in);
            Eigen::MatrixXd F_in(2,2);
            F_in << 1.0,delta_t,
                    0.0,1.0;
            kf.SetF(F_in);
            Eigen::MatrixXd Q_in(2,2);
            Q_in << 2.0,0.0,
                    0.0,2.0;
            kf.SetQ(Q_in);
            Eigen::MatrixXd H_in(1,2);
            H_in << 1.0,0.0;
            kf.SetH(H_in);
            Eigen::MatrixXd R_in(1,1);
            R_in << 20.0;
            kf.SetR(R_in);
        }
        Eigen::VectorXd z(1,1);
        z << x_temp;
        kf.Update(z);
        Eigen::VectorXd x_out = kf.GetX();
        std::cout << "actual x:" << x_temp << "  ";
        std::cout << "kalman predict x:" << x_out(0) << " v:" << x_out(1) << std::endl;
    }
    ftm.close();
    return 0;
}