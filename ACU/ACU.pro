#-------------------------------------------------
#
# Project created by QtCreator 2019-09-23T01:52:26
#
#-------------------------------------------------

QT       += core gui serialport printsupport sql
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ACU
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
    para_adj.cpp \
    back_debug.cpp \
    firware_upd.cpp \
    par_config.cpp \
    tab_widget.cpp \
    man_s.cpp \
    man_p.cpp \
    man_s_p.cpp \
    para_inface.cpp \
    war_limit.cpp \
    deal_thread.cpp \
    qcustomplot.cpp \
    man_widget.cpp \
    sql_oper.cpp \
    pub_info.cpp \
    auto_widget.cpp \
    frmtcpclient.cpp \
    utils_hmac.c \
    utils_md5.c \
    utils_sha1.c \
    httpwindow.cpp \
    app.cpp \
    quiwidget.cpp \
    Ymodem.cpp \
    YmodemFileReceive.cpp \
    YmodemFileTransmit.cpp \
    widget.cpp \
    keyBoard.cpp

HEADERS += \
    para_adj.h \
    back_debug.h \
    firware_upd.h \
    par_config.h \
    tab_widget.h \
    man_s.h \
    man_p.h \
    man_s_p.h \
    para_inface.h \
    war_limit.h \
    deal_thread.h \
    pub_info.h \
    qcustomplot.h \
    man_widget.h \
    sql_oper.h \
    auto_widget.h \
    frmtcpclient.h \
    utils_hmac.h \
    utils_md5.h \
    utils_sha1.h \
    httpwindow.h \
    app.h \
    quiwidget.h \
    Ymodem.h \
    YmodemFileReceive.h \
    YmodemFileTransmit.h \
    widget.h \
    keyBoard.h

FORMS += \
    para_adj.ui \
    back_debug.ui \
    firware_upd.ui \
    par_config.ui \
    tab_widget.ui \
    man_s.ui \
    man_p.ui \
    man_s_p.ui \
    para_inface.ui \
    war_limit.ui \
    man_widget.ui \
    sql_oper.ui \
    auto_widget.ui \
    frmtcpclient.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \
    ACU.pro

RESOURCES += \
    src.qrc
