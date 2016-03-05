#ifndef SIMPLEVIEWER_H
#define SIMPLEVIEWER_H

#include <QObject>

#include "ApplicationStreamDataObserver/abstractstreamdataobserver.h"

#include <pcl/visualization/cloud_viewer.h>


class SimpleViewer : public QObject
{
    Q_OBJECT
  public:
    SimpleViewer(int channels);

    pcl::visualization::PCLVisualizer viewer;

    AbstractStreamDataObserver* obs1;
    AbstractStreamDataObserver* obs2;
    AbstractStreamDataObserver* obs3;

public slots:
    void updateViewer1();
    void updateViewer2();
    void updateViewer3();


private:
    int v1,v2,v3;

    void configureOneViewport(std::string viewportname, std::string id, int &viewportid);

    void configureTwoViewport(std::string viewportname1, std::string id1, int &viewportid1,
                              std::string viewportname2, std::string id2, int &viewportid2);

    void configureThreeViewport(std::string viewportname1, std::string id1, int &viewportid1,
                              std::string viewportname2, std::string id2, int &viewportid2,
                              std::string viewportname3, std::string id3, int &viewportid3);
};


#endif // SIMPLEVIEWER_H
