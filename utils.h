#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <string>


using namespace cv;
using namespace std;

class Utils
{
public:
    Utils();
    void static buildDictionary(const string path, const int numberOfClusters);
    std::string static to_string(int i);
};

