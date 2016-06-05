#include <QCoreApplication>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //Step 1: Obtain the set of bags of features.
    //to store the input file names
    char * filename = new char[100];

    //To store the keypoints that will be extracted by SIFT
    vector<KeyPoint> keypoints;
    //To store the SIFT descriptor of current image
    Mat descriptor;
    //To store all the descriptors that are extracted from all the images.
    Mat unclusteredFeatures;

    SurfDescriptorExtractor detector;

    Mat image;
    for (int i = 1; i <= 10; ++i) {
        //create the file name of an image
        sprintf(filename,"C:\\Users\\DAT\\Desktop\\train_data\\%i.jpg", i);
        //open the file
        image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
        detector.detect(image, keypoints);
        //compute the descriptors for each keypoint
        detector.compute(image, keypoints,descriptor);
        //put the all feature descriptors in a single Mat object
        unclusteredFeatures.push_back(descriptor);
        printf("extracted: %s(%dx%d)\n", filename, image.size().width, image.size().height);
    }
    //Construct BOWKMeansTrainer
    //number of clusters(the number of bags)
    int dictionarySize=200;
    //define Term Criteria
    TermCriteria termCriteria(CV_TERMCRIT_ITER,100,0.001);
    //retries number
    int retries=1;
    //Create the BoW (or BoF) trainer
    BOWKMeansTrainer bowTrainer(dictionarySize, termCriteria, retries, KMEANS_PP_CENTERS);
    //cluster the feature vectors
    Mat dictionary = bowTrainer.cluster(unclusteredFeatures);
    //store the vocabulary
    FileStorage fs("C:\\Users\\DAT\\Desktop\\dictionary.yml", FileStorage::WRITE);
    fs << "vocabulary" << dictionary;
    fs.release();


    return 0;
}

