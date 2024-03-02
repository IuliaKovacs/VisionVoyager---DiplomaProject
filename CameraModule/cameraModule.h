#ifndef CAMERA_MODULE
#define CAMERA_MODULE



#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;


class CameraModule
{

public:
    static void display_camera_capture();
    static bool resize_and_apply_grayscale(const fs::path& input_path, int subject_number);

};

#endif //CAMERA_MODULE
