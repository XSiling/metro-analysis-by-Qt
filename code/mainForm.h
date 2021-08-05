#ifndef MAINFORM_H
#define MAINFORM_H

#include <qevent.h>
#include <qvariant.h>
#include <qwidget.h>
#include <qmessagebox.h>
#include <QPen>
#include <QPainter>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;

class mainForm:public QWidget
{
    Q_OBJECT

public:
    mainForm(QWidget* parent=NULL);
    ~mainForm();

    QLabel* myPixmapLabel;

signals:
    void clicked(int);
    void repainting(int);

public slots:
    virtual void mousePressEventSlot(int);

protected:
    void mousePressEvent(QMouseEvent*);

protected slots:
   // virtual void languageChange();

};

class Widget:public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget* parent=NULL);
    ~Widget();
    QLabel *mymaplabel;
public slots:
    void repainting();
};

#endif // MAINFORM_H
