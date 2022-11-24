#ifndef SQL_H
#define SQL_H

#include "commonstruct.h"
#include <QSqlDatabase>
#include <QSqlQuery>

/**
 * @brief 封装数据库
 * @author Zwj
 */

class sql
{
public:
    sql();
    void initDB();
    void createTables();
    //修改密码Api
    bool updatePassWord(int id, QString strPwd);
    //登录校验Api
    bool checkLogin(int id, QString strPwd);




public:
    QSqlDatabase m_db;

};

#endif // SQL_H
