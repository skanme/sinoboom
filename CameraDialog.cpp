#include "CameraDialog.h"
#include "ui_CameraDialog.h"
#include "CameraThread.h"

extern HANDLE MainVectorhMutex;// 主互斥量
extern std::vector<cv::Mat> MainImageVector;//主视频图片容器
CameraThread cc;


CameraDialog::CameraDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraDialog)
{
    ui->setupUi(this);
    setWindowTitle("高空作业机器人");
    setWindowFlags (Qt::FramelessWindowHint);
    timer8=new QTimer;
    cc.initSDK();
    connect(timer8,&QTimer::timeout,this,&CameraDialog::Proview);
}

CameraDialog::~CameraDialog()
{
    timer8->stop();
    //  cc.uninitSDK();
     cc.close();
    delete ui;
}

void CameraDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(),QPixmap(":/icon/Dialog1/biangkuang2.png"),QRect());
    QWidget::paintEvent(event);
}

void CameraDialog::on_btn_Pause_clicked()
{   camersigns();
    cc.initSDK();
    cc.OnClickedLogin();
    timer8->start(33);
    ui->btn_Pause->hide();
}

void CameraDialog::on_btn_Up_clicked()
{

}

void CameraDialog::on_btn_Down_clicked()
{

}


void CameraDialog::on_btn_Left_clicked()
{

}

void CameraDialog::on_btn_Right_clicked()
{

}

void CameraDialog::on_btn_addFocal_clicked()
{

}

void CameraDialog::on_btn_decreaseFocal_clicked()
{

}

void CameraDialog::on_btn_close_clicked()
{ 
    this->close();
    timer8->stop();
    cc.close();
    ui->label_viwe->clear();

    ui->btn_Pause->show();
}

void CameraDialog::ShowImage1(cv::Mat &src)
{

    QImage image=MatToQImage3(src);
    QPixmap pixmap = QPixmap::fromImage(image);
    int width = ui->label_viwe->width();
    int height = ui->label_viwe->height();
    QPixmap fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap(pixmap);
    ui->label->resize(ui->label_viwe->pixmap()->size());//label自适应图片
}

QImage CameraDialog::MatToQImage3(const cv::Mat &src){
    //CV_8UC1 8位无符号的单通道---灰度图片
       if(src.type() == CV_8UC1)
       {
           //使用给定的大小和格式构造图像
           //QImage(int width, int height, Format format)
           QImage qImage(src.cols,src.rows,QImage::Format_Indexed8);
           //扩展颜色表的颜色数目
           qImage.setColorCount(256);

           //在给定的索引设置颜色
           for(int i = 0; i < 256; i ++)
           {
               //得到一个黑白图
               qImage.setColor(i,qRgb(i,i,i));
           }
           //复制输入图像,data数据段的首地址
           uchar *pSrc = src.data;
           //
           for(int row = 0; row < src.rows; row ++)
           {
               //遍历像素指针
               uchar *pDest = qImage.scanLine(row);
               //从源src所指的内存地址的起始位置开始拷贝n个
               //字节到目标dest所指的内存地址的起始位置中
               memcmp(pDest,pSrc,src.cols);
               //图像层像素地址
               pSrc += src.step;
           }
            qDebug() <<"！！"<<endl;
           return qImage;
       }
       //为3通道的彩色图片
       else if(src.type() == CV_8UC3)
       {
           //得到图像的的首地址
           const uchar *pSrc = (const uchar*)src.data;
           //以src构造图片
           QImage qImage(pSrc,src.cols,src.rows,src.step,QImage::Format_RGB888);
           //在不改变实际图像数据的条件下，交换红蓝通道
           return qImage.rgbSwapped();
       }
       //四通道图片，带Alpha通道的RGB彩色图像
       else if(src.type() == CV_8UC4)
       {
           const uchar *pSrc = (const uchar*)src.data;
           QImage qImage(pSrc, src.cols, src.rows, src.step, QImage::Format_ARGB32);
           //返回图像的子区域作为一个新图像
           return qImage.copy();
       }
       else
       {
           return QImage();
       }


}
void CameraDialog::Proview(){


    while (MainImageVector.size() <= 0)
      {
           Sleep(20);
           continue;
       }
//       获取互斥量
       WaitForSingleObject(MainVectorhMutex, INFINITE);
       // 获取头一帧
      cv::Mat getimg;
      if(MainImageVector.size()>0){
           getimg = MainImageVector.front();
           //getimg = *(MainImageVector.begin());
//           }
   }
   else
           {
                  return ;
             }
      // 删除头一帧
            MainImageVector.erase(MainImageVector.begin());
            cv::Mat detect1_image = getimg.clone();
            cv::resize(detect1_image,detect1_image,Size(809,451));
            cv::cvtColor(detect1_image,detect1_image,COLOR_BGR2RGB);
            img = QImage((const unsigned char*)(detect1_image.data), detect1_image.cols, detect1_image.rows, detect1_image.step, QImage::Format_RGB888);
            ui->label_viwe->setPixmap(QPixmap::fromImage(img));
            //ui->label_viwe->resize(ui->label_viwe->pixmap()->size());//label自适应图片
            ReleaseMutex(MainVectorhMutex);
            detect1_image.release();
}
void CameraDialog::on_btn_addFocal_pressed()
{
    cc.large_start();
}


void CameraDialog::on_btn_addFocal_released()
{
    cc.large_stop();
}


void CameraDialog::on_btn_decreaseFocal_pressed()
{
    cc.smart_start();
}


void CameraDialog::on_btn_decreaseFocal_released()
{
    cc.smart_stop();
}


void CameraDialog::on_btn_Up_pressed()
{
    cc.up_start();
}


void CameraDialog::on_btn_Up_released()
{
    cc.up_stop();
}


void CameraDialog::on_btn_Down_pressed()
{
    cc.down_start();
}


void CameraDialog::on_btn_Down_released()
{
    cc.down_stop();
}


void CameraDialog::on_btn_Left_pressed()
{
    cc.left_start();
}


void CameraDialog::on_btn_Left_released()
{
    cc.left_stop();
}


void CameraDialog::on_btn_Right_pressed()
{
    cc.right_start();
}


void CameraDialog::on_btn_Right_released()
{
    cc.right_stop();
}

