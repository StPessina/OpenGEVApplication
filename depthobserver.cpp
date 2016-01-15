#include "depthobserver.h"

DepthObserver::DepthObserver(StreamDataReceiver *channel)
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

void DepthObserver::refreshStartTime()
{
    start = end = std::chrono::steady_clock::now();
}

void DepthObserver::update()
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

    const quint16* data = (const quint16*) map->data;

    float scale = cloud.width / (float) 1280;
    float focalLength = 1050 * scale;
    //float focalLength = depth_focal_length_SXGA_ * scale;

    float constant_x = 1.0f / focalLength;
    float constant_y = 1.0f / focalLength;

    float centerX = ((float)cloud.width - 1.f) / 2.f;
    float centerY = ((float)cloud.height - 1.f) / 2.f;

    float bad_point = std::numeric_limits<float>::quiet_NaN();

    int depth_idx = 0;
    for (int v = 0; v < cloud.height; ++v)
    {
        for (int u = 0; u < cloud.width; ++u, ++depth_idx)
        {
            pcl::PointXYZRGBA& pt = cloud.points[depth_idx];
            // Check for invalid measurements
            if (data[depth_idx] == 0
                    //||
                    //data[depth_idx] == depth_image->getNoSampleValue () ||
                    //data[depth_idx] == depth_image->getShadowValue ()
                    )
            {
                // not valid
                pt.x = pt.y = pt.z = bad_point;
                continue;
            }
            pt.z = data[depth_idx] * 0.001f;
            pt.x = (static_cast<float> (u) - centerX) * pt.z * constant_x;
            pt.y = (static_cast<float> (v) - centerY) * pt.z * constant_y;
            pt.r = 255;
            pt.g = 255;
            pt.b = 255;
        }
    }

    cloud.sensor_origin_.setZero ();
    cloud.sensor_orientation_.w () = 1.0f;
    cloud.sensor_orientation_.x () = 0.0f;
    cloud.sensor_orientation_.y () = 0.0f;
    cloud.sensor_orientation_.z () = 0.0f;

    end = std::chrono::steady_clock::now();

    meanTimeMs = (meanTimeMs+std::chrono::duration_cast<std::chrono::microseconds>(end- start).count())/2;

    std::cout<<meanTimeMs<<std::endl;

    emit newStreamData(ptrCloud);
}

/*
float OpenNIDevice::getDepthFocalLength (int output_x_resolution) const throw ()
00255 {
00256   if (output_x_resolution == 0)
00257     output_x_resolution = getDepthOutputMode ().nXRes;
00258
00259   float scale = output_x_resolution / (float)XN_SXGA_X_RES;
00260   if (isDepthRegistered ())
00261     return rgb_focal_length_SXGA_ * scale;
00262   else
00263     return depth_focal_length_SXGA_ * scale;
00264 }
*/
