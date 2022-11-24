#ifndef SERIALPORTSETTINGDIALOG_H
#define SERIALPORTSETTINGDIALOG_H
#pragma execution_character_set("utf-8")//解决中文乱码问题，并解决了mingw无法转换msvc2017的

#include <QDialog>
#include <QPainter>
#include <QPaintEvent>
#include<QSerialPort>
#include<QSerialPortInfo>
#include<QTimer>
#include<QDebug>
#include<QIODevice>
#include <QMessageBox>
#include <QMouseEvent>
#include <QString>
extern QString str1;

namespace Ui {
class SerialPortSettingDialog;
}

class SerialPortSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerialPortSettingDialog(QWidget *parent = 0);
    ~SerialPortSettingDialog();

     bool geserialportConfig(void);//获取当前设置的五个参数（串口号，波特率。。。。。）
     //串口执行函数
     void on_on();//打开返回值，获取当前打开按钮是不是打开状态，若打开禁止设置五个参数
     void on_ok();//发送返回值，判断发送按钮是否可以发送
     void on_js();//接收返回值，显示接收文本
     void on_ss(QString);//串口通信，跨界面调取参数，使用
public slots:


protected:
    void paintEvent(QPaintEvent *event);
    //拖拽窗口
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    //发送槽函数
    void on_btnSend_clicked();
    //定时发送
    void on_btnTimerSend_clicked();
    //打开/关闭串口槽函数
    void on_btnOpenSerial_clicked();
    //停止发送槽函数
    void on_btnStopSend_clicked();
    //关闭窗口槽函数
    void on_btnClose_clicked();



private:
    Ui::SerialPortSettingDialog *ui;

    //设置串口成员**重点
     QSerialPort mSerialPort;
     QByteArray sendArray;
    //增加一个是否打开变量**重点
     bool mIsopen;
     QString box4;  //串口
     QString box5;  //波特率
     QString box6;  //校验位
     QString box7;  //数据位
     QString box8;  //停止位
    //定时器
      QTimer *timer4;

      QPoint mousepoint;
      bool widget_move;

signals:
     void mainWidgetStr(QString);//槽函数中发送者的信号，并将该函数获得的值传递到主界面中
};

#endif // SERIALPORTSETTINGDIALOG_H
