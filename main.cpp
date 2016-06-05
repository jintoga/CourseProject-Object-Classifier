#include <QCoreApplication>
#include "utils.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Step 1: Obtain the set of bags of features.
    vector<KeyPoint> keypoints = Utils::buildDictionary(200);
    Utils::trainClassesData(keypoints);
    return 0;
}

