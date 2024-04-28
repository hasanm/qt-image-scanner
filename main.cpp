#include "mainwindow.h"
#include <QApplication>

#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace cv;
using namespace std;


int main(int argc, char *argv[])
{
    // Seed Random Number Generator
    srand(time(nullptr));

    QApplication a(argc, argv);

    MainWindow w;
    w.show();


    return a.exec();
}
