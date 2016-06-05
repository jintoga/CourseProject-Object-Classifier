#include "utils.h"
Utils::Utils()
{

}

void Utils::buildDictionary(const string path, const int numberOfClusters)
{
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
        string filename = path + Utils::to_string(i) + ".jpg";
        //open the file
        image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
        detector.detect(image, keypoints);
        //compute the descriptors for each keypoint
        detector.compute(image, keypoints,descriptor);
        //put the all feature descriptors in a single Mat object
        unclusteredFeatures.push_back(descriptor);
        printf("extracted: %s(%dx%d)\n", filename.c_str(), image.size().width, image.size().height);
    }
    //Construct BOWKMeansTrainer
    //number of clusters(the number of bags)
    int dictionarySize = numberOfClusters;
    //define Term Criteria
    TermCriteria termCriteria(CV_TERMCRIT_ITER, 100, 0.001);
    //retries number
    int retries = 1;
    //Create the BoW (or BoF) trainer
    BOWKMeansTrainer bowTrainer(dictionarySize, termCriteria, retries, KMEANS_PP_CENTERS);
    //cluster the feature vectors
    Mat dictionary = bowTrainer.cluster(unclusteredFeatures);
    //store the vocabulary
    string dictionaryPath = path + "dictionary.yml";
    FileStorage fs(dictionaryPath, FileStorage::WRITE);
    fs << "vocabulary" << dictionary;
    fs.release();
    printf("Dictionary file was created: %s\n", dictionaryPath.c_str());
}


std::string Utils::to_string(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}
