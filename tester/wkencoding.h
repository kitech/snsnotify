#ifndef MYHANDLE_H
#define MYHANDLE_H

#include <QtCore>

class MyHandler : public QObject
{
    Q_OBJECT;
public:
    MyHandler():QObject(0) {

    }
    ~MyHandler() {

    }
public slots:
    void finished(bool);
};

#endif
