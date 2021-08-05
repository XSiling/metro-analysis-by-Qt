QT       += core gui
QT       += charts
QT       += printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
INCLUDEPATH +=./zip
CONFIG(debug,debug|release){
win32:win32-g++:PRE_TARGETDEPS +=$$PWD/zip/libquazipd.a
win32:win32-g++:LIBS+= -L$$PWD/zip/ -l quazipd
}else{
    win32:win32-g++:PRE_TARGETDEPS += $$PWD/zip/libquazip.a
    win32:win32-g++:LIBS += -L$$PWD/zip/ -l quazip
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Loading.cpp \
    main.cpp \
    mainForm.cpp \
    mainwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    Loading.h \
    mainForm.h \
    mainwindow.h \
    qcustomplot.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    map.qrc \
    map1.qrc
