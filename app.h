#ifndef APP_H
#define APP_H

#include <QThread>

#include <string>

#include <opengev.h>

#include <Application/gvapplication.h>

#include <Device/gvdevice.h>
#include <DeviceCommandHandler/devicecommandhandlerfactory.h>

#include "CommonStream/streamrawdataleader.h"
#include "CommonStream/streamrawdatapayload.h"
#include "CommonStream/streamrawdatatrailer.h"

#include "iostream"

#include <ApplicationStreamDataObserver/depthstreamdataobserver.h>
#include <ApplicationStreamDataObserver/colorstreamdataobserver.h>
#include <ApplicationStreamDataObserver/depthcolorstreamdataobserver.h>

#include "simpleviewer.h"

#define STREAM_PACKET_DELAY 10000
#define STREAM_PACKET_SIZE 576

#define DEPTH 0x100
#define RGB 0x010
#define DEPTHRBG 0x001

#define DEPTH_RGB (DEPTH | RGB)
#define DEPTH_RGB_DEPTHRBG (DEPTH | RGB | DEPTHRGB)

class App : public QThread
{
    Q_OBJECT
public:
    App(int channels, int packetSize, int packetDelay);

    std::string a = "MAN_NAME";
    std::string b = "MODEL_NAME";
    std::string c = "DEVICE_VERSION";


    GVApplication* app;

    PartnerDevice pdevice;

    DepthStreamDataObserver* obs1;
    ColorStreamDataObserver* obs2;
    DepthColorStreamDataObserver* obs3;

    QTimer* timer;

    SimpleViewer* simpleViewer;

    bool initPartnerDevice = true;

    bool initControlChannel = true;

    bool initStreamChannel = true;

    int channels;

    int packetSize;

    int packetDelay;

    int vfov, hfov;

signals:
    void initialization();

public slots:
    void checkSlot();

    void setupTimer();

protected:
    void run();
};

#endif // APP_H
