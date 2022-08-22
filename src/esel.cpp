

#include "vbase.hpp"
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;


int main() {


    VBase app;
    VideoCapture cam("/dev/video0");


    Mat frame;
    cam >> frame;
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGBA);
    cv::Mat flat = frame.reshape(1, frame.total()*frame.channels());
    std::vector<uchar> frame1 = frame.isContinuous()? flat : flat.clone();
    std::cout<<frame.cols<<std::endl;
    std::cout<<frame.rows<<std::endl;
    try {
        app.show(&frame1, &frame.cols, &frame.rows);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
