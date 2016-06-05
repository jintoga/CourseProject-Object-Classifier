#include "utils.h"
Utils::Utils()
{

}

vector<KeyPoint> Utils::buildDictionary(const int numberOfClusters)
{
    //To store the keypoints that will be extracted by SURF
    vector<KeyPoint> keypoints;
    //To store the SURF descriptor of current image
    Mat descriptor;
    //To store all the descriptors that are extracted from all the images.
    Mat unclusteredFeatures;

    SurfDescriptorExtractor detector;

    Mat image;
    for (int i = 1; i <= 10; ++i) {
        //create the file name of an image
        string filename = TRAIN_DATA_PATH + Utils::to_string(i) + ".jpg";
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
    //Create the BoW trainer
    BOWKMeansTrainer bowTrainer(dictionarySize, termCriteria, retries, KMEANS_PP_CENTERS);
    //cluster the feature vectors
    Mat dictionary = bowTrainer.cluster(unclusteredFeatures);
    //store the vocabulary
    FileStorage fs(DICTIONARY_PATH, FileStorage::WRITE);
    fs << VOCABULARY << dictionary;
    fs.release();
    printf("Dictionary file was created\n");
    return keypoints;
}

void Utils::trainClassesData(vector<KeyPoint> keypoints)
{
    //prepare BOW descriptor extractor from the dictionary
    Mat dictionary;
    FileStorage fs(DICTIONARY_PATH, FileStorage::READ);
    fs[VOCABULARY] >> dictionary;
    fs.release();

    //create a nearest neighbor matcher
    Ptr<DescriptorMatcher> matcher(new FlannBasedMatcher);
    //create SURF feature point extracter
    Ptr<FeatureDetector> detector(new SurfFeatureDetector());
    //create SURF descriptor extractor
    Ptr<DescriptorExtractor> extractor(new SurfDescriptorExtractor);
    //create BoW descriptor extractor
    BOWImgDescriptorExtractor bowIDE(extractor, matcher);
    //Set the dictionary with the vocabulary we created in the first step
    bowIDE.setVocabulary(dictionary);

    //Train SVM
    //setup training data for classifiers
    map<string,Mat> classesTrainingData;
    classesTrainingData.clear();

    Mat responseHistogram;
    Mat image;
    for (int i = 1; i <= 10; ++i) {
        //create the file name of an image
        string filename = TRAIN_DATA_PATH + Utils::to_string(i) + ".jpg";
        //open the file
        image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
        bowIDE.compute(image, keypoints, responseHistogram);

        if(classesTrainingData.count(filename) == 0) { //not yet created...
            classesTrainingData[filename].create(0, responseHistogram.cols, responseHistogram.type());
        }


        printf("classesTrainingData:%d\n",classesTrainingData.count(filename));
        classesTrainingData[filename].push_back(responseHistogram);
    }

    printf("trained\n");
}


string Utils::to_string(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}
