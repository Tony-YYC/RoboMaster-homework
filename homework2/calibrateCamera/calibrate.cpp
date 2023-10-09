#include "calibrate.hpp"

CalibrateCam::CalibrateCam(const cv::Size & boardSize,const cv::Size & chessSize,const std::string & srcFolder,int photoNum)
{
    camera_matrix = cv::Mat(3, 3, CV_32FC1, cv::Scalar::all(0));
    distort_matrix = cv::Mat(1, 5, CV_32FC1, cv::Scalar::all(0));

    board_w = boardSize.width;
    board_h = boardSize.height;
    board_num = boardSize.width * boardSize.height;
    board_size = boardSize;

    photo_num = photoNum;
    success_photo_num = 0;

    cv::Mat src0 = cv::imread(srcFolder+"1.jpg");
    img_size.width = src0.cols;
    img_size.height = src0.rows;

    chess_size = chessSize;
    src_folder = srcFolder;
}

void CalibrateCam::getStdChessboard()
{
    std::vector<cv::Point3f> temp_std_chessboard_pix;
    for (int i = 0; i < success_photo_num; i++)
    {
        for (int j = 0; j < board_h; j++)
        {
            for (int k = 0; k < board_w; k++)
            {
                cv::Point3f pt;
                pt.x = k * chess_size.width;
                pt.y = j * chess_size.height;
                pt.z = 0;
                temp_std_chessboard_pix.push_back(pt);
            }
        }
        std_chessboard_pix_array.push_back(temp_std_chessboard_pix);
        temp_std_chessboard_pix.clear();
    }
}

void CalibrateCam::getChessboard()
{
    cv::Mat gray_img, drawn_img;
    std::vector<cv::Point2f> temp_chessboard_pix;
    bool found = false;
    for (int i = 0; i < photo_num; i++)
    {
        cv::Mat src = cv::imread(src_folder + std::__cxx11::to_string(i).append(".jpg"));
        found = findChessboardCorners(src, board_size, temp_chessboard_pix);
        if (found && temp_chessboard_pix.size() == board_num)
        {
            success_photo_num++;
            cv::cvtColor(src, gray_img, cv::COLOR_BGR2GRAY);
            cv::find4QuadCornerSubpix(gray_img, temp_chessboard_pix, cv::Size(5, 5));
            chessboard_pix_array.push_back(temp_chessboard_pix);
            drawn_img = src.clone();
            cv::drawChessboardCorners(drawn_img, board_size, temp_chessboard_pix, found);
            cv::imshow("corners", drawn_img);
            cv::waitKey(100);
        }
        else
        {
            std::cout << "\t fail to load photo \"" << i << ".jpg\"" << std::endl;
        }
        temp_chessboard_pix.clear();
    };
    std::cout << success_photo_num << " useful chess boards" << std::endl;
}

void CalibrateCam::calcParam()
{
    getChessboard();
    getStdChessboard();
    calibrateCamera(std_chessboard_pix_array, chessboard_pix_array, img_size, camera_matrix, distort_matrix, rvecs, tvecs);
}

void CalibrateCam::printresult()
{
    calcParam();
    std::cout << camera_matrix << std::endl << distort_matrix << std::endl;
}