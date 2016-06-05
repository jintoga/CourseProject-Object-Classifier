#include <QCoreApplication>
#include "utils.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Step 1: Obtain the set of bags of features.
    Utils::buildDictionary();
    Utils::trainClassesData();
    return 0;
}

