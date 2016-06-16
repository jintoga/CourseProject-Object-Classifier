#include <QCoreApplication>
#include "utils.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Utils::buildDictionary();
    Utils::trainClassesData();
    return 0;
}

