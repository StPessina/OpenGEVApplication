#include "app.h"

App::App(int channels, int packetSize, int packetDelay)
    : channels(channels), packetSize(packetSize), packetDelay(packetDelay)
{
    app = new GVApplication();
    connect(this, SIGNAL(initialization()),this,SLOT(setupTimer()));

    simpleViewer = new SimpleViewer(channels);
}

void App::checkSlot()
{
    if(initPartnerDevice) {
        if(app->discoverDevices()>0) {

            foreach (PartnerDevice aDevice, app->getDiscoveredDevices()) {
                std::cout<<aDevice.manufactureName.toStdString()<<std::endl;
                std::cout<<aDevice.modelName.toStdString()<<std::endl;
                std::cout<<aDevice.deviceVersion.toStdString()<<std::endl;
                std::cout<<aDevice.macAddress.toStdString()<<std::endl;
                std::cout<<aDevice.ipAddress.toString().toStdString()<<std::endl;
                std::cout<<aDevice.subnetMask.toString().toStdString()<<std::endl;
                std::cout<<aDevice.defaultGateway.toString().toStdString()<<std::endl;
            }

            pdevice = app->getDiscoveredDevices().at(0);
            initPartnerDevice = false;

        }
    } else {
        if(initControlChannel) {
            if(pdevice.openControlChannel(5001))
                initControlChannel = false;
        } else {
            if(initStreamChannel) {
                if(pdevice.getStreamingChannelNumber()>=0) {

                    hfov = pdevice.getHorizontalFieldOfView();
                    vfov = pdevice.getVerticalFieldOfView();

                    std::cout<<"HFOV: "<<hfov<<"; VFOV: "<<vfov<<std::endl;

                    if((channels & 0xF00)==0x100) {
                        if(pdevice.openStreamChannel(0)==GEV_STATUS_SUCCESS) {
                            pdevice.setStreamChannelDelay(0,packetDelay);
                            pdevice.setStreamChannelPacketLength(0,packetSize);
                            obs1 = new DepthStreamDataObserver(*pdevice.getStreamChannel(0),hfov,vfov);

                            connect(obs1, SIGNAL(pointCloudUpdate(const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr)),
                                    simpleViewer, SLOT(updateViewer1(const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr)));
                        }
                    }

                    if((channels & 0x0F0)==0x010) {
                        if(pdevice.openStreamChannel(1)==GEV_STATUS_SUCCESS) {

                            pdevice.setStreamChannelDelay(1,packetDelay);
                            pdevice.setStreamChannelPacketLength(1,packetSize);
                            obs2 = new ColorStreamDataObserver(*pdevice.getStreamChannel(1),hfov,vfov);

                            connect(obs2, SIGNAL(pointCloudUpdate(const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr)),
                                    simpleViewer, SLOT(updateViewer2(const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr)));
                        }
                    }

                    if((channels & 0x00F)==0x001) {
                        if(pdevice.openStreamChannel(2)==GEV_STATUS_SUCCESS) {

                            pdevice.setStreamChannelDelay(2,packetDelay);
                            pdevice.setStreamChannelPacketLength(2,packetSize);
                            obs3 = new DepthColorStreamDataObserver(*pdevice.getStreamChannel(2),hfov,vfov);
                            connect(obs3, SIGNAL(pointCloudUpdate(const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr)),
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
