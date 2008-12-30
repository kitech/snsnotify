#ifndef LIVE_UPDATE_H
#define LIVE_UPDATE_H

#include <QtCore>

#include "qlog.h"

class SNSNotify;

class LiveUpdate : public QThread
{
    Q_OBJECT;

public:
    LiveUpdate(QObject * parent = 0);
    ~LiveUpdate();

    virtual void run();

signals:
    void hasUpdateModulesFound();
    //void modulesDownloadFinished();
    void fetchModule(QString name, int ver);

private:
    bool getLocalModVers();
    bool getRemoteModVers();
    QVector<QString> findNewModoules();
    bool fetchModuleFile(QString name, int ver);

private:
    QString modversUrl;
    QString modBaseUrl;    
    QVector<QString> localModVers;
    QVector<QString> remoteModVers;
    QVector<QString> hasUpdateModules;
    int status;
    QString errorMessage;
    SNSNotify *sns_notify;
};

#endif

