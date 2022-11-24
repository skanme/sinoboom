#ifndef MYLABEL_H
#define MYLABEL_H
extern int start_x,start_y,end_x,end_y;
#include <QLabel>
#include <QRect>
#include <QPainter>
#include <QMouseEvent>
#include<QDebug>
#include<QString>
#include "source.h"
class MainWindow;
class MyLabel : public QLabel
{
    Q_OBJECT
public:
    MyLabel(QWidget* parent = 0);
    MyLabel(MainWindow * parent);
    MainWindow *mainwindow;
    ~MyLabel();
public:
    bool clean=false;
    int tempStartX, tempStartY, tempEndX, tempEndY, width, height;
    QPoint startPos, endPos;
protected:
     void paintEvent(QPaintEvent *event);
     bool eventFilter(QObject *watched, QEvent *event);
};

#endif // MYLABEL_H
