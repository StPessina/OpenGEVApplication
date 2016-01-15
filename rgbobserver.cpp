#include "rgbobserver.h"

RgbObserver::RgbObserver(StreamDataReceiver *channel)
    : ptrCloud(&cloud)
{
    connect(channel,SIGNAL(newStreamDataAvailable()),
            this,SLOT(update()));
    connect(channel,SIGNAL(startGetStreamData()),
            this,SLOT(refreshStartTime()));
    this->channel = channel;
    start = end = std::chrono::steady_clock::now();

    //simpleViewer.updateViewer(ptrCloud);
}

void RgbObserver::refreshStartTime()
{
    start = end = std::chrono::steady_clock::now();
}

void RgbObserver::update()
{    
    PixelMap::Ptr map = channel->getStreamData();

    /*
    bool check = true;
    int pixelCount = 0;
    int i = 0;
    for (i = 0; i < map->dataLength; ++i) {
        if(map->data[i]!=0x45) {
            check = false;
            break;
        }
        pixelCount++;
    }

    end = std::chrono::steady_clock::now();

    if(!check)
    std::cout << "Recostruction stream time "
                  << std::chrono::duration_cast<std::chrono::microseconds>(end- start).count()
                  << "us. (Missing pixel: "<<(map->pixelMapSize-pixelCount/2)
                  <<"(at "<<i<<")"<<std::endl;
    */


    cloud.width=map->sizex;
    cloud.height=map->sizey;
    cloud.is_dense=false;
    cloud.points.resize(map->sizex*map->sizey);

    const char* data = (const char*) map->data;

    float scale = cloud.width / (float) 1280;
    float focalLength = 1050 * scale;
    //float focalLength = depth_focal_length_SXGA_ * scale;

    float constant_x = 1.0f / focalLength;
    float constant_y = 1.0f / focalLength;

    float centerX = ((float)cloud.width - 1.f) / 2.f;
    float centerY = ((float)cloud.height - 1.f) / 2.f;

    int depth_idx = 0;
    int char_idx = 0;
    for (int v = 0; v < cloud.height; ++v)
    {
        for (int u = 0; u < cloud.width; ++u, ++depth_idx, char_idx+=3)
        {
            pcl::PointXYZRGBA& pt = cloud.points[depth_idx];

            pt.z = 0.001f;
            pt.x = (static_cast<float> (u) - centerX) * constant_x;
            pt.y = (static_cast<float> (v) - centerY) * constant_y;
            pt.r = data[char_idx];
            pt.g = data[char_idx+1];
            pt.b = data[char_idx+2];
        }
    }

    cloud.sensor_origin_.setZero ();
    cloud.sensor_orientation_.w () = 0.0f;
    cloud.sensor_orientation_.x () = 0.0f;
    cloud.sensor_orientation_.y () = 0.0f;
    cloud.sensor_orientation_.z () = 0.0f;

    end = std::chrono::steady_clock::now();

    meanTimeMs = (meanTimeMs+std::chrono::duration_cast<std::chrono::microseconds>(end- start).count())/2;

    std::cout<<meanTimeMs<<std::endl;

    emit newStreamData(ptrCloud);
}
