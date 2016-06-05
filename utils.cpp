#include "utils.h"
Utils::Utils()
{

}

void Utils::buildDictionary()
{
    //To store the keypoints that will be extracted by SURF
    vector<KeyPoint> keypoints;
    //To store the SURF descriptor of current image
    Mat descriptor;
    //To store all the descriptors that are extracted from all the images.
    Mat unclusteredFeatures;

    SurfDescriptorExtractor detector;

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

    Mat image;
    Mat labels(0, 1, CV_32FC1);
    Mat trainingData(0, 200, CV_32FC1);
    vector<KeyPoint> keypoint;
    Mat bowDescriptor1;

    SurfFeatureDetector surfDetector(500);
    //extracting histogram in the form of bow for each image
    for (int i = 1; i <= 400; ++i) {

        string filename = TRAIN_DATA_PATH + Utils::to_string(i) + ".jpg";
        image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
        surfDetector.detect(image, keypoint);


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

    printf("Processing evaluation data...\n");


    Mat groundTruth(0, 1, CV_32FC1);
    Mat evalData(0, 200, CV_32FC1);
    vector<KeyPoint> keypoint2;
    Mat bowDescriptor2;
    Mat results(0, 1, CV_32FC1);;
    for (int i = 1; i <= 200; ++i) {
        string filename = TEST_DATA_PATH + Utils::to_string(i) + ".jpg";
        image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);

        surfDetector.detect(image, keypoint2);
        bowIDE.compute(image, keypoint2, bowDescriptor2);

        evalData.push_back(bowDescriptor2);
        groundTruth.push_back((float) i);
        float response = svm.predict(bowDescriptor2);
        results.push_back(response);
        printf("img%d: %f\n",i, response);
    }


    //calculate the number of unmatched classes
    double errorRate = (double) countNonZero(groundTruth- results) / evalData.rows;
    printf("Error rate: %f\n",errorRate);
}


string Utils::to_string(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}
