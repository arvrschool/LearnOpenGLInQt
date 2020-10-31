#-------------------------------------------------
#
# Project created by QtCreator 2020-10-27T22:49:19
#
#-------------------------------------------------

QT       += core gui 3dcore opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 9_Load3DObject
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

INCLUDEPATH += \
3rdparty

win32 {
    LIBS += -lopengl32
    LIBS += -L$$PWD/lib/win/ -lassimp-vc140-mt
}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    myshader.cpp \
    mycamera.cpp \
    myopenglwindow.cpp \
    model.cpp

HEADERS += \
        mainwindow.h \
    mesh.h \
    model.h \
    myshader.h \
    mycamera.h \
    myopenglwindow.h \
    stb_image.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
