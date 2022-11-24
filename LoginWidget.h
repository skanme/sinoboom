#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>
#include <QDebug>
#include "sql.h"
#include "mainView.h"
#include<Windows.h>
#pragma comment(lib,"user32.lib")


namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();
    void initUI();

protected:
    void paintEvent(QPaintEvent *event);
    virtual void keyPressEvent(QKeyEvent *ev);

private slots:
    void on_btn_forgetPwd_clicked();    //忘记密码

    void on_btn_Login_clicked();    //登录

    void on_btnConfirmUpdate_clicked();     //确认修改
   // bool eventFilter(QObject *,QEvent *);
    //void keyPressEvent(QKeyEvent *event);
private:
    Ui::LoginWidget *ui;
    sql m_sql;
    mainView m_mainView;
    QWidget *window=nullptr;
};

#endif // LOGINWIDGET_H
