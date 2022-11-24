                                                                                                                                                                                                                                                                                         #-------------------------------------------------
#
# Project created by QtCreator 2022-10-27T21:07:51
#
#-------------------------------------------------

QT       += core gui sql
QT       +=serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QT_SinoBoom
TEMPLATE = app


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
        -L. $$PWD\include

# "-L. $$PWD" 表示pro所在目录
LIBS += \
        -L. $$PWD/libs/HCCore.lib \
        -L. $$PWD/libs/HCNetSDK.lib \
        -L. $$PWD/libs/PlayCtrl.lib \
        -L. $$PWD/libs/HCNetSDKCom\HCAlarm.lib \
        -L. $$PWD/libs/HCNetSDKCom\HCGeneralCfgMgr.lib \
        -L. $$PWD/libs/HCNetSDKCom\HCPreview.lib

SOURCES += \
    CameraThread.cpp \
        main.cpp \
        LoginWidget.cpp \
    mainView.cpp \
    mylabel.cpp \
    source.cpp \
    sql.cpp \
    CameraDialog.cpp \
    SerialPortSettingDialog.cpp \
    yolo.cpp

HEADERS += \
    CameraThread.h \
        LoginWidget.h \
    mainView.h \
    mylabel.h \
    source.h \
    sql.h \
    commonstruct.h \
    CameraDialog.h \
    SerialPortSettingDialog.h \
    yolo.h

FORMS += \
        loginwidget.ui \
    mainview.ui \
    CameraDialog.ui \
    SerialPortSettingDialog.ui

RESOURCES += \
    icon1/icon1.qrc \
    res.qrc

INCLUDEPATH +=D:\software\opencv4.5.1\build\install\include\
              D:\software\opencv4.5.1\build\install\include\opencv2
#LIBS +=D:\opencv1\opencv\build\x64\vc15\lib\opencv_world455.lib
#LIBS +=D:\opencv1\opencv\build\x64\vc15\lib\opencv_world455d.lib
LIBS +=D:\software\opencv4.5.1\build\install\x64\vc16\lib\opencv_world451d.lib
#LIBS +=D:\opencv4.5.1\build\install\x64\vc16\lib\opencv_*d.lib
#LIBS +=D:\software\opencv4.5.1\build\install\x64\vc16\lib\opencv_world451.lib
#LIBS +=D:\opencv4.5.1\build\install\x64\vc16\lib\opencv_*.lib

#DEFINES -= UNICODE
#DEFINES += UMBCS
#QMAKE_CXXFLAGS -= -Zc:strictStrings

INCLUDEPATH +=$$PWD/ffmpeg-master-latest-win64-gpl-shared/include\

LIBS += $$PWD/ffmpeg-master-latest-win64-gpl-shared/lib/avdevice.lib \
        $$PWD/ffmpeg-master-latest-win64-gpl-shared/lib/avfilter.lib\
        $$PWD/ffmpeg-master-latest-win64-gpl-shared/lib/avformat.lib\
        $$PWD/ffmpeg-master-latest-win64-gpl-shared/lib/avutil.lib\
        $$PWD/ffmpeg-master-latest-win64-gpl-shared/lib/avcodec.lib\
        $$PWD/ffmpeg-master-latest-win64-gpl-shared/lib/postproc.lib\
        $$PWD/ffmpeg-master-latest-win64-gpl-shared/lib/swresample.lib\
        $$PWD/ffmpeg-master-latest-win64-gpl-shared/lib/swscale.lib\




#添加图标
RC_ICONS = sinoboom.ico


