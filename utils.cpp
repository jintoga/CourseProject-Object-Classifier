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
    for (int i = 1; i <= 4; ++i) {
        for (int j = 1; j <= 60; ++j) {
            //create the file name of an image
            string filename = TRAIN_DATA_PATH + Utils::to_string(j) + "-" + Utils::to_string(i) + ".jpg";
            //open the file
            image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
            detector.detect(image, keypoints);
            //compute the descriptors for each keypoint
            detector.compute(image, keypoints,descriptor);
            //put the all feature descriptors in a single Mat object
            unclusteredFeatures.push_back(descriptor);
            printf("extracted: %s(%dx%d)\n", filename.c_str(), image.size().width, image.size().height);
        }
    }
    //Construct BOWKMeansTrainer
    //define Term Criteria
    TermCriteria termCriteria(CV_TERMCRIT_ITER, 100, 0.001);
    //retries number
    int retries = 1;
    //Create the BoW trainer
    printf("Clustering features...\n");
    BOWKMeansTrainer bowTrainer(CLUSTERS_NUM, termCriteria, retries, KMEANS_PP_CENTERS);
    //cluster the feature vectors
    Mat dictionary = bowTrainer.cluster(unclusteredFeatures);
    printf("Done clustering features.\n");
    printf("Creating Dictionary File...\n");
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

    Ptr<DescriptorMatcher> matcher(new FlannBasedMatcher);
    Ptr<DescriptorExtractor> extractor(new SiftDescriptorExtractor);
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

    SiftFeatureDetector SiftDetector(1500);
    printf("Extracting histograms\n");
    //extracting histogram in the form of bow for each image
    for (int i = 1; i <= 4; ++i) {
        for (int j = 1; j <= 60; ++j) {

            string filename = TRAIN_DATA_PATH + Utils::to_string(j) + "-" + Utils::to_string(i) + ".jpg";
            image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
            SiftDetector.detect(image, keypoint);

            bowIDE.compute(image, keypoint, bowDescriptor1);

            trainingData.push_back(bowDescriptor1);

            labels.push_back((float) i);
        }
    }

    printf("Done extracting histograms\n");
    //Setting up SVM parameters
    CvSVMParams params;
    params.kernel_type=CvSVM::RBF;
    params.svm_type=CvSVM::C_SVC;
    params.gamma=0.50625000000000009;
    params.C=312.50000000000000;
    params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER, 100, 0.000001);

    CvSVM svm;
    printf("Training SVM classifier\n");
    bool res = svm.train(trainingData, labels, cv::Mat(), cv::Mat(), params);

    if(res){
        printf("Done training SVMs\n");

        //Testing data
        Utils::testData(bowIDE, svm);
    }else{
        printf("Error training SVMs!!!\n");
    }


}

void Utils::testData(BOWImgDescriptorExtractor &bowIDE, CvSVM &svm)
{

    Ptr<FeatureDetector> detector(new SiftFeatureDetector());
    Mat image;

    vector<KeyPoint> keypoint2;
    Mat bowDescriptor2;

    printf("Start testing images...\n");
    for (int i = 1; i <= 80; ++i) {

        string filename = TEST_DATA_PATH + Utils::to_string(i) +  ".jpg";

        image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
        detector->detect(image, keypoint2);

        detector->detect(image, keypoint2);
        bowIDE.compute(image, keypoint2, bowDescriptor2);

        float response = svm.predict(bowDescriptor2);

        string className;
        if(response == 1){
            className = "Plane";
        }
        else if(response == 2){
            className = "Car";
        }
        else if(response == 3){
            className = "Leopard";
        }
        else if(response == 4){
            className = "Mobike";
        }


        printf("This is an image of: %s\n", className.c_str());
        imshow("Testing", image );
        resizeWindow("Testing",600,600);

        waitKey(0);

        Mat cleaner(Mat(600, 600, CV_8U));
        cleaner = cv::Scalar(0);
        imshow("Testing", cleaner);
    }


    printf("Done testing images.\n");
    waitKey(0);
}


string Utils::to_string(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}
