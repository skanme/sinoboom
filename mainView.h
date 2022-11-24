#ifndef MAINVIEW_H
#define MAINVIEW_H
#pragma execution_character_set("utf-8")//解决中文乱码问题，并解决了mingw无法转换msvc2017的
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QEvent>
#include <QDateTime>
#include "include/HCNetSDK.h"
#include<opencv2/opencv.hpp>
#include<include/plaympeg4.h>
#include<QTimer>
#include<QImage>
#include"yolo.h"
#include<QPainter>
#include<QIODevice>
#include "source.h"
#include <QString>
#include "mylabel.h"
//子界面文件
#include "CameraDialog.h"
#include "SerialPortSettingDialog.h"
//串口头文件
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
using namespace std;
using namespace cv;
using namespace dnn;


namespace Ui {
class mainView;
}

class mainView : public QWidget
{
    Q_OBJECT

public:
    explicit mainView(QWidget *parent = 0);
    ~mainView();
    void initUI();
    void setWidgetBlueStyle();         //下方是蓝色按钮的时候 界面的样式
    void setWidgetRedStyle();         //下方是红色按钮的时候 界面的样式
    QImage image1;
    cv::Mat detect_image;
    QImage img;

    QImage MatToQImage2(const cv::Mat &src);
     void ShowImage(cv::Mat &src);
     void Get_Frame();
     void Get_chick_Frame();

signals:



protected:

    void paintEvent(QPaintEvent *event);

private slots:
    //打开摄像头槽函数
    void on_btnOpenCamera_clicked();
    //暂停播放槽函数
    void on_btn_PauseVedio_clicked();
    //相机检测槽函数
    void on_btn_CarmeraCheck_clicked();
    //停止检测槽函数
    void on_btnStopCheck_clicked();
    //关闭摄像头槽函数
    void on_btn_CloseCamera_clicked();
    //串口设置槽函数
    void on_btn_SerialPortSetting_clicked();
    //相机设置槽函数
    void on_btn_CameraSetting_clicked();
    //清洗检测（切换界面）
    void on_btnWashingCheck_clicked();
    //返回至上一界面
    void on_btn_CloseTaskCheck_2_clicked();
    //从串口子界面得到数据函数
    void setStr(QString str);
    //刷新状态栏
    void statuviwe();
private:
    Ui::mainView *ui;
    CameraDialog m_cameraDlg;
    SerialPortSettingDialog m_serialPortSettingDlg;//注意子界面声明

    /****定时器定义****/
    QTimer *timer;//显示定时
    QTimer *timer_chick;
    QTimer *timer_viwe;//显示返回数据
    QTimer *timer_statu;//刷线状态栏定时器
    //检测判断
    bool  chick=false;
    bool  open1=true;
    //深度模型变量定义
     string model_path = "D://QT_SinoBoom(1)//best1026.onnx";
     Yolo test;
     Net net;
     /*********************/
     bool mouseIsdown;
     QString strcx,strcy,str1,str2,str3,str4,str5,str6,str7,strConv,strConv1,strConv2,strConv3,strConv4,strConv5,strConv6,
     strConv7,strConv8,strConv9,all,str;
};

#endif // MAINVIEW_H
