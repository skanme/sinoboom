#include "sql.h"
#include <QDebug>

sql::sql()
{
    initDB();
    createTables();
}

void sql::initDB()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");//建立数据库链接
    m_db.setDatabaseName("D://SinoBoom.db");
    if(m_db.open())
        qDebug()<<"ok!!!!!!";
}

void sql::createTables()
{
    //用户表
    QSqlQuery query;
    QString str= QString(
                "CREATE TABLE USERINFO ("
                "UserId int NOT NULL PRIMARY KEY,"
                "UserPassWord text NOT NULL )");
    qDebug()<<query.exec(str);
    query.exec("insert into 'USERINFO' values(12345678,  '12345678')");     //默认账号、密码
}

bool sql::updatePassWord(int id, QString strPwd)
{
    QSqlQuery query;
    QString str = QString("update USERINFO set UserPassWord = '%1' where UserId = %2")
            .arg(strPwd)
            .arg(id);
    if(query.exec(str))
        return true;
    else
        return false;
}

bool sql::checkLogin(int id, QString strPwd)
{
    QString tempStr = "";
    QSqlQuery query;
    query.exec(QString("select UserPassWord FROM USERINFO WHERE UserId = %1 ").arg(id));
    while(query.next())
    {
        tempStr = query.value("UserPassWord").toString();
    }

    if(tempStr == strPwd)
        return true;
    else
        return false;
}
