#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <string>
#include <random>

using namespace cv;
using namespace std;

#define VOCABULARY "vocabulary"
#define TRAIN_DATA_PATH "C:\\Users\\Nguyen\\Desktop\\tr\\"
#define DICTIONARY_PATH "C:\\Users\\Nguyen\\Desktop\\tr\\dictionary.yml"
#define TEST_DATA_PATH "C:\\Users\\Nguyen\\Desktop\\test1\\"

class Utils
{
private:
    const static int CLUSTERS_NUM = 100; //dictionary size
public:
    Utils();
    void static buildDictionary();
    void static trainClassesData();
    string static to_string(int i);
};

