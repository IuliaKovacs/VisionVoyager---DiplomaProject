#ifndef CAMERA_MODULE
#define CAMERA_MODULE



#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

#define RAW_IMAGES_FOLDER_PATH "../CameraModule/Raw_Images"
#define FACES_DATASET_PATH "../CameraModule/Faces_dataset"


class CameraModule
{   
    static int no_of_recognized_subjects;
    static bool resize_and_apply_grayscale(const fs::path& input_path, int subject_number);
    // @ToDo
    // static int count_recognized_subjects();

public:
    static void display_camera_capture();
    static bool parse_raw_images_folder();

};

#endif //CAMERA_MODULE
