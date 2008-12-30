#include "utils.h"

#include "cookieparser.h"

//linux
/*
  gzl@localhost ~/snsnotify/src $ more /home/gzl/.mozilla/firefox/profiles.ini
  [General]
  StartWithLastProfile=1
  
  [Profile0]
  Name=default
  IsRelative=1
  Path=f8gsrpy4.default
 */

//windows
/*
  C:\Documents and Settings\Administrator\Application Data\Mozilla\Firefox\profiles.ini
  [General]
  StartWithLastProfile=1
  
  [Profile0]
  Name=default
  IsRelative=1
  Path=Profiles/3cmvbdf1.default

 */
//TODO Profile maybe has more than one, and need consider IsRealtive's value
static QString getFireFoxProfileDataPath()
{
    QString mozPath;
    QString mozProfileDataPath;
    QString mozProfileINIPath ;
#ifdef Q_OS_WIN
    mozPath = QDir::homePath() + "/Application Data/Mozilla/Firefox";
#else
    mozPath = QDir::homePath() + "/.mozilla/firefox";    
#endif

    mozProfileINIPath = mozPath + "/profiles.ini";    
    QSettings settings(mozProfileINIPath, QSettings::IniFormat);
    
    qDebug()<<settings.allKeys();
    QStringList keys = settings.allKeys();
    
    QString tmpPath = settings.value("Profile0/Path").toString();
    mozProfileDataPath = mozPath + "/" + tmpPath;
    
    return mozProfileDataPath;
}

//ff3.0.1
QList<QPair<QString, QString> > CookieParser::getFireFoxCookies(QString path)
{
    QList<QPair<QString, QString> > cookies;
    QList<QNetworkCookie> tcookies;
    QString cs;

    tcookies = CookieParser::getFireFoxCookiesEx(path);

    for(int i = 0 ; i < tcookies.count(); i++) {
        cs = CookieParser::toCookieString(tcookies.at(i).name(), 
                                          tcookies.at(i).value(), 
                                          tcookies.at(i).domain(), 
                                          tcookies.at(i).path(), 
                                          tcookies.at(i).expirationDate());
        cookies.append(QPair<QString,QString>("Cookie", cs));
    }

    return cookies;
}
    
QList<QNetworkCookie> CookieParser::getFireFoxCookiesEx(QString path)
{
    QList<QNetworkCookie> cookies;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "cookies");

    QString dataPath = getFireFoxProfileDataPath();
    qDebug()<<dataPath;
    QString cookieFile = dataPath + "/cookies.sqlite";
    QString sql = "select * from moz_cookies";
    QSqlRecord rec;

    db.setDatabaseName(cookieFile);
    if(!db.open()) {
        q_debug()<<"open db error"<<db.lastError().text();
    }
    
    QSqlQuery query(db);
    if(!query.exec(sql)) {
        q_debug()<<"query error"<<db.lastError().text();
    }else{
        q_debug()<<"row:"<<query.size();
        while (query.next()) {
            //QString country = query.value(0).toString();
            //qDebug()<<country;
            //rec = query.record();
            //qDebug()<<rec;
            //qDebug()<<rec.count();
            QString id = query.value(0).toString();
            QString name= query.value(1).toString();
            QString value = query.value(2).toString();
            QString host = query.value(3).toString();
            QString ckpath = query.value(4).toString();
            QString expires = query.value(5).toString();

            //qDebug()<<name<<value<<host<<ckpath<<expir;
            //QString ck = CookieParser::toCookieString(name, value, host, ckpath, expires);
            //qDebug()<<ck;
            //cookies.append(QPair<QString, QString>("Cookie", ck));
            QNetworkCookie ck;
            ck.setName(name.toAscii());
            ck.setValue(value.toAscii());
            ck.setPath(ckpath);
            ck.setDomain(host);
            ck.setExpirationDate(QDateTime::fromTime_t(expires.toULong()));

            cookies.append(ck);
        }
    }
    
    if(db.isOpen()) {
        db.close();
    }

    return cookies;
}

QList<QPair<QString, QString> > CookieParser::getFireFoxCookiesByHost(QString host, QString path)
{
    QList<QPair<QString, QString> > cookies;
    QList<QNetworkCookie> tcookies = CookieParser::getFireFoxCookiesEx(path);
    QNetworkCookie nc;
    QString cs;

    q_debug()<<"cookie count:"<<tcookies.count();
    for(int i = 0; i < tcookies.count(); i++) {
        nc = tcookies.at(i);
        if(host.endsWith(nc.domain())) {
            cs = CookieParser::toCookieString(nc.name(), nc.value(),
                                        nc.domain(), nc.path(), nc.expirationDate());
            cookies.append(QPair<QString,QString>("Cookie", cs));
        }
    }
    
    return cookies;
}

QList<QNetworkCookie> CookieParser::getFireFoxCookiesByHostEx(QString host, QString path)
{
    QList<QNetworkCookie> cookies;
    QList<QNetworkCookie> tcookies = CookieParser::getFireFoxCookiesEx(path);
    QNetworkCookie nc;
    QString cs;

    q_debug()<<"cookie count:"<<tcookies.count();
    for(int i = 0; i < tcookies.count(); i++) {
        nc = tcookies.at(i);
        if(host.endsWith(nc.domain())) {
            cookies.append(nc);
        }
    }
    
    return cookies;
}

