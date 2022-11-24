#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H
#pragma execution_character_set("utf-8")//解决中文乱码问题，并解决了mingw无法转换msvc2017的
#include <QObject>
#include <QDebug>
#include <QImage>
#include <QString>
#include <QThread>
#include <QTime>
#include <QApplication>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/types_c.h"
#include "include/HCNetSDK.h"
#include<include/plaympeg4.h>
#include <QMessageBox>
#include "iostream"
#include <stdio.h>
#include <Windows.h>
extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
#include "libavutil/pixfmt.h"
#include "libavutil/imgutils.h"
}
#pragma warning(disable:4996);
using namespace std;
using namespace cv;
//using namespace cv::cuda;
namespace GPU =cv::cuda ;
typedef struct AVPicture{
    uint8_t *data[AV_NUM_DATA_POINTERS];
    int linesize[AV_NUM_DATA_POINTERS];
} AVPicture ;
class CameraThread:public QThread
{

public:
    bool OnClickedLogin();
    CameraThread();
    ~CameraThread();
    //发起线程退出操作，调用quit()
    void quit();
    //线程运行的主体
    void run();
    void large_start();
    void large_stop();
    void smart_start();
    void smart_stop();
    void up_start();
    void up_stop();
    void down_start();
    void down_stop();
    void left_start();
    void left_stop();
    void right_start();
    void right_stop();
    bool initSDK();
    bool uninitSDK();
    void close();
    bool YV12ToBGR24_FFmpeg(unsigned char* pYUV,unsigned char* pBGR24,int width,int height);


protected:

private:

      long userID;
      long previewID;
      LONG lUserID;
      long IRealPlayHandle;
     //struct SwsContext*img_ctx=nullptr;
signals:


public slots:


};

#endif // CAMERATHREAD_H


