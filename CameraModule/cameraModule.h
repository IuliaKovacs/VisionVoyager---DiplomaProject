#ifndef CAMERA_MODULE
#define CAMERA_MODULE


#include <opencv2/core.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;
using namespace std;

#define RAW_IMAGES_FOLDER_PATH "../CameraModule/Raw_Images"
#define FACES_DATASET_PATH "../CameraModule/Faces_dataset"
#define DEFAULT_NO_OF_RECOGNIZED_SUBJECTS 0
#define CSV_FILE_PATH "../CameraModule/faces_csv.txt"


class CameraModule
{   
    static vector<cv::Mat> images;
    static vector<int> labels;  
    static int no_of_recognized_subjects;
    static bool resize_and_apply_grayscale_to_s(const fs::path& input_path, int subject_number);
    static int count_recognized_subjects();
    static bool parse_raw_images_folder();
    static void read_images_from_csv();
    static bool check_data_valid();

public:
    static void initialize_camera_module();
    static void display_camera_capture();
    static void add_new_recognized_subject();
    static void create_csv_database_file();
    static int recognize_face(string test_image_path);
    static bool resize_and_apply_grayscale(const fs::path& input_path);
    static bool detect_face_and_preprocess_if_so(string test_image_path);
};

#endif //CAMERA_MODULE
