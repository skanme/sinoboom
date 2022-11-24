#include "SerialPortSettingDialog.h"
#include "ui_SerialPortSettingDialog.h"

 QString str1;
SerialPortSettingDialog::SerialPortSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialPortSettingDialog)
{
    ui->setupUi(this);
    setWindowTitle("高空作业机器人");
    setWindowFlags (Qt::FramelessWindowHint);

    //串口相关设置
    mIsopen=false;//初始化打开变量
    ui->comboBox_5->addItem("9600");
    ui->comboBox_5->addItem("19200");
    ui->comboBox_5->addItem("115200");
    ui->comboBox_3->addItem("NONE");
    ui->comboBox_3->addItem("ODD");
    ui->comboBox->addItem("8");
    ui->comboBox->addItem("7");
    ui->comboBox->addItem("6");
    ui->comboBox->addItem("5");
    ui->comboBox_2->addItem("1");
    ui->comboBox_2->addItem("1.5");
     ui->comboBox_2->addItem("2");

     //访问本机串口
     QList<QSerialPortInfo>serialportinfo=QSerialPortInfo::availablePorts();
     int count =serialportinfo.count();
     for(int i=0;i<count;i++)
     {
         ui->comboBox_4->addItem(serialportinfo.at(i).portName());
     }
    ui->btnOpenSerial->setEnabled(true);
    ui->btnSend->setEnabled(false);
     timer4=new QTimer(this);



     connect(timer4,&QTimer::timeout,this,&SerialPortSettingDialog::on_ok);
     connect(&mSerialPort,&QSerialPort::readyRead,this,&SerialPortSettingDialog::on_js);
     connect(ui->btnOpenSerial,&QPushButton::clicked,this,&SerialPortSettingDialog::on_on);
}

SerialPortSettingDialog::~SerialPortSettingDialog()
{
    delete ui;
}

void SerialPortSettingDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(),QPixmap(":/icon/Dialog2/beijing.png"),QRect());
    QWidget::paintEvent(event);
}

void SerialPortSettingDialog::on_btnSend_clicked()
{

}

void SerialPortSettingDialog::on_btnTimerSend_clicked()
{

}

void SerialPortSettingDialog::on_btnOpenSerial_clicked()
{
//    on_on();
}

void SerialPortSettingDialog::on_btnStopSend_clicked()
{

}

void SerialPortSettingDialog::on_btnClose_clicked()
{
    this->close();
}


//获取当前五个参数的配置，串口，波特率
bool SerialPortSettingDialog::geserialportConfig(){
    box4= ui->comboBox_4->currentText();
    box5= ui->comboBox_5->currentText();
    box6= ui->comboBox_3->currentText();
    box7= ui->comboBox->currentText();
    box8= ui->comboBox_2->currentText();
    mSerialPort.setPortName(box4);
    if("9600"==box5)
    {
       mSerialPort.setBaudRate(QSerialPort::Baud9600);
    }else if("19200"==box5)
    {
        mSerialPort.setBaudRate(QSerialPort::Baud19200);
    }else
    {
         mSerialPort.setBaudRate(QSerialPort::Baud115200);
    }

    if("NONE" == box6)
    {
         mSerialPort.setParity(QSerialPort::OddParity);
    }
    else
    {
        mSerialPort.setParity(QSerialPort::EvenParity);
    }
    //设置数据位
       if("5"==box7)
       {
           mSerialPort.setDataBits(QSerialPort::Data5);
       }
       else if("6"==box7)
       {
           mSerialPort.setDataBits(QSerialPort::Data6);
       }
       else if("7"==box7)
       {
           mSerialPort.setDataBits(QSerialPort::Data7);
       }

       else
       {
           mSerialPort.setDataBits(QSerialPort::Data8);
       }
       //设置停止位
       if("1.5" == box8)
       {
           mSerialPort.setStopBits(QSerialPort::OneAndHalfStop);
       }
       else if("2" == box8)
       {
           mSerialPort.setStopBits(QSerialPort::TwoStop);
       }
       else
       {
           mSerialPort.setStopBits(QSerialPort::OneStop);
       }

       //返回打开按键的结果
       return mSerialPort.open(QSerialPort::ReadWrite);

}
//串口执行函数

void SerialPortSettingDialog::on_on(){
    //判断当前打开按钮是不是在true下
    if(true==mIsopen)
    {
        //当前串口助手已经打开一个串口

        mSerialPort.close();
        mIsopen = false;

        ui->btnSend->setEnabled(mIsopen);
        ui->comboBox->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        ui->comboBox_3->setEnabled(true);
        ui->comboBox_4->setEnabled(true);
        ui->comboBox_5->setEnabled(true);
        qDebug()<<"open port false";
         QMessageBox::information(NULL,"通知",box4+"已关闭");

    }
    else
    {
        //未打开串口，这时应执行打开串口动作
        if(true==geserialportConfig()){

            mIsopen=true;

            //提示：串口打开成功，端口号
            qDebug()<<"open port succeed"<<box4;

            //串口打开成功禁止操作别的按钮
            ui->comboBox->setEnabled(false);
            ui->comboBox_2->setEnabled(false);
            ui->comboBox_3->setEnabled(false);
            ui->comboBox_4->setEnabled(false);
            ui->comboBox_5->setEnabled(false);
//            qDebug()<<"1"<<mSerialPort.isOpen();
            QMessageBox::information(NULL,"通知",box4+"已打开");
        }
        else{
            mIsopen=false;

        }
    }

}

void SerialPortSettingDialog::on_ok(){
    QString STR="123";
  mSerialPort.write(STR.toStdString().c_str());
//  qDebug()<<"1"<<mSerialPort.isOpen();
}

void  SerialPortSettingDialog::on_js(){

 // qDebug()<<"1"<<mSerialPort.isOpen();判断串口是否打开的函数
    if(true==mIsopen){
        QByteArray recvDate= mSerialPort.readAll();
        str1 = QString(recvDate);
     }
   emit mainWidgetStr(str1);//获得值
}

void SerialPortSettingDialog::on_ss(QString str){


  mSerialPort.write(str.toStdString().c_str());

}
//窗口移动
void SerialPortSettingDialog::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){
        widget_move = true;
        //获得鼠标的初始位置与窗口的初始位置
        mousepoint = event->globalPos()-this->pos();
    }
    else if(event->button()==Qt::RightButton){
        this->close();
    }
}

void SerialPortSettingDialog::mouseMoveEvent(QMouseEvent *event){

   if(widget_move){
       //获取鼠标移动距离
      //改变窗口位置
       this->move(event->globalPos()-mousepoint);
   }

}

void SerialPortSettingDialog::mouseReleaseEvent(QMouseEvent *event){
    widget_move = false;
}
