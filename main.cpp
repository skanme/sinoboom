#include "LoginWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWidget loginView;
    loginView.resize(1920,1080);
    loginView.show();
    /**************弹出软键盘**************/
    PVOID OldValue;
    BOOL bRet=Wow64DisableWow64FsRedirection(&OldValue);
    QString csProcess="C:\\Windows\\System32\\osk.exe";
    QString params="";
    ShellExecute(NULL,L"open",(LPCWSTR)csProcess.utf16(),(LPCWSTR)params.utf16(),NULL,SW_SHOWNORMAL);
    if(bRet){
        Wow64RevertWow64FsRedirection(OldValue);
    }
    /************************************/
    return a.exec();
}
