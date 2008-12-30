#ifndef THREAD_FUNCTOR_H
#define THREAD_FUNCTOR_H


#include <QtCore>
#include <QtNetwork>

/*
  used for functor's object garbage 
  no use at other file
  only one file scope 
  global instance 
 */

class FunctorGarbage : public QObject
{
    Q_OBJECT;
public:
    FunctorGarbage(QObject *parent = 0) {}
    ~FunctorGarbage() {}

public slots:
    void fetchDone(bool error);
};

int functorFetchIcon(QString url);

#endif
