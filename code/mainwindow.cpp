#include "mainwindow.h"
#include "ui_mainwindow.h"
extern QMutex mutex1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    currentdate=QDate::currentDate();


    ui->setupUi(this);
    QString thistime,tmp;
    loadingyes=0;
    state=0;
    viewcombobox=new QComboBox;
    widget=new QWidget;
    bday=new QSpinBox;
    bday->setRange(1,15);
    bday->setSingleStep(1);
    eday=new QSpinBox;
    eday->setRange(1,15);
    eday->setSingleStep(1);

    QFile roadfile("D:/road.txt");
    if(!roadfile.open(QFile::ReadOnly|QFile::Text))
    {
     qDebug()<<"can't open";
    }
    int jjj=0;
    while(!roadfile.atEnd())
    {
        QByteArray line=roadfile.readLine();
        QString str(line);
        qDebug()<<str;
        str=str.simplified();
        myroad[jjj]=str;
        ++jjj;
    }

    hlayer1=new QHBoxLayout;//
    hlayer2=new QHBoxLayout;
    hlayer3=new QHBoxLayout;
    hlayer4=new QHBoxLayout;
    hlayer5=new QHBoxLayout;
    hlayer6=new QHBoxLayout;
    vlayer1=new QVBoxLayout;
    vlayer2=new QVBoxLayout;
    vlayer3=new QVBoxLayout;
    vlayer4=new QVBoxLayout;

    hlayer1->addLayout(vlayer1);
    //hlayer1->addLayout(vlayer2,Qt::AlignLeft);
    //vlayer1->SetFixedSize;
    vlayer1->addLayout(vlayer3);
    vlayer1->setStretch(0,3);
    vlayer1->setStretch(1,5);

    hlayer2->addWidget(bday);
    hlayer2->addWidget(eday);


    QImage image(QString(":/map.png"));
    //image.scaled(50,50);
    mymap=new QLabel;
    mymap->setPixmap(QPixmap::fromImage(image));
    mymap->resize(50,50);
    //mymap->setPixmap(pix);


    //set combobox here!
    //here here here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!WOWOWOWOWOWOOWOWOWOWOOWOWOO
    viewcombobox->addItem("none");
    viewcombobox->addItem("number-day");
    viewcombobox->addItem("number-day(depature grid)");
    viewcombobox->addItem("number-day(arrival grid)");
    viewcombobox->addItem("distribution of travel time");
    viewcombobox->addItem("distribution of fee");
    viewcombobox->addItem("revenue line chart");
    viewcombobox->addItem("revenue histogram");
    //viewcombobox[0].setBackgroundRole(QPalette::Light);
    //viewcombobox[1].setBackgroundRole(QPalette::Shadow);


    chartwidget=new QChartView;
    vlayer4->addWidget(viewcombobox);
    vlayer4->addLayout(hlayer4);
    vlayer4->addLayout(hlayer5);
    vlayer4->addWidget(chartwidget);

    hlayer1->addLayout(vlayer4,1);

    widget->setLayout(hlayer1);
    setCentralWidget(widget);
    //design the loading part - vlayer3
    loadlabel=new QLabel("Loading");
    begindaylabel=new QLabel("from/day");
    enddaylabel=new QLabel("to/day");


    hlayer3->addWidget(begindaylabel);
    hlayer3->addWidget(enddaylabel);


    loadingset=new QButtonGroup(this);
    loadingset->setExclusive(false);//can get more than 1 options

    clickbutton=new QPushButton("picture");
    hlayer6->addWidget(loadlabel);
    hlayer6->addWidget(clickbutton);
    vlayer3->addLayout(hlayer6);
    vlayer3->addSpacerItem(new QSpacerItem(20,10,QSizePolicy::Expanding));

    dtimebox=new QCheckBox("travel time",this);
    loadingset->addButton(dtimebox);
    vlayer3->addWidget(dtimebox);
    //atimebox=new QCheckBox("arrival time",this);
    //loadingset->addButton(atimebox);
    //vlayer3->addWidget(atimebox);
    dplacebox=new QCheckBox("departure location",this);
    loadingset->addButton(dplacebox);
    vlayer3->addWidget(dplacebox);
    aplacebox=new QCheckBox("arrival location",this);
    loadingset->addButton(aplacebox);
    vlayer3->addWidget(aplacebox);
    feebox=new QCheckBox("fee",this);
    loadingset->addButton(feebox);
    vlayer3->addWidget(feebox);


    vlayer3->addLayout(hlayer3);
    vlayer3->addLayout(hlayer2);



    openfile=new QPushButton("open file");
    vlayer3->addWidget(openfile);


    this->setLayout(hlayer1);
    //showing the time label;
    timelabel=new QLabel();
    thistime="";
    tmp=QString::number(currentdate.year());
    thistime+=tmp;
    thistime+="/";
    tmp=QString::number(currentdate.month());
    thistime+=tmp;
    thistime+="/";
    tmp=QString::number(currentdate.day());
    thistime+=tmp;
    timelabel->setText(thistime);
    vlayer1->addWidget(timelabel);

    //vlayer1->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
    vlayer1->addWidget(mymap);

    //----------vlayer4->addWidget(viewcombobox);

    m_ploading=new Loading(0);
    m_pthread=new QThread();
    m_ploading->moveToThread(m_pthread);

    //m_painting=new ChartView(0);
    //m_paintingthread=new QThread();
    //m_painting->moveToThread(m_paintingthread);


    //check whether information has been loaded
    if(loadingyes)
    {qDebug()<<day.size();}
    //begin to paint!




    connect(m_pthread,&QThread::finished,m_ploading,&QObject::deleteLater);
    connect(openfile,&QPushButton::clicked,this,&MainWindow::dowork);//once click the button , begin the thread.
    connect(this,SIGNAL(sig_loading(int,int,bool,bool,bool,bool,bool)),m_ploading,SLOT(slot_loading(int,int,bool,bool,bool,bool,bool)));
    connect(m_ploading,SIGNAL(sig_finish()),this,SLOT(slot_finish()));
    connect(dtimebox,&QCheckBox::stateChanged,this,&MainWindow::changedtime);
    //connect(atimebox,&QCheckBox::stateChanged,this,&MainWindow::changeatime);
    connect(dplacebox,&QCheckBox::stateChanged,this,&MainWindow::changedplace);
    connect(aplacebox,&QCheckBox::stateChanged,this,&MainWindow::changeaplace);
    connect(feebox,&QCheckBox::stateChanged,this,&MainWindow::changefee);
    connect(bday,SIGNAL(valueChanged(int)),this,SLOT(bdaychanged(int)));
    connect(eday,SIGNAL(valueChanged(int)),this,SLOT(edaychanged(int)));
    connect(m_ploading,&Loading::senddtime,this,&MainWindow::getdtime);
    //connect()

    connect(m_ploading,&Loading::beginprogressbar,this,&MainWindow::beginprogressbar);
    connect(m_ploading,&Loading::resetprogressbar,this,&MainWindow::resetprogressbar);
    connect(m_ploading,SIGNAL(getfilename()),this,SLOT(getfilename()));
    connect(this,SIGNAL(setfilename(QString)),m_ploading,SLOT(setfilename(QString)));

    connect(m_ploading,&Loading::senddtime,this,&MainWindow::getdtime);
    //connect(m_ploading,&Loading::sendatime,this,&MainWindow::getatime);
    connect(m_ploading,&Loading::senddplace,this,&MainWindow::getdplace);
    connect(m_ploading,&Loading::sendaplace,this,&MainWindow::getaplace);
    connect(m_ploading,&Loading::sendfee,this,&MainWindow::getfee);
    connect(m_ploading,&Loading::sendday,this,&MainWindow::getday);


    connect(m_ploading,&Loading::readyloading,m_ploading,&Loading::loading);//the function of loading files

    connect(viewcombobox,SIGNAL(currentIndexChanged(int)),this,SLOT(paint(int)));

    connect(m_ploading,&Loading::resetcombo,this,&MainWindow::resetcombo);

    connect(clickbutton,&QPushButton::clicked,this,&MainWindow::switchwindow);

}

MainWindow::~MainWindow()
{
    delete ui;
    m_pthread->quit();
    m_pthread->wait();
}

void MainWindow::dowork()
{
    qDebug()<<"ready!";
    if(loadingyes==1)
    {
        qDebug()<<"yes!";
        delete fileloadingProgress;
    }
    fileloadingProgress=new QProgressBar;
    //loadingyes=0;
    fileloadingProgress->setRange(0,(endday-beginday+1)*5);
    fileloadingProgress->setValue(0);
    this->statusBar()->addWidget(fileloadingProgress,0);


    m_pthread->start();
    emit sig_loading(this->beginday,this->endday,this->dtimecheck,this->atimecheck,this->dplacecheck,this->aplacecheck,this->feecheck);
}

void MainWindow::slot_finish()
{
    //nothing here.
    //emit setcombobox();
    //just here setting the combobox;
    //check whether
}

void MainWindow::changedtime()
{
    this->dtimecheck=!this->dtimecheck;
}

void MainWindow::changeatime()
{
    this->atimecheck=!this->atimecheck;
}

void MainWindow::changedplace()
{
    this->dplacecheck=!this->dplacecheck;
}

void MainWindow::changeaplace()
{
    this->aplacecheck=!this->aplacecheck;
}

void MainWindow::changefee()
{
    this->feecheck=!this->feecheck;
}

void MainWindow::slot_sendloadingsignal()
{
    emit this->sig_loading(this->beginday,this->endday,this->dtimecheck,this->atimecheck,this->dplacecheck,this->aplacecheck,this->feecheck);
}

void MainWindow::bdaychanged(int d)
{
    beginday=d;
}

void MainWindow::edaychanged(int d)
{
    endday=d;
}

void MainWindow::beginprogressbar()
{

}

void MainWindow::resetprogressbar(int x)
{
    fileloadingProgress->setValue(x);
    if(fileloadingProgress->value()==(endday-beginday+1)*5)
    {
        loadingyes=1;
    }
}


void MainWindow::resetcombo()
{
    viewcombobox->setCurrentIndex(0);
}

void MainWindow::getdtime(QVector<int> d)
{
    dtime.clear();
    int x=d.size();
    for(int i=0;i<x;++i)
    {
        dtime.append(d[i]);
    }
    if(x!=0)
    {
        dtimebox->setText("depature time√");
    }
    else
    {
        dtimebox->setText("depature time");
    }
}

/*void MainWindow::getatime(QVector<int> a)
{
    atime.clear();
    int x=a.size();
    for(int i=0;i<x;++i)
    {
        atime.append(a[i]);
    }
    if(x!=0)
    {
        atimebox->setText("arrival time√");
    }
    else
    {
        atimebox->setText("arrival time");
    }
}*/

void MainWindow::getdplace(QVector<site> d)
{
    dplace.clear();
    int x=d.size();
    for(int i=0;i<x;++i)
    {
        dplace.append(d[i]);
    }
    if(x!=0)
    {
        dplacebox->setText("depature location√");
    }
    else
    {
        dplacebox->setText("depature location");
    }
}

void MainWindow::getaplace(QVector<site> a)
{
    aplace.clear();
    int x=a.size();
    for(int i=0;i<x;++i)
    {
        aplace.append(a[i]);
    }
    if(x!=0)
    {
        aplacebox->setText("arrival location√");
    }
    else
    {
        aplacebox->setText("arrival location");
    }
}

void MainWindow::getfee(QVector<double> f)
{
    fee.clear();
    int x=f.size();
    for(int i=0;i<x;++i)
    {
        fee.append(f[i]);
    }
    if(x!=0)
    {
        feebox->setText("fee√");
    }
    else
    {
        feebox->setText("fee");
    }
}

void MainWindow::getday(QVector<int> d)
{
    day.clear();
    int x=d.size();
    for(int i=0;i<x;++i)
    {
        day.append(d[i]);
    }
}

void MainWindow::getfilename()
{
    QString filename=QFileDialog::getOpenFileName(0,tr("open a file"),"",tr("packages(*.zip *.rar);;All files(*.*)"));
    emit setfilename(filename);
}

void MainWindow::clearwidget()
{
    QLayoutItem *child;
    if(hlayer4!=NULL)
    {while((child=hlayer4->takeAt(0))!=0)
    {
        if(child->widget())
        {
            child->widget()->setParent(NULL);
        }
        delete child;
    }}
    if(hlayer5!=NULL)
    {while((child=hlayer5->takeAt(0))!=0)
    {
        if(child->widget())
        {
            child->widget()->setParent(NULL);
        }
        delete child;
    }}
}

void MainWindow::paint(int index)
{
    switch(index)
    {
    case 0://none
    {
        clearwidget();
        paintempty();
        break;
    }
    case 1://number-day
    {
        if(day.isEmpty())
        {
            viewcombobox->setCurrentIndex(0);
            QMessageBox::information(NULL,"ERROR","No enough data!");
            break;
        }
        else
        {
            clearwidget();
            beginlabel=new QLabel;
            beginlabel->setText("begin from:");
            endlabel=new QLabel;
            endlabel->setText("end to:");
            timeedgelabel=new QLabel;
            timeedgelabel->setText("edge:");
            hlayer4->addWidget(beginlabel);
            hlayer4->addWidget(endlabel);
            hlayer4->addWidget(timeedgelabel);

            leftspinbox=new QSpinBox;
            leftspinbox->setRange(beginday,endday);
            leftspinbox->setValue(beginday);
            hlayer5->addWidget(leftspinbox);
            rightspinbox=new QSpinBox;
            rightspinbox->setRange(beginday,endday);
            rightspinbox->setValue(endday);
            hlayer5->addWidget(rightspinbox);
            dayedgespinbox=new QSpinBox;
            dayedgespinbox->setRange(1,2);
            dayedgespinbox->setValue(1);
            hlayer5->addWidget(dayedgespinbox);
            //then get the painting yeah♂
            paintnumberday();

            connect(leftspinbox,SIGNAL(valueChanged(int)),this,SLOT(paintnumberday()));
            connect(rightspinbox,SIGNAL(valueChanged(int)),this,SLOT(paintnumberday()));
            connect(dayedgespinbox,SIGNAL(valueChanged(int)),this,SLOT(paintnumberday()));
        }
        break;
    }
    case 2://depature:number-day
    {
        //paint with grid;
        if(day.isEmpty()||dplace.isEmpty())
        {
            viewcombobox->setCurrentIndex(0);
            QMessageBox::information(NULL,"ERROR","No enough data!");
            break;
        }
        else
        {
            clearwidget();
            beginlabel=new QLabel;
            beginlabel->setText("begin from");
            endlabel=new QLabel;
            endlabel->setText("end to");
            timeedgelabel=new QLabel;
            timeedgelabel->setText("edge");
            gridlabel=new QLabel;
            gridlabel->setText("select grid");
            hlayer4->addWidget(beginlabel);
            hlayer4->addWidget(endlabel);
            hlayer4->addWidget(timeedgelabel);
            hlayer4->addWidget(gridlabel);

            dgridleftspinbox=new QSpinBox;
            dgridleftspinbox->setRange(beginday,endday);
            dgridleftspinbox->setValue(beginday);
            hlayer5->addWidget(dgridleftspinbox);
            dgridrightspinbox=new QSpinBox;
            dgridrightspinbox->setRange(beginday,endday);
            dgridrightspinbox->setValue(endday);
            hlayer5->addWidget(dgridrightspinbox);
            dgriddayedgespinbox=new QSpinBox;
            dgriddayedgespinbox->setRange(1,2);
            dgriddayedgespinbox->setValue(1);
            hlayer5->addWidget(dgriddayedgespinbox);
            dgridspinbox=new QSpinBox;
            dgridspinbox->setRange(0,99);
            dgridspinbox->setValue(0);
            hlayer5->addWidget(dgridspinbox);
            paintdepaturegridnumberday();

            connect(dgridleftspinbox,SIGNAL(valueChanged(int)),this,SLOT(paintdepaturegridnumberday()));
            connect(dgridrightspinbox,SIGNAL(valueChanged(int)),this,SLOT(paintdepaturegridnumberday()));
            connect(dgriddayedgespinbox,SIGNAL(valueChanged(int)),this,SLOT(paintdepaturegridnumberday()));
            connect(dgridspinbox,SIGNAL(valueChanged(int)),this,SLOT(paintdepaturegridnumberday()));
        }
        break;
    }
    case 3://arrival number-day
    {
        if(day.isEmpty()||aplace.isEmpty())
        {
            viewcombobox->setCurrentIndex(0);
            QMessageBox::information(NULL,"ERROR","No enough data!");
            break;
        }
        else
        {
            clearwidget();
            beginlabel=new QLabel;
            beginlabel->setText("begin from");
            endlabel=new QLabel;
            endlabel->setText("end to");
            timeedgelabel=new QLabel;
            timeedgelabel->setText("edge");
            gridlabel=new QLabel;
            gridlabel->setText("select grid");
            hlayer4->addWidget(beginlabel);
            hlayer4->addWidget(endlabel);
            hlayer4->addWidget(timeedgelabel);
            hlayer4->addWidget(gridlabel);

            agridleftspinbox=new QSpinBox;
            agridleftspinbox->setRange(beginday,endday);
            agridleftspinbox->setValue(beginday);
            hlayer5->addWidget(agridleftspinbox);
            agridrightspinbox=new QSpinBox;
            agridrightspinbox->setRange(beginday,endday);
            agridrightspinbox->setValue(endday);
            hlayer5->addWidget(agridrightspinbox);
            agriddayedgespinbox=new QSpinBox;
            agriddayedgespinbox->setRange(1,2);
            agriddayedgespinbox->setValue(1);
            hlayer5->addWidget(agriddayedgespinbox);
            agridspinbox=new QSpinBox;
            agridspinbox->setRange(0,99);
            agridspinbox->setValue(0);
            hlayer5->addWidget(agridspinbox);
            paintarrivalgridnumberday();

            connect(agridleftspinbox,SIGNAL(valueChanged(int)),this,SLOT(paintarrivalgridnumberday()));
            connect(agridrightspinbox,SIGNAL(valueChanged(int)),this,SLOT(paintarrivalgridnumberday()));
            connect(agriddayedgespinbox,SIGNAL(valueChanged(int)),this,SLOT(paintarrivalgridnumberday()));
            connect(agridspinbox,SIGNAL(valueChanged(int)),this,SLOT(paintarrivalgridnumberday()));
        }
        break;
    }
    case 4://distribution of travel time--piechart
    {
        if(dtime.isEmpty())
        {
            viewcombobox->setCurrentIndex(0);
            QMessageBox::information(NULL,"ERROR","No enough data!");
            break;
        }
        else
        {
            clearwidget();
            //do not want parameters this time
            painttraveltimepie();
        }
        break;
    }
    case 5:
    {
        if(fee.isEmpty())
        {
            viewcombobox->setCurrentIndex(0);
            QMessageBox::information(NULL,"ERROR","No enough data!");
            break;
        }
        else
        {
            clearwidget();
            paintfeepie();
        }
        break;
    }
    case 6:
    {
        if(fee.isEmpty())
        {
            viewcombobox->setCurrentIndex(0);
            QMessageBox::information(NULL,"ERROR","No enough data!");
            break;
        }
        else
        {
            clearwidget();
            beginlabel= new QLabel;
            beginlabel->setText("begin from/day");
            endlabel= new QLabel;
            endlabel->setText("end to/day");
            hlayer4->addWidget(beginlabel);
            hlayer4->addWidget(endlabel);

            revenuebegin=new QSpinBox;
            revenuebegin->setRange(beginday,endday);
            revenuebegin->setValue(beginday);
            revenueend=new QSpinBox;
            revenueend->setRange(beginday,endday);
            revenueend->setValue(endday);
            hlayer5->addWidget(revenuebegin);
            hlayer5->addWidget(revenueend);

            paintrevenue();

            connect(revenuebegin,SIGNAL(valueChanged(int)),this,SLOT(paintrevenue()));
            connect(revenueend,SIGNAL(valueChanged(int)),this,SLOT(paintrevenue()));
        }
        break;
    }
    case 7://revenue histogram
    {
        if(fee.isEmpty())
        {
            viewcombobox->setCurrentIndex(0);
            QMessageBox::information(NULL,"ERROR","No enough data!");
            break;
        }
        else
        {
            clearwidget();
            beginlabel= new QLabel;
            beginlabel->setText("begin from/day");
            endlabel= new QLabel;
            endlabel->setText("end to/day");
            hlayer4->addWidget(beginlabel);
            hlayer4->addWidget(endlabel);

            hisrevenuebegin=new QSpinBox;
            hisrevenuebegin->setRange(beginday,endday);
            hisrevenuebegin->setValue(beginday);
            hisrevenueend=new QSpinBox;
            hisrevenueend->setRange(beginday,endday);
            hisrevenueend->setValue(endday);
            hlayer5->addWidget(hisrevenuebegin);
            hlayer5->addWidget(hisrevenueend);

            painthisrevenue();

            connect(hisrevenuebegin,SIGNAL(valueChanged(int)),this,SLOT(painthisrevenue()));
            connect(hisrevenueend,SIGNAL(valueChanged(int)),this,SLOT(painthisrevenue()));
        }
        break;
    }


    }
}

void MainWindow::paintnumberday()
{
    int begin=leftspinbox->value();
    int end=rightspinbox->value();
    int edge=dayedgespinbox->value();
    //qDebug()<<"edge"<<edge;
    int totalx=0;

    if(begin>end)
    {
        QMessageBox::information(NULL,"ERROR","Wrong range!");
        return;
    }

    chart=new QChart;
    QSplineSeries *myseries=new QSplineSeries;
    myseries->setColor(QColor(0,100,255));
    qDebug()<<day.size();
    for(int i=begin;i<=end;++i)
    {
        ++totalx;
    }

    int *yl,*xl;
    xl=new int[totalx]{0};//store each x
    yl=new int[totalx]{0};//store eacy y to x
    for(int i=begin;i<=end;++i)
    {
        xl[i-begin]=i;
    }
    for(int i=0;i<day.size();++i)
    {
        for(int j=0;j<totalx;++j)//determing which day is for
        {
            if(day[i]==xl[j])
            {
                ++yl[j];
                break;
            }
        }
    }
    for(int i=0;i<totalx;++i)
    {
        qDebug()<<yl[i];
        myseries->append(QPointF(xl[i],yl[i]));
        if(edge==2){++i;}
    }

    myseries->setName("one");
    myseries->setPointLabelsFormat("(@xPoint,@yPoint)");
    myseries->setPointLabelsVisible(true);

    chart->setTheme(QChart::ChartThemeLight);
    chart->setDropShadowEnabled(true);
    chart->addSeries(myseries);

    chart->setTitleBrush(QBrush(QColor(0,0,255)));
    chart->setTitleFont(QFont("微软雅黑"));
    chart->setTitle("number-day");

    QValueAxis *axisX=new QValueAxis;
    axisX->setRange(begin-1,end+1);
    axisX->setLabelFormat("day%d");
    QValueAxis *axisY=new QValueAxis;
    axisY->setRange(180000,210000);
    axisY->setTitleText("number");


    chart->setAxisX(axisX,myseries);
    chart->setAxisY(axisY,myseries);
    //chart->createDefaultAxes();

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setBackgroundVisible(true);
    chart->legend()->setAutoFillBackground(true);
    chart->legend()->setColor(QColor(222,233,251));
    chart->legend()->setLabelColor(QColor(0,100,255));
    chart->legend()->setMaximumHeight(50);
    chartwidget->setChart(chart);
    chartwidget->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::paintdepaturegridnumberday()
{
    int begin=dgridleftspinbox->value();
    int end=dgridrightspinbox->value();
    int edge=dgriddayedgespinbox->value();
    int grid=dgridspinbox->value();
    int totalx=0;

    if(begin>end)
    {
        QMessageBox::information(NULL,"ERROR","Wrong range!");
        return;
    }

    chart=new QChart;
    QSplineSeries *myseries=new QSplineSeries;
    myseries->setColor(QColor(0,100,255));
    qDebug()<<day.size();
    for(int i=begin;i<=end;++i)
    {
        ++totalx;
    }

    int *yl,*xl;
    xl=new int[totalx]{0};//store each x
    yl=new int[totalx]{0};//store eacy y to x
    for(int i=begin;i<=end;++i)
    {
        xl[i-begin]=i;
    }
    for(int i=0;i<day.size();++i)
    {
        for(int j=0;j<totalx;++j)//determing which day is for
        {
            if(day[i]==xl[j]&&dplace[i].grid_id==grid)
            {
                ++yl[j];
                break;
            }
        }
    }
    for(int i=0;i<totalx;++i)
    {
        qDebug()<<yl[i];
        myseries->append(QPointF(xl[i],yl[i]));
        if(edge==2){++i;}
    }

    myseries->setName(QString::number(grid));
    myseries->setPointLabelsFormat("(@xPoint,@yPoint)");
    myseries->setPointLabelsVisible(true);

    chart->setTheme(QChart::ChartThemeLight);
    chart->setDropShadowEnabled(true);
    chart->addSeries(myseries);

    chart->setTitleBrush(QBrush(QColor(0,0,255)));
    chart->setTitleFont(QFont("微软雅黑"));
    chart->setTitle("depature number-day with grid");

    QValueAxis *axisX=new QValueAxis;
    axisX->setRange(begin-1,end+1);
    axisX->setLabelFormat("day%d");
    QValueAxis *axisY=new QValueAxis;
    //axisY->setRange(180000,210000);
    axisY->setTitleText("number");


    chart->setAxisX(axisX,myseries);
    chart->setAxisY(axisY,myseries);
    //chart->createDefaultAxes();

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setBackgroundVisible(true);
    chart->legend()->setAutoFillBackground(true);
    chart->legend()->setColor(QColor(222,233,251));
    chart->legend()->setLabelColor(QColor(0,100,255));
    chart->legend()->setMaximumHeight(50);
    chartwidget->setChart(chart);
    chartwidget->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::paintarrivalgridnumberday()
{
    int begin=agridleftspinbox->value();
    int end=agridrightspinbox->value();
    int edge=agriddayedgespinbox->value();
    int grid=agridspinbox->value();
    int totalx=0;

    if(begin>end)
    {
        QMessageBox::information(NULL,"ERROR","Wrong range!");
        return;
    }

    chart=new QChart;
    QSplineSeries *myseries=new QSplineSeries;
    myseries->setColor(QColor(0,100,255));
    qDebug()<<day.size();
    for(int i=begin;i<=end;++i)
    {
        ++totalx;
    }

    int *yl,*xl;
    xl=new int[totalx]{0};//store each x
    yl=new int[totalx]{0};//store eacy y to x
    for(int i=begin;i<=end;++i)
    {
        xl[i-begin]=i;
    }
    for(int i=0;i<day.size();++i)
    {
        for(int j=0;j<totalx;++j)//determing which day is for
        {
            if(day[i]==xl[j]&&aplace[i].grid_id==grid)
            {
                ++yl[j];
                break;
            }
        }
    }
    for(int i=0;i<totalx;++i)
    {
        qDebug()<<yl[i];
        myseries->append(QPointF(xl[i],yl[i]));
        if(edge==2){++i;}
    }

    myseries->setName(QString::number(grid));
    myseries->setPointLabelsFormat("(@xPoint,@yPoint)");
    myseries->setPointLabelsVisible(true);

    chart->setTheme(QChart::ChartThemeLight);
    chart->setDropShadowEnabled(true);
    chart->addSeries(myseries);

    chart->setTitleBrush(QBrush(QColor(0,0,255)));
    chart->setTitleFont(QFont("微软雅黑"));
    chart->setTitle("arrival number-day with grid");

    QValueAxis *axisX=new QValueAxis;
    axisX->setRange(begin-1,end+1);
    axisX->setLabelFormat("day%d");
    QValueAxis *axisY=new QValueAxis;
    //axisY->setRange(180000,210000);
    axisY->setTitleText("number");


    chart->setAxisX(axisX,myseries);
    chart->setAxisY(axisY,myseries);
    //chart->createDefaultAxes();

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setBackgroundVisible(true);
    chart->legend()->setAutoFillBackground(true);
    chart->legend()->setColor(QColor(222,233,251));
    chart->legend()->setLabelColor(QColor(0,100,255));
    chart->legend()->setMaximumHeight(50);
    chartwidget->setChart(chart);
    chartwidget->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::painttraveltimepie()
{
    chart=new QChart();
    QPieSeries *myseries =new QPieSeries();
    int value[4]{0};
    int betweenvalue[3]={5,10,30};
    //0-5 5-10 10-30 30+
    for(int i=0;i<day.size();++i)
    {
        if(dtime[i]<betweenvalue[0]*60)
        {
            ++value[0];continue;
        }
        if(dtime[i]<betweenvalue[1]*60)
        {
            ++value[1];
            continue;
        }
        if(dtime[i]<betweenvalue[2]*60)
        {
            ++value[2];
            continue;
        }
        ++value[3];
    }

    //qDebug()<<value[0];
    //qDebug()<<value[1];
    //qDebug()<<value[2];
    //qDebug()<<value[3];
    //qDebug()<<value[4];
    myseries->append("0 to 5min",value[0]);
    myseries->append("5 to 10min",value[1]);
    myseries->append("10 to 20min",value[2]);
    myseries->append("20 to moremin",value[3]);

    myseries->setLabelsVisible(true);
    myseries->setUseOpenGL(true);

    myseries->slices().at(0)->setColor(QColor(13, 128, 217)); //设置颜色
    myseries->slices().at(0)->setLabelColor(QColor(13, 128, 217));
    myseries->slices().at(1)->setColor(QColor(69, 13, 217));
    myseries->slices().at(1)->setLabelColor(QColor(69, 13, 217));
    myseries->slices().at(2)->setColor(QColor(200, 50, 50));
    myseries->slices().at(2)->setLabelColor(QColor(200, 50, 50));
    myseries->slices().at(3)->setColor(QColor(120, 217, 110));
    myseries->slices().at(3)->setLabelColor(QColor(120, 217, 110));
    /*myseries->slices().at(4)->setColor(QColor(13, 0, 110));
    myseries->slices().at(4)->setLabelColor(QColor(13, 0, 110));
    myseries->slices().at(5)->setColor(QColor(230, 217, 110));
    myseries->slices().at(5)->setLabelColor(QColor(230, 217, 110));*/
    chart->setTheme(QChart::ChartThemeLight);//设置白色主题
    chart->setDropShadowEnabled(true);//背景阴影
    chart->addSeries(myseries);//添加系列到QChart上

    chart->setTitleBrush(QBrush(QColor(0, 0, 255))); //设置标题Brush
    chart->setTitleFont(QFont("微软雅黑"));//设置标题字体
    chart->setTitle("distribution of travel time");

    //修改说明样式
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);//底部对齐
    chart->legend()->setBackgroundVisible(true);//设置背景是否可视
    chart->legend()->setAutoFillBackground(true);//设置背景自动填充
    chart->legend()->setColor(QColor(222, 233, 251)); //设置颜色
    chart->legend()->setLabelColor(QColor(0, 100, 255)); //设置标签颜色
    //chart->legend()->setMaximumHeight(150);
    chartwidget->setChart(chart);
    chartwidget->setRenderHint(QPainter::Antialiasing);


}

void MainWindow::paintfeepie()
{
    chart=new QChart();
    QPieSeries *myseries =new QPieSeries();
    int value[4]{0};
    double betweenvalue[3]={2.0,5.0,10.0};
    //0-5 5-10 10-30 30+
    for(int i=0;i<day.size();++i)
    {
        if(fee[i]<betweenvalue[0])
        {
            ++value[0];continue;
        }
        if(fee[i]<betweenvalue[1])
        {
            ++value[1];
            continue;
        }
        if(fee[i]<betweenvalue[2])
        {
            ++value[2];
            continue;
        }
        ++value[3];
    }

    //qDebug()<<value[0];
    //qDebug()<<value[1];
    //qDebug()<<value[2];
    //qDebug()<<value[3];
    //qDebug()<<value[4];
    myseries->append("0 to 2￥",value[0]);
    myseries->append("2 to 5￥",value[1]);
    myseries->append("5 to 10￥",value[2]);
    myseries->append("10 to more￥",value[3]);

    myseries->setLabelsVisible(true);
    myseries->setUseOpenGL(true);

    myseries->slices().at(0)->setColor(QColor(13, 128, 217)); //设置颜色
    myseries->slices().at(0)->setLabelColor(QColor(13, 128, 217));
    myseries->slices().at(1)->setColor(QColor(69, 13, 217));
    myseries->slices().at(1)->setLabelColor(QColor(69, 13, 217));
    myseries->slices().at(2)->setColor(QColor(200, 50, 50));
    myseries->slices().at(2)->setLabelColor(QColor(200, 50, 50));
    myseries->slices().at(3)->setColor(QColor(120, 217, 110));
    myseries->slices().at(3)->setLabelColor(QColor(120, 217, 110));
    /*myseries->slices().at(4)->setColor(QColor(13, 0, 110));
    myseries->slices().at(4)->setLabelColor(QColor(13, 0, 110));
    myseries->slices().at(5)->setColor(QColor(230, 217, 110));
    myseries->slices().at(5)->setLabelColor(QColor(230, 217, 110));*/
    chart->setTheme(QChart::ChartThemeLight);//设置白色主题
    chart->setDropShadowEnabled(true);//背景阴影
    chart->addSeries(myseries);//添加系列到QChart上

    chart->setTitleBrush(QBrush(QColor(0, 0, 255))); //设置标题Brush
    chart->setTitleFont(QFont("微软雅黑"));//设置标题字体
    chart->setTitle("distribution of fee");

    //修改说明样式
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);//底部对齐
    chart->legend()->setBackgroundVisible(true);//设置背景是否可视
    chart->legend()->setAutoFillBackground(true);//设置背景自动填充
    chart->legend()->setColor(QColor(222, 233, 251)); //设置颜色
    chart->legend()->setLabelColor(QColor(0, 100, 255)); //设置标签颜色
    //chart->legend()->setMaximumHeight(150);
    chartwidget->setChart(chart);
    chartwidget->setRenderHint(QPainter::Antialiasing);

}

void MainWindow::paintrevenue()
{
    int begin=revenuebegin->value();
    int end=revenueend->value();
    double sum=0;
    int currentday=begin;
    bool turn=false;
    if(begin>end)
    {
        QMessageBox::information(NULL,"ERROR","Wrong range!");
        return;
    }

    chart=new QChart;
    QSplineSeries *myseries=new QSplineSeries;
    myseries->setColor(QColor(0,100,255));
    qDebug()<<day.size();

    for(int i=0;i<fee.size();++i)
    {
        if(i>0&&day[i]!=day[i-1])
        {
            turn=true;
        }
        if(turn) {
            myseries->append(QPointF(currentday,sum));
            ++currentday;
            turn=false;
        }
        sum+=fee[i];
    }

    myseries->setName("revenue line");
    myseries->setPointLabelsFormat("(@xPoint,@yPoint)");
    myseries->setPointLabelsVisible(false);

    chart->setTheme(QChart::ChartThemeLight);
    chart->setDropShadowEnabled(true);
    chart->addSeries(myseries);

    chart->setTitleBrush(QBrush(QColor(0,0,255)));
    chart->setTitleFont(QFont("微软雅黑"));
    chart->setTitle("revenue line chart");

    QValueAxis *axisX=new QValueAxis;
    axisX->setRange(begin-1,end+1);
    axisX->setLabelFormat("day%d");
    QValueAxis *axisY=new QValueAxis;
    //axisY->setRange(180000,210000);
    axisY->setTitleText("revenue/￥");


    chart->setAxisX(axisX,myseries);
    chart->setAxisY(axisY,myseries);
    //chart->createDefaultAxes();

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setBackgroundVisible(true);
    chart->legend()->setAutoFillBackground(true);
    chart->legend()->setColor(QColor(222,233,251));
    chart->legend()->setLabelColor(QColor(0,100,255));
    chart->legend()->setMaximumHeight(50);
    chartwidget->setChart(chart);
    chartwidget->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::painthisrevenue()
{
    const int begin=hisrevenuebegin->value();
    const int end=hisrevenueend->value();
    double sum[end-begin+1];
    chart=new QChart();

    QBarSet *set=new QBarSet("revenue each day");
    for(int i=0;i<end-begin+1;++i)
    {
        sum[i]=0;
    }

    for(int i=0;i<fee.size();++i)
    {
        sum[day[i]-begin]+=fee[i];
    }

    for(int i=0;i<end-begin+1;++i)
    {
        *set<<sum[i];
    }

    set->setLabelColor(QColor(0,0,255));

    QBarSeries *myseries=new QBarSeries();
    myseries->append(set);
    myseries->setVisible(true);
    myseries->setLabelsVisible(true);

    chart->setTheme(QChart::ChartThemeLight);
    chart->setDropShadowEnabled(true);
    chart->addSeries(myseries);

    QBarCategoryAxis *axisX =new QBarCategoryAxis;
    for(int i=begin;i<=end;++i)
    {
        axisX->append(QString::number(i));
    }
    axisX->setLabelsColor(QColor(7,28,96));

    QValueAxis *axisY= new QValueAxis;
    axisY->setTitleText("revenue");
   // axisY->setLabelFormat()


    chart->setAxisX(axisX,myseries);
    chart->setAxisY(axisY,myseries);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);//底部对齐
    chart->legend()->setBackgroundVisible(true);//设置背景是否可视
    chart->legend()->setAutoFillBackground(true);//设置背景自动填充
    chart->legend()->setColor(QColor(222,233,251));//设置颜色
    chart->legend()->setLabelColor(QColor(0,100,255));//设置标签颜色
    chart->legend()->setMaximumHeight(50);

    chartwidget->setChart(chart);
    chartwidget->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::paintempty()
{
    chart=new QChart;
    chartwidget->setChart(chart);
    //if(chart)chart->legend()->setVisible(false);
    //chartwidget->setChart(chart);
}

void MainWindow::showprediction(int locate)
{
    QVector<int> value;
    for(int i=0;i<100;++i)
    {
        value.append(0);
    }
    //show prediction according to the locate;
    for(int i=0;i<day.size();++i)
    {
        if(dplace[i].grid_id==locate)
        {
            ++value[aplace[i].grid_id];
        }
    }
    int index=0;
    int max=value[0];
    for(int i=0;i<99;++i)
    {
        if(value[i]>max)
        {
            max=value[i];
            index=i;
        }
    }
    value[index]=0;
    result1->setText("grid_id:"+QString::number(index)+"  "+myroad[index]);
    index=0;
    max=0;
    for(int i=0;i<99;++i)
    {
        if(value[i]>max)
        {
            max=value[i];
            index=i;
        }
    }
    result2->setText("grid_id:"+QString::number(index)+"  "+myroad[index]);
    value[index]=0;
    index=0;
    max=0;
    for(int i=0;i<99;++i)
    {
        if(value[i]>max)
        {
            max=value[i];
            index=i;
        }
    }
    result3->setText("grid_id:"+QString::number(index)+"  "+myroad[index]);
    //ready to add the particular places!
}

void MainWindow::switchwindow()
{
    QWidget *mywid;
    QCustomPlot *myplot;

    if(state==0)//from chart to picture;
    {
        clickbutton->setText("prediction");
        QLayoutItem *child;
        while((child=vlayer4->takeAt(0))!=0)
        {
            if(child->widget())
            {
                child->widget()->setParent(NULL);
            }
            delete child;
        }
        if(dplace.size()==0)
        {
            QMessageBox::information(NULL,"ERROR","No enough data!");
        }
        else{
            mywid=new QWidget;
            vlayer4->addSpacerItem(new QSpacerItem(200,70));
            vlayer4->addWidget(mywid);
            myplot=new QCustomPlot(mywid);
            // configure axis rect:
            myplot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
            myplot->axisRect()->setupFullAxesBox(true);
            myplot->xAxis->setLabel("longitude");
            myplot->yAxis->setLabel("latitude");

            // set up the QCPColorMap:
            QCPColorMap *colorMap = new QCPColorMap(myplot->xAxis, myplot->yAxis);
            int nx = 10;
            int ny = 10;
            colorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
            colorMap->data()->setRange(QCPRange(0,9), QCPRange(0,9)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
            // now we assign some data, by accessing the QCPColorMapData instance of the color map:
            double x, y, z;
            int grid;
            z=0;
            for (int xIndex=0; xIndex<nx; ++xIndex)
            {
              for (int yIndex=0; yIndex<ny; ++yIndex)
              {
                  z=0;
                colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
                //double r = 3*qSqrt(x*x+y*y)+1e-2;
                //z = 2*x*(qCos(r+2)/r-qSin(r+2)/r);
                // the B field strength of dipole radiation (modulo physical constants)
                grid=int(xIndex+10*yIndex);
                for(int i=0;i<day.size();++i)
                {
                    if(dplace[i].grid_id==grid)
                    {
                        ++z;
                    }
                }
                colorMap->data()->setCell(xIndex, yIndex, z);
              }
            }

            // add a color scale:
            QCPColorScale *colorScale = new QCPColorScale(myplot);
            myplot->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
            colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
            colorMap->setColorScale(colorScale); // associate the color map with the color scale
            colorScale->axis()->setLabel("Magnetic Field Strength");

            // set the color gradient of the color map to one of the presets:
            colorMap->setGradient(QCPColorGradient::gpPolar);
            // we could have also created a QCPColorGradient instance and added own colors to
            // the gradient, see the documentation of QCPColorGradient for what's possible.

            // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
            colorMap->rescaleDataRange();

            // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
            QCPMarginGroup *marginGroup = new QCPMarginGroup(myplot);
            myplot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
            colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

            // rescale the key (x) and value (y) axes so the whole color map is visible:
           myplot->rescaleAxes();
           myplot->resize(500,500);
        }
    }

    if(state==1)//from picture to prediction;
    {
        clickbutton->setText("chart");
        QLayoutItem *child;
        while((child=vlayer4->takeAt(0))!=0)
        {
            if(child->widget())
            {
                child->widget()->setParent(NULL);
            }
            delete child;
        }
        //begin to prediction
        //all the buttons are on the vlayer4
        if(dplace.size()==0||aplace.size()==0)
        {
            QMessageBox::information(NULL,"ERROR","No enough data!");
        }
        else
        {
        predictionlabel=new QLabel("please select your original places in below picture:");
        vlayer4->addWidget(predictionlabel);
        mainForm *mymainform;
        mymainform=new mainForm;
        vlayer4->addWidget(mymainform);
        topredictionlabel=new QLabel("you may want to go to:");
        vlayer4->addWidget(topredictionlabel);
        //predictmap=new Widget;
        //vlayer4->addWidget(predictmap);
        result1= new QLabel;
        result2=new QLabel;
        result3=new QLabel;
        vlayer4->addWidget(result1);
        vlayer4->addWidget(result2);
        vlayer4->addWidget(result3);
        vlayer4->setStretch(0,1);
        vlayer4->setStretch(1,6);
        vlayer4->setStretch(2,1);
        vlayer4->setStretch(3,1);
        vlayer4->setStretch(4,1);
        vlayer4->setStretch(5,1);
        connect(mymainform,SIGNAL(repainting(int)),this,SLOT(showprediction(int)));
        }
    }


    if(state==2)//from prediction to chart
    {
        clickbutton->setText("picture");
        QLayoutItem *child;
        while((child=vlayer4->takeAt(0))!=0)
        {
            if(child->widget())
            {
                child->widget()->setParent(NULL);
            }
            delete child;
        }
        //clearwidget();
        viewcombobox=new QComboBox;
        viewcombobox->addItem("none");
        viewcombobox->addItem("number-day");
        viewcombobox->addItem("number-day(depature grid)");
        viewcombobox->addItem("number-day(arrival grid)");
        viewcombobox->addItem("distribution of travel time");
        viewcombobox->addItem("distribution of fee");
        viewcombobox->addItem("revenue line chart");
        viewcombobox->addItem("revenue histogram");

        chartwidget=new QChartView;
        hlayer4=new QHBoxLayout;
        hlayer5=new QHBoxLayout;
        vlayer4->addWidget(viewcombobox);
        vlayer4->addLayout(hlayer4);
        vlayer4->addLayout(hlayer5);
        vlayer4->addWidget(chartwidget);

        connect(viewcombobox,SIGNAL(currentIndexChanged(int)),this,SLOT(paint(int)));
    }


    state=(state+1)%3;
}

QVector<double> labelPositions(const QVector<QString> &labels, double offset = 0)
{
    QVector<double> result(labels.size());
    for (int i = 0; i < labels.size(); ++i)
        result[i] = i + offset;
    return result;
}

void MainWindow::setupHeatmapDemo(QCustomPlot *customPlot)
{

        QVector<QString> hours = {"12a", "1a", "2a", "3a", "4a", "5a", "6a",
                "7a", "8a", "9a","10a","11a",
                "12p", "1p", "2p", "3p", "4p", "5p",
                "6p", "7p", "8p", "9p", "10p", "11p"};
        QVector<QString> days = {"Saturday", "Friday", "Thursday",
                "Wednesday", "Tuesday", "Monday", "Sunday"};

        QVector<QVector<double>> data = {{0,0,5},{0,1,1},{0,2,0},{0,3,0},{0,4,0},{0,5,0},{0,6,0},{0,7,0},{0,8,0},{0,9,0},{0,10,0},{0,11,2},{0,12,4},{0,13,1},{0,14,1},{0,15,3},{0,16,4},{0,17,6},{0,18,4},
                                {0,19,4},{0,20,3},{0,21,3},{0,22,2},{0,23,5},{1,0,7},{1,1,0},{1,2,0},{1,3,0},{1,4,0},{1,5,0},{1,6,0},{1,7,0},{1,8,0},{1,9,0},{1,10,5},{1,11,2},{1,12,2},{1,13,6},{1,14,9},{1,15,11},
                                {1,16,6},{1,17,7},{1,18,8},{1,19,12},{1,20,5},{1,21,5},{1,22,7},{1,23,2},{2,0,1},{2,1,1},{2,2,0},{2,3,0},{2,4,0},{2,5,0},{2,6,0},{2,7,0},{2,8,0},{2,9,0},{2,10,3},{2,11,2},{2,12,1},
                                {2,13,9},{2,14,8},{2,15,10},{2,16,6},{2,17,5},{2,18,5},{2,19,5},{2,20,7},{2,21,4},{2,22,2},{2,23,4},{3,0,7},{3,1,3},{3,2,0},{3,3,0},{3,4,0},{3,5,0},{3,6,0},{3,7,0},{3,8,1},
                                {3,9,0},{3,10,5},{3,11,4},{3,12,7},{3,13,14},{3,14,13},{3,15,12},{3,16,9},{3,17,5},{3,18,5},{3,19,10},{3,20,6},{3,21,4},{3,22,4},{3,23,1},{4,0,1},{4,1,3},{4,2,0},{4,3,0},{4,4,0},
                                {4,5,1},{4,6,0},{4,7,0},{4,8,0},{4,9,2},{4,10,4},{4,11,4},{4,12,2},{4,13,4},{4,14,4},{4,15,14},{4,16,12},{4,17,1},{4,18,8},{4,19,5},{4,20,3},{4,21,7},{4,22,3},{4,23,0},{5,0,2},
                                {5,1,1},{5,2,0},{5,3,3},{5,4,0},{5,5,0},{5,6,0},{5,7,0},{5,8,2},{5,9,0},{5,10,4},{5,11,1},{5,12,5},{5,13,10},{5,14,5},{5,15,7},{5,16,11},{5,17,6},{5,18,0},{5,19,5},{5,20,3},{5,21,4},
                                {5,22,2},{5,23,0},{6,0,1},{6,1,0},{6,2,0},{6,3,0},{6,4,0},{6,5,0},{6,6,0},{6,7,0},{6,8,0},{6,9,0},{6,10,1},{6,11,0},{6,12,2},{6,13,1},{6,14,3},{6,15,4},{6,16,0},{6,17,0},{6,18,0},
                                {6,19,0},{6,20,1},{6,21,2},{6,22,2},{6,23,6}};

        QCPColorMap *heatmap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);  // 构造一个颜色图
        heatmap->data()->setSize(hours.size(), days.size());   // 设置颜色图数据维度，其内部维护着一个一维数组（一般表现为二维数组），这里可以理解为有多少个小方块
        heatmap->data()->setRange(QCPRange(0.5, hours.size() - 0.5), QCPRange(0.5, days.size() - 0.5));  // 颜色图在x、y轴上的范围

        // 设置轴的显示，这里使用文字轴，如果这部分还不会的请看 QCustomPlot之个性化外观（二）这章节
        QSharedPointer<QCPAxisTickerText> xTicker(new QCPAxisTickerText);
        QSharedPointer<QCPAxisTickerText> yTicker(new QCPAxisTickerText);
        xTicker->setTicks(labelPositions(hours, 0.5), hours);
        yTicker->setTicks(labelPositions(days, 0.5), days);
        xTicker->setSubTickCount(1);
        yTicker->setSubTickCount(1);
        customPlot->xAxis->setTicker(xTicker);
        customPlot->yAxis->setTicker(yTicker);
        customPlot->xAxis->grid()->setPen(Qt::NoPen);
        customPlot->yAxis->grid()->setPen(Qt::NoPen);
        customPlot->xAxis->grid()->setSubGridVisible(true);
        customPlot->yAxis->grid()->setSubGridVisible(true);
        customPlot->xAxis->setSubTicks(true);
        customPlot->yAxis->setSubTicks(true);
        customPlot->xAxis->setTickLength(0);
        customPlot->yAxis->setTickLength(0);
        customPlot->xAxis->setSubTickLength(6);
        customPlot->yAxis->setSubTickLength(6);
        customPlot->xAxis->setRange(0, hours.size());
        customPlot->yAxis->setRange(0, days.size());

        for (int x = 0; x < hours.size(); ++x) {
            for (int y = 0; y < days.size(); ++y) {
                int z = data.at(hours.size() * y + x).at(2);
                if (z) heatmap->data()->setCell(x, y, z);     // 如果z不为0，则设置颜色值的位置
                else heatmap->data()->setAlpha(x, y, 0);  // z为0，设置为透明
            }
        }

        QCPColorScale *colorScale = new QCPColorScale(customPlot);  // 构造一个色条
        colorScale->setType(QCPAxis::atBottom);   // 水平显示
        customPlot->plotLayout()->addElement(1, 0, colorScale); // 在颜色图下面显示
        heatmap->setColorScale(colorScale);
        QCPColorGradient gradient;  // 色条使用的颜色渐变
        gradient.setColorStopAt(0.0, QColor("#f6efa6"));   // 设置色条开始时的颜色
        gradient.setColorStopAt(1.0, QColor("#bf444c"));  // 设置色条结束时的颜色
        heatmap->setGradient(gradient);
    //    colorMap->rescaleDataRange();        // 自动计算数据范围，数据范围决定了哪些数据值映射到QCPColorGradient的颜色渐变当中
        heatmap->setDataRange(QCPRange(0, 10));     // 为了保持与echart的例子一致，我们这里手动设置数据范围
        heatmap->setInterpolate(false);         // 为了显示小方块，我们禁用插值

        // 保持色条与轴矩形边距一致
        QCPMarginGroup *marginGroup = new QCPMarginGroup(customPlot);
        customPlot->axisRect()->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);
        colorScale->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);

}


