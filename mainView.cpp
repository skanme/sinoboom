#include "mainView.h"
#include "ui_mainview.h"
#include "CameraThread.h"
#include "yolo.h"
#include "mylabel.h"
#include "source.h"

extern HANDLE MainVectorhMutex;// 主互斥量
extern std::vector<cv::Mat> MainImageVector;//主视频图片容器
extern int cx,cy;
extern int start_x,start_y,end_x,end_y;
extern bool judge;
extern QString str1;
CameraThread Ca;
mainView::mainView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainView)
{
    ui->setupUi(this);

     mouseIsdown=false;
     ui->label_play->installEventFilter(this);
     this->setMouseTracking(true);
    initUI();
    timer=new QTimer();
    timer_chick =new QTimer();
    timer_viwe =new QTimer();
    timer_statu =new QTimer();

    connect(timer,&QTimer::timeout,this,&mainView::Get_Frame);
    connect (timer_chick,&QTimer::timeout,this,&mainView::Get_chick_Frame);
    connect(&m_serialPortSettingDlg,&SerialPortSettingDialog::mainWidgetStr,this,[=](QString temp){        //利用lamda函数将子界面的值传递到主界面函数中，以及子界面在主界面中如何使用的槽函数
                                setStr(temp);
 });
     connect(&m_cameraDlg,&CameraDialog::camersigns,this,&mainView::on_btn_CloseCamera_clicked);
     connect(timer_statu,&QTimer::timeout,this,&mainView::statuviwe);
     ui->label_statu->hide();

bool C2=Ca.initSDK();
if(C2)
      qDebug()<<"SDK init Success!"<<endl;
    }
mainView::~mainView()
{
    delete ui;
}

void mainView::initUI()
{
    setWindowTitle("高空作业机器人");
    ui->stackedWidget->setCurrentIndex(0);
    setWidgetBlueStyle();
    //    ui->widget_Vedio->setStyleSheet("#widget_Vedio{background-color:#181A1C;border:6px solid #650a0c;border-radius:10px;}"
    //                                    "#btn_PauseVedio{background-color: rgba(0,0,0,0%);border:none;}");
   // ui->btn_CloseTaskCheck_2->setStyleSheet("hover{border-radius:10pix;background-image:url(:qrc:/icon/mainView/anniu_press.png)}");
   // this->setStyleSheet(QString::fromUtf8("QPushButton#btn_CloseTaskCheck_2{border-radius:10pix}"));
}

void mainView::setWidgetBlueStyle()
{
    ui->widget_Vedio->setStyleSheet("#widget_Vedio{background-color:#181A1C;border:6px solid #1C8FBF;border-radius:10px;}"
                                    "#btn_PauseVedio{background-color: rgba(0,0,0,0%);border:none;}");
     ui->label->setText("坐标信息");
    ui->label_Abnormal->hide();
    ui->label_Normal->hide();
    ui->label_play_1->hide();
}

void mainView::setWidgetRedStyle()
{
    ui->widget_Vedio->setStyleSheet("#widget_Vedio{background-color:#181A1C;border:6px solid #650a0c;border-radius:10px;}"
                                    "#btn_PauseVedio{background-color: rgba(0,0,0,0%);border:none;}");
    ui->label->setText("清洗情况反馈");
    ui->label_Abnormal->show();
    ui->label_Normal->show();
    ui->label_play_1->show();
}

void mainView::paintEvent(QPaintEvent *event)
{
    //    QPainter painter(this);
    //    painter.drawPixmap(rect(),QPixmap(":/icon/beijing.png"),QRect());
    //    QWidget::paintEvent(event);
}


void mainView::on_btnOpenCamera_clicked()
{
    Ca.initSDK();
    Ca.OnClickedLogin();
    open1=true;
    timer->start(33);
}
void mainView::on_btn_PauseVedio_clicked()
{

}
void mainView::on_btn_CarmeraCheck_clicked()
{
     chick=true;
     timer_viwe->start(33);
     //QMessageBox::information(NULL,"提示","已打开摄像头检测");
     timer_statu->start(100);
     ui->label_statu->show();
}

void mainView::on_btnStopCheck_clicked()
{
    chick=false;
    QMessageBox::information(NULL,"提示","已关闭摄像头检测");
    timer_chick->stop();
    timer_statu->stop();
    ui->label_statu->hide();
}

void mainView::on_btn_CloseCamera_clicked()
{
      open1=false;
      timer->stop();
      timer_chick->stop();
      timer_viwe->stop();
      timer_statu->stop();
      ui->label_statu->hide();
     // NET_DVR_Cleanup();
     // MainImageVector.clear();
      //vector<cv::Mat>().swap(MainImageVector);
      Ca.close();
      ui->label_play->clear();
      //QMessageBox::information(NULL,"提示","摄像头已关闭");

}

void mainView::on_btn_SerialPortSetting_clicked()
{
    m_serialPortSettingDlg.show();
}

void mainView::on_btn_CameraSetting_clicked()
{
    m_cameraDlg.show();
    open1=false;
    timer->stop();
    timer_chick->stop();
    timer_viwe->stop();
    timer_statu->stop();
    ui->label_statu->hide();
   // NET_DVR_Cleanup();
   // MainImageVector.clear();
    //vector<cv::Mat>().swap(MainImageVector);
    Ca.close();
    ui->label_play->clear();
    //QMessageBox::information(NULL,"提示","摄像头已关闭");
}

void mainView::on_btnWashingCheck_clicked()
{
    setWidgetRedStyle();
    ui->stackedWidget->setCurrentIndex(1);
    ui->label_play_1->show();
    ui->label_play->hide();
}


//返回至检测界面
void mainView::on_btn_CloseTaskCheck_2_clicked()
{
   setWidgetBlueStyle();
   ui->stackedWidget->setCurrentIndex(0);
   ui->label_play->show();
}

//相机显示功能

void mainView::ShowImage(cv::Mat &src){

//    QImage image=MatToQImage2(src);
//    QPixmap pixmap = QPixmap::fromImage(image);
//    int width = ui->label_play->width();
//    int height = ui->label_play->height();
//    QPixmap fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//    ui->label_play->setPixmap(pixmap);
//    ui->label_play->resize(ui->label_play->pixmap()->size());//label自适应图片

}

//图片转换功能io

QImage mainView::MatToQImage2(const cv::Mat &src){

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


//显示
void mainView::Get_Frame(){

    test.readModel(net,model_path,true);
    vector<Scalar>color;
     for (int i = 0; i < 80; i++)
     {
         int b = rand() % 256;
         int g = rand() % 256;
         int r = rand() % 256;
         color.push_back(Scalar(b, g, r));
     }

     while(open1){
         if(judge){
     //         qDebug()<<"ss"<<endl;
//                                       timer5->start(33);
         }
         else{

//                                      timer5->stop();
         }
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
                 cv::Mat detect_image = getimg.clone();
                 cv::resize(detect_image,detect_image,Size(1120,620));
         if(chick){
                  vector<Output> result;
                  if (test.Detect(detect_image, net, result)) {
                      test.drawPred(detect_image, result, color);
                      QString strcx=QString::number(cx);
                      QString strConv = QString("%1").arg(strcx.toInt(), 3, 10, QLatin1Char('0'));
                      QString strcy=QString::number(cy);
                      QString strConv1 = QString("%1").arg(strcy.toInt(), 3, 10, QLatin1Char('0'));
                      QString str=QString::number(start_x);
                      QString strConv2 = QString("%1").arg(str.toInt(), 3, 10, QLatin1Char('0'));
                      QString str1=QString::number(start_y);
                      QString strConv3 = QString("%1").arg(str1.toInt(), 3, 10, QLatin1Char('0'));
                      QString str2=QString::number(end_x);
                      QString strConv4 = QString("%1").arg(str2.toInt(), 3, 10, QLatin1Char('0'));
                      QString str3=QString::number(end_y);
                      QString strConv5 = QString("%1").arg(str3.toInt(), 3, 10, QLatin1Char('0'));
                      QString all=strConv+strConv1+strConv2+strConv3+strConv4+strConv5;
                      m_serialPortSettingDlg.on_ss(all);
                  }

         }

                  cv::cvtColor(detect_image,detect_image,COLOR_BGR2RGB);
                  img = QImage((const unsigned char*)(detect_image.data), detect_image.cols, detect_image.rows, detect_image.step, QImage::Format_RGB888);
                  img.scaled(ui->label_play->size(),Qt::KeepAspectRatio);

                  ui->label_play->setScaledContents(true);
                  ui->label_play->setPixmap(QPixmap::fromImage(img));

//                ui->label_play->resize(ui->label_play->pixmap()->size());//label自适应图片
                  if(waitKey(2)==27)break;
     } 
          qDebug()<<"ss"<<endl;
          ReleaseMutex(MainVectorhMutex);
          detect_image.release();




}

void mainView::Get_chick_Frame(){

    timer->stop();

}

void mainView::setStr(QString str){


    ui->textEdit_feedback->insertPlainText(str);

}

void mainView::statuviwe(){

    ui->label_statu->setStyleSheet(
                "border:3px solid #1C8FBF;"
            "border-radius:8px;"
             "color:#FFFFF0"   );
    QString str= "start_x= "+QString::number(start_x)+"  start_y= "+QString::number(start_y)+"  end_x= "+QString::number(end_x)+"  end_y= "+QString::number(end_y);
    ui->label_statu->setText(str);
}

