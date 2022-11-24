#include "LoginWidget.h"
#include "ui_loginwidget.h"
#pragma execution_character_set("utf-8")//解决中文乱码问题，并解决了mingw无法转换msvc2017的
LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    initUI();
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::initUI()
{
    setWindowTitle("高空作业机器人");
    ui->label_7->hide();
    ui->stackedWidget->setCurrentIndex(0);

}

void LoginWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(),QPixmap(":/icon/beijing.png"),QRect());
    QWidget::paintEvent(event);
}


void LoginWidget::on_btn_forgetPwd_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    QString str = "   ";
    str.append(ui->lineEdit_inputId->text());
    qDebug()<<str;
    ui->lineEdit_id->setText(str);
    ui->lineEdit_id->setEnabled(false);
}

void LoginWidget::on_btn_Login_clicked()
{
    if(ui->lineEdit_inputId->text().isEmpty() || ui->lineEdit_inputPwd->text().isEmpty())
    {
        QMessageBox::information(this, "提示", "账号、密码不不能为空");
        return;
    }
    bool ret = m_sql.checkLogin(ui->lineEdit_inputId->text().toInt(), ui->lineEdit_inputPwd->text());
    if(ret)
    {
        this->hide();
        m_mainView.show();
    }
    else
    {
        ui->label_7->show();
    }
//    ui->lineEdit_inputPwd->clear();
}

void LoginWidget::on_btnConfirmUpdate_clicked()
{
    if(ui->lineEdit_NewPwd->text().isEmpty())
    {
        QMessageBox::information(this, "提示", "新密码不能为空");
        return;
    }
    else
    {
        if(ui->lineEdit_NewPwd->text().size() != 8)
        {
            QMessageBox::information(this, "提示", "8位数字或字母");
            return;
        }
        else
        {
            bool ret = m_sql.updatePassWord(ui->lineEdit_id->text().toInt(), ui->lineEdit_NewPwd->text());
            if(ret)
            {
                ui->stackedWidget->setCurrentIndex(0);
            }
            else
            {
                QMessageBox::information(this, "提示", "修改失败，请联系开发人员");
                return;
            }
        }
    }

}
//实现回车键登录
void LoginWidget::keyPressEvent(QKeyEvent *ev)
{
    switch(ev->key())
    {
    case Qt::Key_Return:
        on_btn_Login_clicked();
        break;
    case Qt::Key_Enter:
        on_btn_Login_clicked();
        break;
    default:
        break;
    }
}
