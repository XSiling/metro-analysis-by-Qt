#include "Loading.h"
Loading::Loading(QObject*parent):QObject(parent)
{
    //-- to add something to show it has been started;
}

Loading::~Loading()
{
    //-- to add something to show it has been shut down;
}

void Loading::setday(int day,int part,QString& t)
{
    t="Dataset-CS241-2020/order_201611";
    if(day<10)
    {
        t+="0";
        t+='0'+day;
        t+="_part0.csv";
    }
    else
    {
        t+='1';
        t+='0'+day-10;
        t+="_part0.csv";
    }
    t[38]='0'+part;
}

void Loading::setfilename(QString name)
{
    qDebug()<<name;
    filename=name;
    qDebug()<<filename;
    alreadygetfilename=true;
    //mutexforfile.unlock();
    emit readyloading();
}

void Loading::loading()
{
    QVector<int> dtime,day;
    QVector<site> dplace,aplace;
    QVector<double> fee;
    QString mystring;
    QStringList mystringlist;
    int progress=0;
    QuaZip zipR(filename);
    QString csvfilename;
    setday(begin,0,csvfilename);
    zipR.open(QuaZip::mdUnzip);
    zipR.setCurrentFile(csvfilename);
    //qDebug()<<csvfilename;
    QuaZipFile *fileR;
    fileR=new QuaZipFile(&zipR);

    for(int i=0;i<=(end-begin);++i)
    {
        for(int j=0;j<5;++j)
        {
            mystring="readytoload!";
            setday(begin+i,j,csvfilename);

            //qDebug()<<csvfilename;
            if(zipR.setCurrentFile(csvfilename))
                qDebug()<<"yes!right set!";
            //qDebug()<<zipR.getFileNameList();
            qDebug()<<zipR.getCurrentFileName();
            delete fileR;
            fileR = new QuaZipFile(&zipR);
            fileR->open(QIODevice::ReadOnly);
            qDebug()<<fileR->readLine();
            //begin to read proper data
            //int testi=0;
            while(mystring!="")
            {
                //qDebug()<<"okay"<<testi;
                //++testi;
                //msleep(1);
                mystring=fileR->readLine();
                mystring=mystring.trimmed();
                mystringlist=mystring.split(',');
                if(mystringlist.length()<8)break;
                if(dtimec)dtime.append(mystringlist[2].toInt()-mystringlist[1].toInt());
                //if(atimec)atime.append(mystringlist[2].toInt());
                if(dplacec)dplace.append(site(mystringlist[3].toDouble(),mystringlist[4].toDouble()));
                if(aplacec)aplace.append(site(mystringlist[5].toDouble(),mystringlist[6].toDouble()));
                if(feec)fee.append(mystringlist[7].toDouble());
                day.append(begin+i);

            }
            ++progress;
            emit resetprogressbar(progress);
            //fileloadingProgress->setValue(progress);
        }
    }
    fileR->close();
    zipR.close();
    qDebug()<<day.size();

    emit senddtime(dtime);
    //emit sendatime(atime);
    emit senddplace(dplace);
    emit sendaplace(aplace);
    emit sendfee(fee);
    emit sendday(day);

    emit resetcombo();

    emit sig_finish();
}

void Loading::slot_loading(int beginday,int endday,bool dtimecheck,bool atimecheck,bool dplacecheck,bool aplacecheck,bool feecheck)
{
    alreadygetfilename=false;
    begin=beginday;
    end=endday;
    dtimec=dtimecheck;
    atimec=atimecheck;
    dplacec=dplacecheck;
    aplacec=aplacecheck;
    feec=feecheck;
    //MainWindow *ptr = (MainWindow*)parent();
    if(beginday>endday)
    {
        QMessageBox::information(NULL,"ERROR","You have selected a improper day-range!");
        emit sig_finish();
        return;
    }

    //qDebug()<<ptr->beginday;
    //qDebug()<<ptr->endday;

    //now get the status of whether checked or not;
    //start to loading the file;
    //first a fileDialog to let user select the file.

   /* while(filename=="")
    {
        if(i==0){emit getfilename();}
        ++i;
       // mutexforfile.lock();
    //how to continue the exe after getting all the results?
        qDebug()<<filename;
    //QString filename=QFileDialog::getOpenFileName(0,tr("open a file"),"",tr("packages(*.zip *.rar);;All files(*.*)"));
    }*/

    emit getfilename();
}
