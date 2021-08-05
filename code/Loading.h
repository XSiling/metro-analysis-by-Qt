#ifndef LOADING_H
#define LOADING_H

#include <QCoreApplication>
#include <QtCharts>
#include <QDebug>
#include <QThread>
#include <QString>
#include <QObject>
#include <QMainWindow>
#include <mainwindow.h>
QT_CHARTS_USE_NAMESPACE
#include <ui_mainwindow.h>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressBar>
#include "quazip.h"
#include "quazipfile.h"
#include <QMutex>
#include <QMetaType>

struct site;

class Loading:public QObject
{
    Q_OBJECT
public:
    Loading(QObject* parent=NULL);
    ~Loading();
    QString filename;
    QMutex mutexforfile;
    bool alreadygetfilename;

private:
    void setday(int,int,QString&);
    int begin;
    int end;
    bool dtimec,atimec,dplacec,aplacec,feec;
    //int beginday,int endday,bool dtimecheck,bool atimecheck,bool dplacecheck,bool aplacecheck,bool feecheck

signals:
    void sig_finish();
    void senddtime(QVector<int>);
    //void sendatime(QVector<int>);
    void senddplace(QVector<site>);
    void sendaplace(QVector<site>);
    void sendfee(QVector<double>);
    void sendday(QVector<int>);
    void beginprogressbar();
    void resetprogressbar(int);
    void getfilename();
    void readyloading();
    void resetcombo();

public slots:
    void slot_loading(int,int,bool,bool,bool,bool,bool);
    void setfilename(QString);
    void loading();
};



#endif // LOADING_H
