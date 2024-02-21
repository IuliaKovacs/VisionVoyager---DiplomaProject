
#include "cameraModule.h"
#include <opencv2/opencv.hpp>



void CameraModule::display_camera_capture()
{
    cv::VideoCapture cap(0);

    if (!cap.isOpened()) {
        std::cout << "Error: Couldn't open the camera." << std::endl;
    }

    cv::namedWindow("Camera", cv::WINDOW_NORMAL);

    cv::Mat frame;
    while (true) {
        cap >> frame; 

        if (frame.empty())
            break;

        cv::imshow("Camera", frame); 

        if (cv::waitKey(25) == 'q')
            break;
    }

    cap.release();
    cv::destroyAllWindows();
}