#include "mylabel.h"
//#include"ui_mainView.h"
#include "mainView.h"
#include "source.h"
extern bool judge;
 int start_x=0,start_y=0,end_x=0,end_y=0;
bool judge=false;


MyLabel::MyLabel(QWidget *parent):QLabel(parent)
{
    //注册事件过滤
    installEventFilter(this);

}
MyLabel::MyLabel(MainWindow *parent)
{
    this->mainwindow=parent;

}
MyLabel::~MyLabel()
{

}
void MyLabel::paintEvent(QPaintEvent *event)
{

    QLabel::paintEvent(event);//先调用父类的paintEvent为了显示'背景'!!!
    QPainter painter(this);
    painter.setPen(QPen(Qt::green, 2));
    if(clean){
            painter.setPen(QPen(Qt::black, 1));
               painter.drawLine(0,0,1,1);
               start_x=0,start_y=0,end_x=0,end_y=0;

             qDebug()<< start_x<<","<<start_y;
        }
        else{
            if (tempStartX + width < 1120 && tempStartY + height <640  && tempStartX >= 0 && tempStartY >= 0)
        {
            painter.drawRect(QRect(tempStartX, tempStartY, width, height));
        }
          }

}

bool MyLabel::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {

            QMouseEvent *mouseEvent = (QMouseEvent *)event;

            if (mouseEvent->buttons() & Qt::LeftButton)
            {
                startPos = mouseEvent->pos();
                start_x=startPos.x();
                start_y=startPos.y();               
                qDebug()<< start_x<<","<<start_y;
                 clean=false;
            }
            // QString sendmsg=QString::number(startPos.x())+QString::number(startPos.y());


        }
        if (event->type() == QEvent::MouseMove)
        {
            QMouseEvent *mouseEvent = (QMouseEvent *)event;
            endPos = mouseEvent->pos();
            this->update();
        }

        if (event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent *mouseEvent = (QMouseEvent *)event;
            endPos = mouseEvent->pos();
            end_x=endPos.x();
            end_y=endPos.y();

             judge = true;
            qDebug()<<end_x<<","<<end_y;
            if(end_x<85||end_x>1035||start_x<85||start_x>1035||start_y<85||start_y>535||end_y<85||end_y>535){

                QMessageBox::information(NULL,"提示","机器人超出显示区域",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            }

            QMessageBox:: StandardButton   btn = QMessageBox::information(NULL,"通知","请确认清洗区域",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                       if(btn==QMessageBox::No){
                           clean=true;
                           MyLabel::repaint();
                          qDebug()<< "start_x";


                            }
                       else{

                            }
        }
        if (event->type() == QEvent::Paint)
        {
            //qDebug() << "endP = " << endPos << endl;

            QPainter painter(this);
            QPen pen = painter.pen();
            pen.setWidth(3);
            pen.setColor(QColor(0, 255, 0));
            //反走样
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(pen);

            // 绘制直线
            width = abs(endPos.x() - startPos.x());
            height = abs(endPos.y() - startPos.y());

            //任意方向矩形  QRect默认左上到右下
            if ((endPos.y() - startPos.y()) < 0 && (endPos.x() - startPos.x()) > 0)
            {
                tempStartX = startPos.x();
                tempStartY = endPos.y();
            }
            else if ((endPos.y() - startPos.y()) > 0 && (endPos.x() - startPos.x()) < 0)
            {
                tempStartX = endPos.x();
                tempStartY = startPos.y();
            }
            else if ((endPos.y() - startPos.y()) < 0 && (endPos.x() - startPos.x()) < 0)
            {
                tempStartX = endPos.x();
                tempStartY = endPos.y();
            }
            else
            {
                tempStartX = startPos.x();
                tempStartY = startPos.y();
            }

        }



    }

    return QWidget::eventFilter(watched, event);
}
