#include "CameraThread.h"
#include <QDebug>
#include <QMessageBox>
#include <QImage>
#include<QThread>
#include <vector>
#include"mylabel.h"
#include<QString>


HANDLE MainVectorhMutex;// 主互斥量
std::vector<cv::Mat> MainImageVector;//主视频图片容器
LONG nPort = -1;



//实时解码回调
bool CALLBACK YV12ToBGR24_FFmpeg(unsigned char* pYUV,unsigned char* pBGR24,int width,int height)
{
    if(width<1||height<1||pYUV==NULL||pBGR24==NULL)
    {
        return false;
    }
    //AVPictureType
    AVPicture pFrameYUV,pFrameBGR;
    //AVPictureType pFrameYUV,pFrameBGR;

    av_image_fill_arrays(pFrameYUV.data,pFrameYUV.linesize,pYUV,AV_PIX_FMT_YUV420P,width,height,1);

    uint8_t* ptemp=pFrameYUV.data[1];
    pFrameYUV.data[1]=pFrameYUV.data[2];
    pFrameYUV.data[2]=ptemp;
    av_image_fill_arrays(pFrameBGR.data,pFrameBGR.linesize,pBGR24,AV_PIX_FMT_BGR24,width,height,1);
    struct SwsContext *imgCtx=NULL;
    imgCtx=sws_getContext(width,height,AV_PIX_FMT_YUV420P,width,height,AV_PIX_FMT_BGR24,SWS_BILINEAR,0,0,0);
    if(imgCtx!=NULL){
        sws_scale(imgCtx,pFrameYUV.data,pFrameYUV.linesize,0,height,pFrameBGR.data,pFrameBGR.linesize);
        if(imgCtx){
            sws_freeContext(imgCtx);
            imgCtx=NULL;
        }
        return true;
    }else
    {
        sws_freeContext(imgCtx);
        imgCtx=NULL;
        return false;
    }
}
void CALLBACK DecCBFun(long nPort, char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1, long nReserved2)
{
    int num_devices=cuda::getCudaEnabledDeviceCount();
    if(num_devices<=0){
        std::cout<<"no cuda"<<std::endl;

    }
    cuda::setDevice(0);


    if (pFrameInfo->nType == T_YV12)	//YV12:视频格式  PCM：音频
    {
        cv::Mat src(pFrameInfo->nHeight + pFrameInfo->nHeight / 2, pFrameInfo->nWidth, CV_8UC1, pBuf);
       int sizeRGB=pFrameInfo->nWidth*pFrameInfo->nHeight*3;
       uchar *pBGR24=new unsigned char[sizeRGB];
       memset(pBGR24,0,sizeRGB);
       YV12ToBGR24_FFmpeg((uchar*)pBuf,pBGR24,1280,720);
       Mat BGR24_IMG(pFrameInfo->nHeight,pFrameInfo->nWidth,CV_8UC3,pBGR24);
//       imshow("bgr24",BGR24_IMG);
//       waitKey(1);



        // cv::cvtColor(src, src, COLOR_YUV2BGR_YV12);

         cv::resize(BGR24_IMG,BGR24_IMG,cv::Size(640,480));
        WaitForSingleObject(MainVectorhMutex,INFINITE);
        MainImageVector.push_back(BGR24_IMG);
        if(MainImageVector.size()>5){
            MainImageVector.erase(MainImageVector.begin());
        }
        ReleaseMutex(MainVectorhMutex);

        delete [] pBGR24;
    }

}

void CALLBACK fRealDataCallBack(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser)
{
    static LONG nPort = -1;														//实时预览通道号

    switch (dwDataType)
    {
    case NET_DVR_SYSHEAD: //头部数据
        if (!PlayM4_GetPort(&nPort))											//申请播放port，存为全局变量，供后面使用
        {
            break;
        }

        if (dwBufSize > 0)														//数据长度>0
        {
            if (!PlayM4_SetStreamOpenMode(nPort, STREAME_REALTIME))				//设置流处理模式  STREAME_REALTIME：尽力实时，不阻塞   STREAME_FILE:按照时间戳
            {
                break;
            }

//            if (!PlayM4_SetDecCBStream( nPort,1))	//设置流类型
//            {
//                break;
//            }


            if (!PlayM4_OpenStream(nPort, pBuffer, dwBufSize, 1024 * 1024))	//打开流接口，缓冲区设置为最大
            {
                break;
            }


            if (!PlayM4_SetDecCallBack(nPort, DecCBFun))						//设置自定义的解码回调函数 DecCBFun
            {
                break;
            }

            if (!PlayM4_Play(nPort, NULL))										//开始处理，不设置窗口句柄
            {
                break;
            }

        }
        break;

    case NET_DVR_STREAMDATA: //流数据
        if (dwBufSize > 0 && nPort != -1)
        {
            if (!PlayM4_InputData(nPort, pBuffer, dwBufSize))
            {
                cout << "error" << PlayM4_GetLastError(nPort) << endl;
                break;
            }
        }
        break;

    default:				//其他数据
        if (dwBufSize > 0 && nPort != -1)
        {
            if (!PlayM4_InputData(nPort, pBuffer, dwBufSize))
            {
                break;
            }
        }
        break;
    }
}


//异常触发
void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG userID, LONG lHandle, void *pUser)
{
    qDebug() << "exception callback," << userID << lHandle;
    switch (dwType)
    {
    case EXCEPTION_RECONNECT:										//预览时触发"重连"信号
        qDebug("设备重新连接，当前时间为：%d\n", (int)time(NULL));
        break;
    default:														//默认不作处理
        break;
    }
}

CameraThread::CameraThread(){


}
CameraThread::~CameraThread(){}
void CameraThread::run(){
     OnClickedLogin();

}

bool CameraThread::OnClickedLogin()
{	//char* p1 = "1234"; 强制转换
    char* p1 = (char*)"192.168.1.64";
    char* p3 = (char*)"sinoboom8";
    char* p2 = (char*)"admin";
    NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
       userID=NET_DVR_Login_V30(p1,8000,p2,p3,&DeviceInfoTmp);

       if(userID<0)
       {
           qDebug() << "NET_DVR_Login_V30 error;" << "error number is " << NET_DVR_GetLastError();
           QMessageBox::warning(0,"error","NET_DVR_Login_V30 error;error number "+QString::number(NET_DVR_GetLastError()));
           return 0;
       }
       qDebug()<<"Login Success,userID:" << userID<<endl;


       if(userID==-1)
       {
           qDebug()<<"Need Login!"<<endl;
                  QMessageBox::warning(0,"error","Need Login!");
                  return 0;
       }

           NET_DVR_PREVIEWINFO struPlayInfo;


       qDebug()<<"Preview Success!" << userID<<endl;




       NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);
       struPlayInfo={0};
       struPlayInfo.hPlayWnd=NULL;//需要 SDK 解码时句柄设为有效值，仅取流不解码时可设为空
       struPlayInfo.lChannel=1;//预览通道号
       struPlayInfo.dwLinkMode=0;//码流类型：0-主码流，1-子码流，2-三码流，3-虚拟码流，以此类推
       struPlayInfo.dwLinkMode=0;//0- TCP 方式，1- UDP 方式，2- 多播方式，3- RTP 方式，4-RTP/RTSP，5-RSTP/HTTP
       struPlayInfo.bBlocked=1;//0- 非阻塞取流，1- 阻塞取流
//       struPlayInfo.byProtoType=0;//0-私有协议，1-RTSP协议
//       struPlayInfo.dwDisplayBufNum=6;
       long IRealPlayHandle = NET_DVR_RealPlay_V40(userID,&struPlayInfo,fRealDataCallBack,NULL);//触发回调函数
   // long IRealPlayHandle = NET_DVR_RealPlay_V40(userID,&struPlayInfo,NULL,NULL);
       if(IRealPlayHandle <0)
           {
               qDebug() << "NET_DVR_RealPlay_V40 error;error number " <<NET_DVR_GetLastError();
               QMessageBox::warning(0,"error","NET_DVR_RealPlay_V40 error;error number "+QString::number(NET_DVR_GetLastError()));
               NET_DVR_Logout(userID);
                NET_DVR_Cleanup();
               userID=-1;

           }
//       if (!NET_DVR_SaveRealData(IRealPlayHandle, (char *)"G:/test.mp4")) {//取流保存
//           cout << "保存到文件失败 错误码:" ,  NET_DVR_GetLastError() ;
//          }

//           return IRealPlayHandle;

//       QMessageBox::information(NULL,"通知","摄像头已打开");
               return true;



}

bool CameraThread::initSDK()
{

      bool isok = NET_DVR_Init();
//    char sdklog[]="C:\\SdkLog\\"; //windows 默认值为"C:\\SdkLog\\"
//        NET_DVR_SetLogToFile(3, sdklog);//启用写日志文件

    if(isok == false){
        qDebug()<< "NET_DVR_Init error;error number is " <<NET_DVR_GetLastError();
              QMessageBox::warning(0,"error","NET_DVR_Init error;error number is "+QString::number(NET_DVR_GetLastError()));
              return isok;
    }
//设置连接时间与重连时间
        NET_DVR_SetConnectTime(2000, 1);
        NET_DVR_SetReconnect(10000, true);
        return isok;

}

bool CameraThread::uninitSDK()
{
 bool isok = NET_DVR_Cleanup();
 if(isok == false)
    {
        qDebug()<< "NET_DVR_Cleanup error;error number is " <<NET_DVR_GetLastError();
        QMessageBox::warning(0,"error","NET_DVR_Cleanup error;error number "+QString::number(NET_DVR_GetLastError()));
        return isok;
    }
    return isok;

}

void CameraThread::large_start()
{

   if (userID < 0)
   {
       printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
       NET_DVR_Cleanup();
       return;
   }

   int iChannel = 1;//设备通道号
   DWORD dwPTZCommand = ZOOM_IN; //云台焦距变大
   DWORD dwStop = 0; //开始转动
   if(!NET_DVR_PTZControl_Other(userID, iChannel, ZOOM_IN, dwStop))
   {
       printf("PAN_LEFT start failed, error code: %d\n", NET_DVR_GetLastError());
       NET_DVR_Logout(userID);
       NET_DVR_Cleanup();
       return;
   }}
void CameraThread::large_stop()
{






    if (userID < 0)
    {
        printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
        NET_DVR_Cleanup();
        return;
    }

    int iChannel = 1;//设备通道号
    DWORD dwPTZCommand = ZOOM_IN; //云台焦距变大
    DWORD dwStop = 0; //开始转动
//    if(!NET_DVR_PTZControl_Other(userID, iChannel, ZOOM_IN, dwStop))
//    {
//        printf("PAN_LEFT start failed, error code: %d\n", NET_DVR_GetLastError());
//        NET_DVR_Logout(userID);
//        NET_DVR_Cleanup();
//        return;
//    }

//    Sleep(50); //云台控制接口下发命令之后是云台持续运动的，持续转动5秒之后再调用接口下发停止转动命令

    dwStop = 1; //停止转动
    if(!NET_DVR_PTZControl_Other(userID, iChannel, ZOOM_IN, dwStop))
    {
        printf("PAN_LEFT stop failed, error code: %d\n", NET_DVR_GetLastError());
    }

   }
void CameraThread::smart_start()
{

       if (userID < 0)
       {
           printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
           NET_DVR_Cleanup();
           return;
       }

       int iChannel = 1;//设备通道号
       DWORD dwPTZCommand = ZOOM_OUT; //云台焦距变小
       DWORD dwStop = 0; //开始转动
       if(!NET_DVR_PTZControl_Other(userID, iChannel,ZOOM_OUT, dwStop))
       {
           printf("PAN_LEFT start failed, error code: %d\n", NET_DVR_GetLastError());
           NET_DVR_Logout(userID);
           NET_DVR_Cleanup();
           return;
       }
}
void CameraThread::smart_stop()
{

   if (userID < 0)
   {
       printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
       NET_DVR_Cleanup();
       return;
   }

   int iChannel = 1;//设备通道号
   DWORD dwPTZCommand = ZOOM_OUT; //云台焦距变小
   DWORD dwStop = 0; //开始转动
//    if(!NET_DVR_PTZControl_Other(userID, iChannel,ZOOM_OUT, dwStop))
//    {
//        printf("PAN_LEFT start failed, error code: %d\n", NET_DVR_GetLastError());
//        NET_DVR_Logout(userID);
//        NET_DVR_Cleanup();
//        return;
//    }
//    Sleep(50); //云台控制接口下发命令之后是云台持续运动的，持续转动5秒之后再调用接口下发停止转动命令

   dwStop = 1; //停止转动
   if(!NET_DVR_PTZControl_Other(userID, iChannel, ZOOM_OUT, dwStop))
   {
       printf("PAN_LEFT stop failed, error code: %d\n", NET_DVR_GetLastError());
   }

   }
void CameraThread::up_start()
{

   if (userID < 0)
   {
       printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
       NET_DVR_Cleanup();
       return;
   }

   int iChannel = 1;//设备通道号
   DWORD dwPTZCommand = TILT_UP; //云台上府
   DWORD dwStop = 0; //开始转动
   if(!NET_DVR_PTZControl_Other(userID, iChannel, TILT_UP, dwStop))
   {
       printf("PAN_LEFT start failed, error code: %d\n", NET_DVR_GetLastError());
       NET_DVR_Logout(userID);
       NET_DVR_Cleanup();
       return;
   }}
void CameraThread::up_stop()
{

   if (userID < 0)
   {
       printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
       NET_DVR_Cleanup();
       return;
   }

   int iChannel = 1;//设备通道号
   DWORD dwPTZCommand = TILT_UP; //云台上府
   DWORD dwStop = 0; //开始转动


   dwStop = 1; //停止转动
   if(!NET_DVR_PTZControl_Other(userID, iChannel, TILT_UP, dwStop))
   {
       printf("PAN_LEFT stop failed, error code: %d\n", NET_DVR_GetLastError());
   }

 }
void CameraThread::down_start()
{

   if (userID < 0)
   {
       printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
       NET_DVR_Cleanup();
       return;
   }

   int iChannel = 1;//设备通道号
   DWORD dwPTZCommand = TILT_DOWN; //云台左转
   DWORD dwStop = 0; //开始转动
   if(!NET_DVR_PTZControl_Other(userID, iChannel, TILT_DOWN, dwStop))
   {
       printf("PAN_LEFT start failed, error code: %d\n", NET_DVR_GetLastError());
       NET_DVR_Logout(userID);
       NET_DVR_Cleanup();
       return;
   }}
void CameraThread::down_stop()
{



   if (userID < 0)
   {
       printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
       NET_DVR_Cleanup();
       return;
   }

   int iChannel = 1;//设备通道号
   DWORD dwPTZCommand = TILT_DOWN; //云台左转
   DWORD dwStop = 0; //开始转动

   dwStop = 1; //停止转动
   if(!NET_DVR_PTZControl_Other(userID, iChannel, TILT_DOWN, dwStop))
   {
       printf("PAN_LEFT stop failed, error code: %d\n", NET_DVR_GetLastError());
   }


}
void CameraThread::left_start()
{

   if (userID < 0)
   {
       printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
       NET_DVR_Cleanup();
       return;
   }

   int iChannel = 1;//设备通道号
   DWORD dwPTZCommand = PAN_LEFT; //云台右转
   DWORD dwStop = 0; //开始转动
   if(!NET_DVR_PTZControl_Other(userID, iChannel, PAN_LEFT, dwStop))
   {
       printf("PAN_LEFT start failed, error code: %d\n", NET_DVR_GetLastError());
       NET_DVR_Logout(userID);
       NET_DVR_Cleanup();
       return;
   }};
void CameraThread::left_stop()
{



   if (userID < 0)
   {
       printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
       NET_DVR_Cleanup();
       return;
   }

   int iChannel = 1;//设备通道号
   DWORD dwPTZCommand = PAN_LEFT; //云台右转
   DWORD dwStop = 0; //开始转动


   dwStop = 1; //停止转动
   if(!NET_DVR_PTZControl_Other(userID, iChannel, PAN_LEFT, dwStop))
   {
       printf("PAN_LEFT stop failed, error code: %d\n", NET_DVR_GetLastError());
   }

}
void CameraThread::right_start()
{

   if (userID < 0)
   {
       printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
       NET_DVR_Cleanup();
       return;
   }

   int iChannel = 1;//设备通道号
   DWORD dwPTZCommand = PAN_RIGHT; //云台左转
   DWORD dwStop = 0; //开始转动
   if(!NET_DVR_PTZControl_Other(userID, iChannel, PAN_RIGHT, dwStop))
   {
       printf("PAN_LEFT start failed, error code: %d\n", NET_DVR_GetLastError());
       NET_DVR_Logout(userID);
       NET_DVR_Cleanup();
       return;
   }}
void CameraThread::right_stop()
{



   if (userID < 0)
   {
       printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
       NET_DVR_Cleanup();
       return;
   }

   int iChannel = 1;//设备通道号
   DWORD dwPTZCommand = PAN_RIGHT; //云台左转
   DWORD dwStop = 0; //开始转动


   dwStop = 1; //停止转动
   if(!NET_DVR_PTZControl_Other(userID, iChannel, PAN_RIGHT, dwStop))
   {
       printf("PAN_LEFT stop failed, error code: %d\n", NET_DVR_GetLastError());
   }


}

void CameraThread::close()
{

       NET_DVR_StopRealPlay(IRealPlayHandle);
    //播放库资源释放（先释放播放库资源，在进行用户注销以及SDK资源释放，否则将会导致视频数据阻塞）
       PlayM4_Stop(nPort);
       PlayM4_StopSound();
       PlayM4_CloseFile(nPort);
       PlayM4_FreePort(nPort);
       nPort=-1;
      //注销用户

      NET_DVR_Logout(userID);

      NET_DVR_Cleanup();

      userID=-1;

}



