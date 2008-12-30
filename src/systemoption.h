#ifndef SYSTEM_OPTION_H
#define SYSTEM_OPTION_H


#include <QtCore>
#include <QtNetwork>

#include "modules/feed_record.h"

class Account;

class SystemOption : public QObject
{
    Q_OBJECT;
public:
    SystemOption(QObject *parent = 0);
    ~SystemOption();

    bool     loadSymbol(const char *name, SiteConst **sc);
    SiteConst *getSite(int siteId);

public:    
    enum {BS_MIN, BS_IE, BS_OPERA, BS_FIREFOX, BS_KONQUER, BS_MAX };
    int updateIntval;
    int historyLifeTime;
    int numOnePage;
    QVector<int> browserList; // 根据不同的系统初始化不同的顺序。

    QTextCodec *codec ;

    QString appPath;
    QString iconPath;  // application icons path
    QString dotPath; //application configuration path
    QString cachePath;  // cache icons tempory downloaded from net
    QString modulePath; // default module .so path

private:
    QMap<int, SiteConst*> siteMap;
    QMap<int, QString> modules;
};

//
extern SystemOption * so;

#endif

