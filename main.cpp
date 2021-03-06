#include <QCoreApplication>

#include <stdio.h>
#include <iostream>

#include <QThread>

#include <opengev.h>

#include "app.h"

#include <string.h>

#include <stdlib.h>

//#include <X11/Xlib.h>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    OpenGEV::configure();

    /*
    App* myObj = new App(); // 0 = no parent
    QThread* thread = new QThread;
    myObj->moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), myObj, SLOT(run()));
    thread->start();
    */

    //XInitThreads();

    std::cout<<argv[1]<<std::endl;

    char* p;
    int channels = strtol(argv[1], &p, 10);
    int packetSize = strtol(argv[2], &p, 10);
    int packetDelay = strtol(argv[3], &p, 10);

    App myObj (channels, packetSize, packetDelay);
    myObj.start();

    return a.exec();
}
