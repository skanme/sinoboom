#ifndef COMMONSTRUCT_H
#define COMMONSTRUCT_H
#pragma execution_character_set("utf-8")//解决中文乱码问题，并解决了mingw无法转换msvc2017的
#include <QString>
//用户信息结构体
struct UserInfo
{
    QString userId;     //账号
    QString userPassWord;   //密码
};

#endif // COMMONSTRUCT_H
