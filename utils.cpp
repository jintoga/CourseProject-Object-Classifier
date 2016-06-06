#include "utils.h"
Utils::Utils()
{

}

void Utils::buildDictionary()
{
    //To store the keypoints that will be extracted by Sift
    vector<KeyPoint> keypoints;
    //To store the Sift descriptor of current image
    Mat descriptor;
    //To store all the descriptors that are extracted from all the images.
    Mat unclusteredFeatures;

    SiftDescriptorExtractor detector;

    Mat image;
    for (int i = 1; i <= 400; ++i) {
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
    //define Term Criteria
    TermCriteria termCriteria(CV_TERMCRIT_ITER, 100, 0.001);
    //retries number
    int retries = 1;
    //Create the BoW trainer
    BOWKMeansTrainer bowTrainer(CLUSTERS_NUM, termCriteria, retries, KMEANS_PP_CENTERS);
    //cluster the feature vectors
    Mat dictionary = bowTrainer.cluster(unclusteredFeatures);
    //store the vocabulary
    FileStorage fs(DICTIONARY_PATH, FileStorage::WRITE);
    fs << VOCABULARY << dictionary;
    fs.release();
    printf("Dictionary file was created\n");
}

void Utils::trainClassesData()
{
    //prepare BOW descriptor extractor from the dictionary
    Mat dictionary;
    FileStorage fs(DICTIONARY_PATH, FileStorage::READ);
    fs[VOCABULARY] >> dictionary;
    fs.release();

    //create a nearest neighbor matcher
    Ptr<DescriptorMatcher> matcher(new FlannBasedMatcher);
    //create Sift feature point extracter
    Ptr<FeatureDetector> detector(new SiftFeatureDetector());
    //create Sift descriptor extractor
    Ptr<DescriptorExtractor> extractor(new SiftDescriptorExtractor);
    //create BoW descriptor extractor
    BOWImgDescriptorExtractor bowIDE(extractor, matcher);
    //Set the dictionary with the vocabulary we created in the first step
    bowIDE.setVocabulary(dictionary);

    //Train SVM
    //setup training data for classifiers
    map<string,Mat> classesTrainingData;
    classesTrainingData.clear();

    Mat image;
    Mat labels(0, 1, CV_32FC1);
    Mat trainingData(0, 400, CV_32FC1);
    vector<KeyPoint> keypoint;
    Mat bowDescriptor1;

    //extracting histogram in the form of bow for each image
    for (int i = 1; i <= 400; ++i) {

        string filename = TRAIN_DATA_PATH + Utils::to_string(i) + ".jpg";
        image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
        detector->detect(image, keypoint);

        bowIDE.compute(image, keypoint, bowDescriptor1);

        trainingData.push_back(bowDescriptor1);
        labels.push_back((float) i);
    }

    //Setting up SVM parameters
    CvSVMParams params;
    params.kernel_type=CvSVM::RBF;
    params.svm_type=CvSVM::C_SVC;
    params.gamma=0.50625000000000009;
    params.C=312.50000000000000;
    params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER, 100, 0.000001);
    CvSVM svm;
    printf("%s\n","Training SVM classifier");

    bool res=svm.train(trainingData, labels, cv::Mat(), cv::Mat(), params);

    if(res){
        printf("trained SVMs\n");
    }


    vector<KeyPoint> keypoint2;
    Mat bowDescriptor2;
    printf("Testing image...\n");
    string filename ="C:\\Users\\DAT\\Desktop\\test.jpg";
    image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
    detector->detect(image, keypoint2);

    detector->detect(image, keypoint2);
    bowIDE.compute(image, keypoint2, bowDescriptor2);

    float response = svm.predict(bowDescriptor2);
    printf("result: %f\n", response);
    imshow( "window1", image );
    waitKey(0);

}


string Utils::to_string(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}
