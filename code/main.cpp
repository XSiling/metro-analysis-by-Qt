#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<QVector<int>>("QVector<int>");
    qRegisterMetaType<QVector<site>>("QVector<site>");
    qRegisterMetaType<QVector<double>>("QVector<double>");
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Analysis on Online Ride-Hailing Order Data");
    w.show();
    return a.exec();
}
