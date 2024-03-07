

#include "cameraModule.h"
#include <opencv2/opencv.hpp>


bool CameraModule::detect_lowerbody(string test_image_path)
{
    cv::Mat image = cv::imread(test_image_path);

    cv::CascadeClassifier  lower_body_cascade;
    lower_body_cascade.load("/home/kovaiu1/opencv/data/haarcascades/haarcascade_lowerbody.xml");

    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

    vector<cv::Rect> lower_body_rectangles;
    lower_body_cascade.detectMultiScale(grayImage, lower_body_rectangles, 1.1, 4, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

    return (!lower_body_rectangles.empty());
}

