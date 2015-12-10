#ifndef RGBOBSERVER_H
#define RGBOBSERVER_H

#include <QObject>

#include <log4cpp/Category.hh>

#include "Application/streamdatareceiver.h"

#include "pcl/point_cloud.h"

#include "pcl/point_types.h"

#include <chrono>

#include "simpleviewer.h"

class RgbObserver : public QObject
{
    Q_OBJECT
public:
    explicit RgbObserver(QObject* parent = 0);

    RgbObserver(StreamDataReceiver* channel);

    std::chrono::steady_clock::time_point start;

    std::chrono::steady_clock::time_point end;

    //SimpleViewer simpleViewer;

    const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr ptrCloud;

signals:
    void newStreamData(const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr &cloud);

public slots:
    void refreshStartTime();
    void update();

private:
    log4cpp::Category &logger = log4cpp::Category::getInstance("ComponentLog");

    pcl::PointCloud<pcl::PointXYZRGBA> cloud;



    StreamDataReceiver *channel;

    long meanTimeMs;
};

#endif // RGBOBSERVER_H
