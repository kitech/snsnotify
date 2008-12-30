#include "utils.h"

#include "cookieparser.h"

//linux
/*

 */

//windows
/*


  cookie为sqlite的

  文件名为 cookies

  所在目录：C:\Documents and Settings\Administrator\Local Settings\Application Data\Google\Chrome\User Data\Default\

CREATE TABLE cookies (creation_utc INTEGER NOT NULL UNIQUE PRIMARY KEY,host_key TEXT NOT NULL,name TEXT NOT NULL,value TEXT NOT NULL,path TEXT NOT NULL,expires_utc INTEGER NOT NULL,secure INTEGER NOT NULL,httponly INTEGER NOT NULL)

"12865238132270376",".tools.google.com","__utmz","21104190.1220764532.1.1.utmcsr=(direct)|utmccn=(direct)|utmcmd=(none)","/chrome","12881006132000000","0","0"
"12865238132274376",".tools.google.com","__utma","21104190.3722306479918640000.1220764532.1220764532.1220764532.1","/chrome","12928310132000000","0","0"

creation_utc:  记录创建时间
host_key:     domain 字段
name:          cookie名字段
value  :      cookie值字段
path   :      
expires_utc:   过期时间
secure  :    cookie 安全性
httponly: 

*/
//TODO Profile maybe has more than one, and need consider IsRealtive's value
static QString getChromeProfileDataPath()
{
    QString mozPath;
    QString mozProfileDataPath;
    QString mozProfileINIPath ;
#ifdef Q_OS_WIN
    mozPath = QDir::homePath() + "/Local Settings/Application Data/Google/Chrome/User Data/Default/";
#else
    Q_ASSERT(1 == 2);
    //maybe ~/.wine/drive_c/windows/profiles/gzl/Local\ Settings/Application\ Data/Google/
     mozPath = QDir::homePath() + "/.wine/drive_c/windows/profiles/%1/Local Settings/Application Data/Google/";
     mozPath = mozPath.arg(CookieParser::getLogName());
#endif

    return mozPath;
    // mozProfileINIPath = mozPath + "/profiles.ini";    
    // QSettings settings(mozProfileINIPath, QSettings::IniFormat);
    
    // qDebug()<<settings.allKeys();
    // QStringList keys = settings.allKeys();
    
    // QString tmpPath = settings.value("Profile0/Path").toString();
    // mozProfileDataPath = mozPath + "/" + tmpPath;
    
    // return mozProfileDataPath;
}

//chrome
QList<QPair<QString, QString> > CookieParser::getChromeCookies(QString path)
{
    QList<QPair<QString, QString> > cookies;
    QList<QNetworkCookie> tcookies;
    QString cs;

    tcookies = CookieParser::getChromeCookiesEx(path);

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
    
QList<QNetworkCookie> CookieParser::getChromeCookiesEx(QString path)
{
    QList<QNetworkCookie> cookies;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "chrome_cookies");

    QString dataPath = getChromeProfileDataPath();
    qDebug()<<dataPath;
    QString cookieFile = dataPath + "/cookies";
    QString sql = "select creation_utc,name,value,host_key,path,expires_utc, secure,httponley from cookies";
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

QList<QPair<QString, QString> > CookieParser::getChromeCookiesByHost(QString host, QString path)
{
    QList<QPair<QString, QString> > cookies;
    QList<QNetworkCookie> tcookies = CookieParser::getChromeCookiesEx(path);
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

QList<QNetworkCookie> CookieParser::getChromeCookiesByHostEx(QString host, QString path)
{
    QList<QNetworkCookie> cookies;
    QList<QNetworkCookie> tcookies = CookieParser::getChromeCookiesEx(path);
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

