#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <string>

using namespace cv;
using namespace std;

#define VOCABULARY "vocabulary"
#define TRAIN_DATA_PATH "C:\\Users\\DAT\\Desktop\\train_data\\"
#define DICTIONARY_PATH "C:\\Users\\DAT\\Desktop\\train_data\\dictionary.yml"
#define DESCRIPTOR_PATH "C:\\Users\\DAT\\Desktop\\train_data\\descriptor.yml"
class Utils
{
public:
    Utils();
    vector<KeyPoint> static buildDictionary(const int numberOfClusters);
    void static trainClassesData(vector<KeyPoint> keypoints);
    string static to_string(int i);
};

