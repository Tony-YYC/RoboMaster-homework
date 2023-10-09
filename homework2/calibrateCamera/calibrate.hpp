#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

class CalibrateCam
{
private:
//input parameters
    int photo_num;
    int board_w,board_h,board_num;
    int success_photo_num;
    std::string src_folder;
    cv::Size chess_size;
    cv::Size board_size;
    cv::Size img_size;

//calculate process
    std::vector<std::vector<cv::Point2f>> chessboard_pix_array;
    std::vector<std::vector<cv::Point3f>> std_chessboard_pix_array;

//final result
    cv::Mat camera_matrix;
    cv::Mat distort_matrix;
    std::vector<cv::Mat> rvecs;
    std::vector<cv::Mat> tvecs;

//method
    void getStdChessboard();
    void getChessboard();
    void calcParam();
public:
    CalibrateCam(const cv::Size & boardSize,const cv::Size & chessSize,const std::string & srcFolder,int photoNum);
    void printresult();
};
