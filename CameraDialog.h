#ifndef CAMERADIALOG_H
#define CAMERADIALOG_H
#pragma execution_character_set("utf-8")//解决中文乱码问题，并解决了mingw无法转换msvc2017的
#include <QDialog>
#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include "CameraThread.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/types_c.h"
#include <QImage>
#include <QTimer>


namespace Ui {
class CameraDialog;
}

class CameraDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CameraDialog(QWidget *parent = 0);
    ~CameraDialog();
    QImage img;
    QImage MatToQImage3(const cv::Mat &src);
    void ShowImage1(cv::Mat &src);
     QTimer *timer8;
      cv::Mat detect1_image;
protected:
    void paintEvent(QPaintEvent *event);

private slots:
    //播放视频槽函数
    void on_btn_Pause_clicked();
    //上下左右槽函数
    void on_btn_Up_clicked();

    void on_btn_Down_clicked();

    void on_btn_Left_clicked();

    void on_btn_Right_clicked();

    //增大焦距槽函数
    void on_btn_addFocal_clicked();
    //减小焦距槽函数
    void on_btn_decreaseFocal_clicked();
    //关闭窗口槽函数
    void on_btn_close_clicked();

    void Proview();

    void on_btn_addFocal_pressed();

    void on_btn_addFocal_released();

    void on_btn_decreaseFocal_pressed();

    void on_btn_decreaseFocal_released();

    void on_btn_Up_pressed();

    void on_btn_Up_released();

    void on_btn_Down_pressed();

    void on_btn_Down_released();

    void on_btn_Left_pressed();

    void on_btn_Left_released();

    void on_btn_Right_pressed();

    void on_btn_Right_released();

private:
    Ui::CameraDialog *ui;
signals:
    void camersigns();
};

#endif // CAMERADIALOG_H
