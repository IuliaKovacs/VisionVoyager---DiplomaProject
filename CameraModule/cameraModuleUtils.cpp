
#include "cameraModule.h"
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;
using namespace std;


bool CameraModule::resize_and_apply_grayscale_to_s(const fs::path& input_path, int subject_number)
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
    cv::resize(image, resized_image, cv::Size(368, 448)); //368x448

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
            if (false == detect_face_and_preprocess_if_so(entry.path()))
            {
                cout << "The following image is not particular usefull:" << endl;
                cout<< entry.path() << endl;
                cout << "------------------------------------------------" << endl;
            }

            resize_and_apply_grayscale_to_s(entry.path(), no_of_recognized_subjects);
        }
    }
    return true;
}



int CameraModule::count_recognized_subjects()
{
    string folder_path = FACES_DATASET_PATH;
    int count = 0;

    for (const auto& entry : fs::directory_iterator(folder_path)) 
    {
        count++;
    }

    return count;
}


void CameraModule::add_new_recognized_subject()
{   
    no_of_recognized_subjects++;

    string folder_path = string(FACES_DATASET_PATH) + "/s" + to_string(no_of_recognized_subjects);

    try 
    {
        fs::create_directory(folder_path);
    } 
    catch (const std::exception& e) 
    {
        cout << "Error in creating subject folder: " << e.what() << endl;
    }

    if (false == parse_raw_images_folder())
    {
        cout << "Error in processing images" << endl;
    }

    // @ToDo
    // update CSV file
}


void CameraModule::create_csv_database_file()
{   
    int label_no = 0;
    string current_directory = fs::current_path();
    string parent_directory = fs::path(current_directory).parent_path();
    string full_specified_path_for_face_db = parent_directory + "/CameraModule/Faces_dataset";

    ofstream CSV_file;
    CSV_file.open(CSV_FILE_PATH, std::ios::out);

    if (CSV_file.is_open())
    {
        if (!fs::exists(full_specified_path_for_face_db) || !fs::is_directory(full_specified_path_for_face_db)) 
        {
            cout << "Error with the Face_dataset folder" << endl;
        }

        for (const auto& s_entry : fs::directory_iterator(full_specified_path_for_face_db)) 
        {
            if (fs::is_directory(s_entry)) 
            {
                string entry_name = s_entry.path().filename().string();
                for (const auto& image_entry : fs::directory_iterator(s_entry)) 
                {
                    if (fs::is_regular_file(image_entry)) 
                    {
                        string image_name = image_entry.path().filename().string();
                        CSV_file << full_specified_path_for_face_db + "/" + entry_name + "/" + image_name + ";" + to_string(label_no) << endl;
                    }
                    else
                    {
                        cout << "Error: Faces_dataset folder structure is not respected" << endl;
                    }
                }
            }
            else
            {
                cout << "Error: Faces_dataset folder structure is not respected" << endl;
            }
            label_no++;
        }
    }
    else
    {
        cout << "Error: Unable to make the CSV file" << endl;
    }
}


cv::Mat CameraModule::resize_and_apply_grayscale(const fs::path& input_path)
{   
    cv::Mat image = cv::imread(input_path.string());

    if (image.empty()) 
    {
        cout << "Failed to load image: " << input_path << endl;
    }

    cv::Mat resized_image;
    cv::resize(image, resized_image, cv::Size(368, 448));

    cv::Mat grayscale_image;
    cv::cvtColor(resized_image, grayscale_image, cv::COLOR_BGR2GRAY);

    return grayscale_image;
}



bool CameraModule::detect_face_and_preprocess_if_so(string test_image_path)
{
    cv::CascadeClassifier face_cascade;
    face_cascade.load("/home/kovaiu1/opencv/data/haarcascades/haarcascade_frontalface_default.xml");

    cv::Mat image = cv::imread(test_image_path);
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(gray, faces, 1.1, 5, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

    if (1 == faces.size())
    {
        cv::Rect rect_face = faces[0];
        cv::Mat face_image = image(rect_face);
        cv::imwrite(test_image_path, face_image);
        return true;
    }
    else if (1 < faces.size())
    {
        cout << "Too many faces in the image!" << endl;
        return true;
    }
    else if (1 > faces.size())
    {
        cout << "The image doesn't contain a face! " << endl;
        return false;
    }
}
