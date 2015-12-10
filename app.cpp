#include "app.h"

App::App(int channels)
    : channels(channels)
{
    app = new GVApplication();
    connect(this, SIGNAL(initialization()),this,SLOT(setupTimer()));

    simpleViewer = new SimpleViewer(channels);
}

void App::checkSlot()
{
    if(initPartnerDevice) {
        if(app->discoverDevice()>0) {

            foreach (PartnerDevice aDevice, app->getDiscoveredDevice()) {
                std::cout<<aDevice.manufactureName.toStdString()<<std::endl;
                std::cout<<aDevice.modelName.toStdString()<<std::endl;
                std::cout<<aDevice.deviceVersion.toStdString()<<std::endl;
                std::cout<<aDevice.macAddress.toStdString()<<std::endl;
                std::cout<<aDevice.ipAddress.toString().toStdString()<<std::endl;
                std::cout<<aDevice.subnetMask.toString().toStdString()<<std::endl;
                std::cout<<aDevice.defaultGateway.toString().toStdString()<<std::endl;
            }

            pdevice = app->getDiscoveredDevice().at(0);
            initPartnerDevice = false;

        }
    } else {
        if(initControlChannel) {
            if(pdevice.openControlChannel(5001))
                initControlChannel = false;
        } else {
            if(initStreamChannel) {
                if(pdevice.getStreamingChannelNumber()>=0) {

                    if((channels & 0xF00)==0x100) {
                        if(pdevice.openStreamChannel(0)==GEV_STATUS_SUCCESS) {
                            pdevice.setStreamChannelDelay(0,10000);
                            pdevice.setStreamChannelPacketLength(0,65000);
                            obs1 = new DepthObserver(pdevice.getStreamChannel(0));

                            connect(obs1, SIGNAL(newStreamData(const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr)),
                                    simpleViewer, SLOT(updateViewer1(const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr)));
                        }
                    }

                    if((channels & 0x0F0)==0x010) {
                        if(pdevice.openStreamChannel(1)==GEV_STATUS_SUCCESS) {

                            pdevice.setStreamChannelDelay(1,10000);
                            pdevice.setStreamChannelPacketLength(1,65000);
                            obs2 = new RgbObserver(pdevice.getStreamChannel(1));

                            connect(obs2, SIGNAL(newStreamData(const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr)),
                                    simpleViewer, SLOT(updateViewer2(const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr)));
                        }
                    }

                    if((channels & 0x00F)==0x001) {
                        if(pdevice.openStreamChannel(2)==GEV_STATUS_SUCCESS) {

                            pdevice.setStreamChannelDelay(2,10000);
                            pdevice.setStreamChannelPacketLength(2,65000);
                            obs3 = new DepthRgbObserver(pdevice.getStreamChannel(2));
                            connect(obs3, SIGNAL(newStreamData(const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr)),
                                    simpleViewer, SLOT(updateViewer3(const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr)));
                        }
                    }

                    initStreamChannel = false;
                    disconnect(this, SIGNAL(initialization()),this,SLOT(setupTimer()));
                    timer->stop();
                }
            }
        }
    }
}

void App::setupTimer()
{
    timer = new QTimer(this);
    timer->setInterval(30);
    connect(timer,SIGNAL(timeout()),this,SLOT(checkSlot()));
    timer->start();
}

void App::run()
{
    emit initialization();
    exec();
}
