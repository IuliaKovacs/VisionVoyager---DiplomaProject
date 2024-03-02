
#include "cameraModule.h"
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;
using namespace std;



bool CameraModule::resize_and_apply_grayscale(const fs::path& input_path, int subject_number)
{
    static int file_no = 0;
    file_no++;

    cv::Mat image = cv::imread(input_path.string());
    // cv::imshow("Image", image);
    // cv::waitKey(3000);  // Wait for 3000 milliseconds (3 seconds)
    // cv::destroyAllWindows();  // Close all OpenCV windows

    if (image.empty()) {
        cout << "Failed to load image: " << input_path << endl;
        return false;
    }

    cv::Mat resized_image;
    cv::resize(image, resized_image, cv::Size(92, 112));

    cv::Mat grayscale_image;
    cv::cvtColor(resized_image, grayscale_image, cv::COLOR_BGR2GRAY);

    string output_path = "../CameraModule/Faces_dataset/s" + to_string(subject_number) + "/" + to_string(file_no) + ".jpg";
    cv::imwrite(output_path, grayscale_image);
    return true;
}