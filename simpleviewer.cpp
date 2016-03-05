#include "simpleviewer.h"

SimpleViewer::SimpleViewer(int channels)
    : viewer("Viewer"),
      v1(0), v2(0), v3(0)
{
    viewer.initCameraParameters ();

    if(channels==0x111)
        configureThreeViewport("Depth stream", "IR", v1,
                               "RGB stream", "RGB", v2,
                               "RBG-Depth stream", "IRRGB", v3);

    if(channels==0x110)
        configureTwoViewport("Depth stream", "IR", v1,
                               "RGB stream", "RGB", v2);

    if(channels==0x101)
        configureTwoViewport("Depth stream", "IR", v1,
                               "RBG-Depth stream", "IRRGB", v3);

    if(channels==0x011)
        configureTwoViewport("RGB stream", "RGB", v2,
                               "RBG-Depth stream", "IRRGB", v3);

    if(channels==0x100)
        configureOneViewport("Depth stream", "IR", v1);

    if(channels==0x010)
        configureOneViewport("RGB stream", "RGB", v2);

    if(channels==0x001)
        configureOneViewport("RBG-Depth stream", "IRRGB", v3);

    //viewer.setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "IR");
    //viewer.setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "RGB");
    viewer.addCoordinateSystem (1.0);
}

void SimpleViewer::updateViewer1()
{
    viewer.updatePointCloud(obs1->ptrCloud,"IR");
    viewer.spinOnce();
}


void SimpleViewer::updateViewer2()
{
    viewer.updatePointCloud(obs2->ptrCloud,"RGB");
    viewer.spinOnce();
}

void SimpleViewer::updateViewer3()
{
    viewer.updatePointCloud(obs3->ptrCloud,"IRRGB");
    viewer.spinOnce();
}

void SimpleViewer::configureOneViewport(std::string viewportname,
                                        std::string id,
                                        int &viewportid)
{
    //viewer = new pcl::visualization::CloudViewer("Viewer");
    viewer.createViewPort (0.0, 0.0, 1.0, 1.0, viewportid);
    viewer.setBackgroundColor (0.0, 0.0, 0.0, viewportid);
    viewer.addText (viewportname, 10, 10, "v1 text", viewportid);

    const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGBA>);

    viewer.addPointCloud(cloud,id,viewportid);
}

void SimpleViewer::configureTwoViewport(std::string viewportname1, std::string id1, int &viewportid1,
                                        std::string viewportname2, std::string id2, int &viewportid2)
{
    //viewer = new pcl::visualization::CloudViewer("Viewer");
    viewer.createViewPort (0.0, 0.0, 0.5, 1.0, viewportid1);
    viewer.setBackgroundColor (0.0, 0.0, 0.0, viewportid1);
    viewer.addText (viewportname1, 10, 10, "v1 text", viewportid1);

    //viewer = new pcl::visualization::CloudViewer("Viewer");
    viewer.createViewPort (0.5, 0.0, 1.0, 1.0, viewportid2);
    viewer.setBackgroundColor (0.2, 0.2, 0.2, viewportid2);
    viewer.addText (viewportname2, 10, 10, "v2 text", viewportid2);

    const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud1 (new pcl::PointCloud<pcl::PointXYZRGBA>);
    const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud2 (new pcl::PointCloud<pcl::PointXYZRGBA>);

    viewer.addPointCloud(cloud1,id1,viewportid1);
    viewer.addPointCloud(cloud2,id2,viewportid2);
}

void SimpleViewer::configureThreeViewport(std::string viewportname1, std::string id1, int &viewportid1,
                                          std::string viewportname2, std::string id2, int &viewportid2,
                                          std::string viewportname3, std::string id3, int &viewportid3)
{
    viewer.createViewPort (0.0, 0.0, 0.5, 0.5, viewportid1);
    viewer.setBackgroundColor (0, 0, 0, viewportid1);
    viewer.addText (viewportname1, 10, 10, "v1 text", viewportid1);

    viewer.createViewPort (0.5, 0.0, 1.0, 0.5, viewportid2);
    viewer.setBackgroundColor (0.15, 0.15, 0.15, viewportid2);
    viewer.addText (viewportname2, 10, 10, "v2 text", viewportid2);

    viewer.createViewPort (0.0, 0.5, 1.0, 1.0, viewportid3);
    viewer.setBackgroundColor (0.3, 0.3, 0.3, viewportid3);
    viewer.addText (viewportname3, 10, 10, "v3 text", viewportid3);

    const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud1 (new pcl::PointCloud<pcl::PointXYZRGBA>);
    const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud2 (new pcl::PointCloud<pcl::PointXYZRGBA>);
    const pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud3 (new pcl::PointCloud<pcl::PointXYZRGBA>);

    viewer.addPointCloud(cloud1,id1,viewportid1);
    viewer.addPointCloud(cloud2,id2,viewportid2);
    viewer.addPointCloud(cloud3,id3,viewportid3);
}


