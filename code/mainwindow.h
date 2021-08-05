#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QSpacerItem>
#include <QCheckBox>
#include <QPushButton>
#include <QProgressBar>
#include "Loading.h"
#include <QSpinBox>
#include <QVector>
#include <QMutex>
#include <QDate>
#include <QButtonGroup>
#include <QComboBox>
#include "qcustomplot.h"
#include "mainForm.h"

QT_BEGIN_NAMESPACE

namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Loading;

struct site
{
    int grid_id=0;
    site(double a,double b)
    {
        //determine the grid_id;
        while(1)
        {
            if(b<=30.47911593){grid_id+=0;break;}
            if(b<=30.52408195){grid_id+=10;break;}
            if(b<=30.56904797){grid_id+=20;break;}
            if(b<=30.61401398){grid_id+=30;break;}
            if(b<=30.65898){grid_id+=40;break;}
            if(b<=30.70394602){grid_id+=50;break;}
            if(b<=30.74891203){grid_id+=60;break;}
            if(b<=30.79387805){grid_id+=70;break;}
            if(b<=30.83884407){grid_id+=80;break;}
            if(b<=30.88381008){grid_id+=90;break;}
            grid_id+=90;
            break;
        }
        while(1)
        {
            if(a<=103.8561346){grid_id+=0;break;}
            if(a<=103.9084075){grid_id+=1;break;}
            if(a<=103.9606803){grid_id+=2;break;}
            if(a<=104.0129532){grid_id+=3;break;}
            if(a<=104.065226){grid_id+=4;break;}
            if(a<=104.1174988){grid_id+=5;break;}
            if(a<=104.1697717){grid_id+=6;break;}
            if(a<=104.2220445){grid_id+=7;break;}
            if(a<=104.2743174){grid_id+=8;break;}
            if(a<=104.3265902){grid_id+=9;break;}
            grid_id+=9;
            break;
        }
        //accomplish check the grid_id;
    }
    site(){}
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void dowork();
    void changedtime();
    void changeatime();
    void changedplace();
    void changeaplace();
    void changefee();
    void clearwidget();
    bool dtimecheck=0,atimecheck=0,dplacecheck=0,aplacecheck=0,feecheck=0;
    int beginday=1;
    int endday=1;
    QString myroad[100];
    QVector<int> dtime;//dtime for the travel time
    //QVector<int> atime;
    QVector<site> dplace;
    QVector<site> aplace;
    QVector<double> fee;
    QVector<int> day;
    QProgressBar *fileloadingProgress;
    QComboBox *viewcombobox;
    QChartView *chartwidget;
    QChart *chart;
    QCheckBox *dtimebox,*atimebox,*dplacebox,*aplacebox,*feebox;
    QSpinBox *leftspinbox,*rightspinbox,*dayedgespinbox;
    QSpinBox *dgridleftspinbox,*dgridrightspinbox,*dgriddayedgespinbox,*dgridspinbox;
    QSpinBox *agridleftspinbox,*agridrightspinbox,*agriddayedgespinbox,*agridspinbox;
    QSpinBox *revenuebegin,*revenueend;
    QSpinBox *hisrevenuebegin,*hisrevenueend;
    QLabel *loadlabel,*begindaylabel,*enddaylabel,*timelabel;
    QLabel *beginlabel,*endlabel,*timeedgelabel,*gridlabel;
    QWidget *widget;
    Widget *predictmap;
    QLabel *mymap;
    QLabel *predictionlabel,*topredictionlabel;
    QLabel *result1,*result2,*result3;
    QButtonGroup *loadingset;
    QPushButton *openfile;
    QPushButton *clickbutton;
    QSpinBox *bday,*eday;
    bool loadingyes;
private:
    Ui::MainWindow *ui;
    Loading *m_ploading;
    QThread *m_pthread;
    QHBoxLayout *hlayer1,*hlayer2,*hlayer3,*hlayer4,*hlayer5,*hlayer6,*hlayer7;//QHBoxLayout takes for left and right
    //halyer1 - the main layer
    //hlayer2 - the day range selector
    //hlayer3 - the name of hlayer2 eg. beginday and endday
    //hlayer4 - parameters for each picture
    //hlayer5 - the spinbox for parameters
    //hlayer6 - the loadinglabel and click label
    //hlayer7 - the predictionpart with instruction labal and a combobox;
    QVBoxLayout *vlayer1,*vlayer2,*vlayer3,*vlayer4;//QVBoxLayout takes for up and down
    //vlayer1 - the left part of whole and the up part is loading part
    //vlayer4- the right part of whole and up is selecting bar
    int state;
    //state=0 chart
    //state=1 png
    //QChartView *m_painting;
    //QThread *m_paintingthread;

    QDate currentdate;

signals:
    void sig_loading(int,int,bool,bool,bool,bool,bool);
    void setfilename(QString);

public slots:
    void slot_finish();
    void slot_sendloadingsignal();
    void bdaychanged(int);
    void edaychanged(int);

    void getdtime(QVector<int>);
    //void getatime(QVector<int>);
    void getdplace(QVector<site>);
    void getaplace(QVector<site>);
    void getfee(QVector<double>);
    void getday(QVector<int>);


    void beginprogressbar();
    void resetprogressbar(int);
    void getfilename();

    void paint(int);
    void paintnumberday();
    void paintempty();
    void paintdepaturegridnumberday();
    void paintarrivalgridnumberday();
    void painttraveltimepie();
    void paintfeepie();
    void paintrevenue();
    void painthisrevenue();

    void resetcombo();
    void setupHeatmapDemo(QCustomPlot *customPlot);
    void switchwindow();
    void showprediction(int);
};
#endif // MAINWINDOW_H


