#ifndef CAMERA_MODULE
#define CAMERA_MODULE



#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

#define RAW_IMAGES_FOLDER_PATH "../CameraModule/Raw_Images"
#define FACES_DATASET_PATH "../CameraModule/Faces_dataset"
#define DEFAULT_NO_OF_RECOGNIZED_SUBJECTS 0


class CameraModule
{   
    static int no_of_recognized_subjects;
    static bool resize_and_apply_grayscale(const fs::path& input_path, int subject_number);
    static int count_recognized_subjects();

public:
    static void initialize_camera_module();
    static void display_camera_capture();
    static bool parse_raw_images_folder();
    static void add_new_recognized_subject();

};

#endif //CAMERA_MODULE
