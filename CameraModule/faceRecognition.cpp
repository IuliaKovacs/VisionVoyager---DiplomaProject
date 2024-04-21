

#include "cameraModule.h"
#include <opencv2/core.hpp>
#include <opencv2/face.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>



using namespace std;


vector<cv::Mat> CameraModule::images;
vector<int> CameraModule::labels;  
vector<Person> CameraModule::recognized_persons;

void CameraModule::read_images_from_csv()
{   
    string line;
    ifstream CSV_file(CSV_FILE_PATH);

    if (CSV_file.is_open())
    {
        while (getline(CSV_file,line))
        {
            while (!line.empty() && isspace(line.back())) 
            {
                line.pop_back();
            }

            string image_path = line.substr(0, line.find(';'));
            string label = line.substr(line.find(';') + 1);

            images.push_back(cv::imread(image_path, 0));
            labels.push_back(atoi(label.c_str()));
        }
    }
    else
    {
        cout << "Error: Unable to open CSV file" << endl;
    }
}


bool CameraModule::check_data_valid()
{
    if(1 >= images.size()) 
    {
        cout << "The face recognition needs at least 2 images in order to work" << endl;
        return false;
    }

    return true;
}


int CameraModule::recognize_face(string test_image_path)
{
    read_images_from_csv();
    check_data_valid();

    if (false == detect_face_and_preprocess_if_so(test_image_path))
    {   
        cout << "Predicted class = -1" << ", The image is not coresponding, does not contain a face!" << endl;
        return -1;
    }
    
    cv::Mat test_image = resize_and_apply_grayscale(test_image_path);
    cv::imshow("Input Image", test_image);

    if (test_image.empty()) 
    {
        cout << "Failed to load test image." << endl;
    }

    /* Create an Eigenfaces model and train it with the Faces_dataset retrieved data */
    cv::Ptr<cv::face::EigenFaceRecognizer> model = cv::face::EigenFaceRecognizer::create();
    model->train(images, labels);

    int predicted_label = -1;
    double confidence = 0.0;
    model->predict(test_image, predicted_label, confidence);

    cout << "Predicted class = " << predicted_label << ", Confidence = " << confidence << endl;

    return predicted_label;
}


