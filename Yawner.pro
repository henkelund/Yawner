#-------------------------------------------------
#
# Project created by QtCreator 2011-07-31T17:14:41
#
#-------------------------------------------------

QT       += core gui network script webkit

TARGET = Yawner
TEMPLATE = app

CONFIG += link_pkgconfig
PKGCONFIG += libnotify

SOURCES += main.cpp\
    Yawner/Ui/MainWindow.cpp \
    OAuth/Consumer.cpp \
    OAuth/Token.cpp \
    OAuth/SignatureMethod.cpp \
    OAuth/SignatureMethod/Plaintext.cpp \
    OAuth/Request.cpp \
    OAuth/Util.cpp \
    Yammer/Api.cpp \
    OAuth/Response.cpp \
    Yawner.cpp \
    Yawner/Ui/MessageWidget.cpp \
    Yammer/Message.cpp \
    Yammer/Message/Thread.cpp \
    Yammer/User.cpp \
    Yawner/Manager.cpp \
    Yawner/Manager/UserManager.cpp \
    Yammer/Abstract.cpp \
    Yawner/Ui/MessageText.cpp \
    Yawner/Manager/MessageManager.cpp \
    Yawner/Manager/NotificationManager.cpp \
    Yawner/Ui/View/AbstractView.cpp \
    Yawner/Ui/View/FeedView.cpp \
    Yawner/Ui/View/Snapshot.cpp

HEADERS  += Yawner/Ui/MainWindow.h \
    OAuth/Consumer.h \
    OAuth/Token.h \
    OAuth/SignatureMethod.h \
    OAuth/SignatureMethod/Plaintext.h \
    OAuth/Request.h \
    OAuth/Util.h \
    Yammer/Api.h \
    OAuth/Response.h \
    Yawner.h \
    Yawner/Ui/MessageWidget.h \
    Yammer/Message.h \
    Yammer/Message/Thread.h \
    Yammer/User.h \
    Yawner/Manager.h \
    Yawner/Manager/UserManager.h \
    Yammer/Abstract.h \
    Yawner/Ui/MessageText.h \
    Yawner/Manager/MessageManager.h \
    Yawner/Manager/NotificationManager.h \
    Yawner/Ui/View/AbstractView.h \
    Yawner/Ui/View/FeedView.h \
    Yawner/Ui/View/Snapshot.h

FORMS    += Yawner/Ui/mainwindow.ui \
    Yawner/Ui/messagewidget.ui

RESOURCES = assets/yawner.qrc
