#include <QCoreApplication>
#include "utils.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Step 1: Obtain the set of bags of features.

    Utils::buildDictionary("C:\\Users\\DAT\\Desktop\\train_data\\", 200);

    return 0;
}

