
#include "cameraModule.h"
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;
using namespace std;

int CameraModule::no_of_recognized_subjects = 1;

bool CameraModule::resize_and_apply_grayscale(const fs::path& input_path, int subject_number)
{
    static int file_no = 0;
    file_no++;

    cv::Mat image = cv::imread(input_path.string());

    if (image.empty()) 
    {
        cout << "Failed to load image: " << input_path << endl;
        return false;
    }

    cv::Mat resized_image;
    cv::resize(image, resized_image, cv::Size(92, 112));

    cv::Mat grayscale_image;
    cv::cvtColor(resized_image, grayscale_image, cv::COLOR_BGR2GRAY);

    string output_path = string(FACES_DATASET_PATH) + "/s" + to_string(subject_number) + "/" + to_string(file_no) + ".jpg";
    cv::imwrite(output_path, grayscale_image);
    return true;
}


bool CameraModule::parse_raw_images_folder()
{
    fs::path folder_path(RAW_IMAGES_FOLDER_PATH);

    if (!fs::exists(folder_path) || !fs::is_directory(folder_path)) 
    {
        cout << "Folder does not exist or is not a directory." << endl;
        return false;
    }

    for (const auto& entry : fs::directory_iterator(folder_path)) 
    {
        if (fs::is_regular_file(entry)) 
        {
            if (false == resize_and_apply_grayscale(entry.path(), no_of_recognized_subjects))
            {
                cout << "There was a problem with processing the images" << endl;
                return false;
            }
        }
    }
    return true;
}