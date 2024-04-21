
#include "cameraModule.h"
#include <opencv2/opencv.hpp>

using namespace std;

int CameraModule::no_of_recognized_subjects = DEFAULT_NO_OF_RECOGNIZED_SUBJECTS;

void CameraModule::initialize_camera_module()
{   
    read_recognized_persons();
    no_of_recognized_subjects = count_recognized_subjects();
}


void CameraModule::display_camera_capture()
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) 
    {
        cout << "Error: Couldn't open the camera." << endl;
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



void CameraModule::capture_image(string path_to_store)
{
    static int no = 0;
    no++;

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) 
    {
        cout << "Error: Couldn't open the camera." << endl;
    }

    cv::Mat image;
    cap >> image;
    if (image.empty()) 
    {
        cerr << "Error: Failed to capture image" << endl;
    }

    if (!cv::imwrite(path_to_store + "/image" + to_string(no) + ".jpg", image)) 
    {
        cerr << "Error: Failed to store the image to " << path_to_store << endl;
    }

    cap.release();
}