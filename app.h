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

#include "depthobserver.h"
#include "rgbobserver.h"
#include "depthrgbobserver.h"

#include "simpleviewer.h"

#define DEPTH 0x100
#define RGB 0x010
#define DEPTHRBG 0x001

#define DEPTH_RGB (DEPTH | RGB)
#define DEPTH_RGB_DEPTHRBG (DEPTH | RGB | DEPTHRGB)

class App : public QThread
{
    Q_OBJECT
public:
    App(int channels);

    std::string a = "MAN_NAME";
    std::string b = "MODEL_NAME";
    std::string c = "DEVICE_VERSION";


    GVApplication* app;

    PartnerDevice pdevice;

    DepthObserver* obs1;
    RgbObserver* obs2;
    DepthRgbObserver* obs3;

    QTimer* timer;

    SimpleViewer* simpleViewer;

    bool initPartnerDevice = true;

    bool initControlChannel = true;

    bool initStreamChannel = true;

    int channels;

signals:
    void initialization();

public slots:
    void checkSlot();

    void setupTimer();

protected:
    void run();
};

#endif // APP_H
