#include "mainForm.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <QDebug>

mainForm::mainForm(QWidget*parent)
{
    myPixmapLabel=new QLabel(this);
    myPixmapLabel->setGeometry(QRect(120,60,300,270));
    QImage myimage(QString(":/map2.png"));
    //image.scaled(50,50);
    //mymap=new QLabel;
    //mymap->setPixmap(QPixmap::fromImage(image));
    //mymap->resize(50,50);
    myPixmapLabel->setPixmap(QPixmap::fromImage(myimage));

    myPixmapLabel->setScaledContents(true);

    //languageChange();
    resize(QSize(600,600).expandedTo(minimumSizeHint()));
    connect(this,SIGNAL(clicked(int)),this,SLOT(mousePressEventSlot(int)));

}

mainForm::~mainForm()
{

}

void mainForm::mousePressEvent(QMouseEvent *e)
{
    int grid=0;
    int x=e->x();
    int y=e->y();
    qDebug()<<x;
    qDebug()<<y;

    if(x>153&&x<375&&y>78&&y<300)
    {
        while(1)
        {
            if(y>=278)
            {
                grid+=0;break;
            }
            if(y>=257)
            {
                grid+=10;break;
            }
            if(y>=234)
            {
                grid+=20;break;
            }
            if(y>=211)
            {
                grid+=30;break;
            }
            if(y>=189)
            {
                grid+=40;break;
            }
            if(y>=167)
            {
                grid+=50;break;
            }
            if(y>=144)
            {
                grid+=60;break;
            }
            if(y>=122)
            {
                grid+=70;break;
            }
            if(y>=99)
            {
                grid+=80;break;
            }

                grid+=90;break;

        }
        while(1)
        {
            if(x<=175)
            {
                grid+=0;break;
            }
            if(x<=197)
            {
                grid+=1;break;
            }
            if(x<=220)
            {
                grid+=2;break;
            }
            if(x<=242)
            {
                grid+=3;break;
            }
            if(x<=264)
            {
                grid+=4;break;
            }
            if(x<=286)
            {
                grid+=5;break;
            }
            if(x<=308)
            {
                grid+=6;break;
            }
            if(x<=331)
            {
                grid+=7;break;
            }
            if(x<=353)
            {
                grid+=8;break;
            }

                grid+=9;break;

        }
        emit clicked(grid);
    }
}

void mainForm::mousePressEventSlot(int x)
{
    //QMessageBox::about(this,"QT MOUSE!","YOU HAVE CLICKED!");
    //close();
    qDebug()<<"clicked!";
    emit repainting(x);
}

Widget::Widget(QWidget* parent)
{
    mymaplabel=new QLabel(this);
    mymaplabel->setGeometry(QRect(200,0,400,200));
    QImage myimage(QString(":/map2.png"));
    //image.scaled(50,50);
    //mymap=new QLabel;
    //mymap->setPixmap(QPixmap::fromImage(image));
    //mymap->resize(50,50);
    mymaplabel->setPixmap(QPixmap::fromImage(myimage));
    mymaplabel->setScaledContents(true);
}

Widget::~Widget()
{

}

void Widget::repainting()
{
    /*qDebug()<<"repaingting!";
    delete mymaplabel;
    mymaplabel=new QLabel(this);
    mymaplabel->setGeometry(QRect(120,60,300,270));
    QImage myimage(QString(":/map2.png"));
    //image.scaled(50,50);
    //mymap=new QLabel;
    //mymap->setPixmap(QPixmap::fromImage(image));
    //mymap->resize(50,50);
    mymaplabel->setPixmap(QPixmap::fromImage(myimage));
    mymaplabel->setScaledContents(true);
    QPen mypen;*/
    QPainter p(this);
    QPen pen;

    pen.setColor(QColor(0,255,255));
    pen.setWidth(10);
    pen.setStyle(Qt::DotLine);

    p.drawPixmap(0,0,QPixmap(":/map2.png"));
    p.drawLine(QPoint(0,0),QPoint(100,100));

}
