
#include "cookieparser.h"

CookieParser::CookieParser(QObject *parent)
    : QObject(parent)
{
    
}
CookieParser::~CookieParser()
{
    
}

QList<QPair<QString, QString> > CookieParser::getAllCookies(int browser, QString path)
{
    QList<QPair<QString, QString> > cookies;

    return cookies;
}

QString CookieParser::toCookieString(QString name, QString value, QString host,
                       QString path, QString expires)
{
    QString ck;
    QDateTime date = QDateTime::fromTime_t(expires.toULong());

    ck = CookieParser::toCookieString(name, value, host, path, date);

    return ck;
}

QString CookieParser::toCookieString(QString name, QString value, QString host,
                                  QString path, QDateTime expires)
{
    QLocale locale("C");
    QString ck;

    //"l=7R2IeWxKxeVdd7d0gNrSzvWhzzSc3CLM; expires=Sat, 20-Sep-2008 10:55:54 GMT; path=/; domain=.hainei.com"
    //name=value; expires=xxx; path=path; domain=host
    // ck = QString("%1=%2; path=%3; domain=%4; expires=%5")
    //     .arg(name).arg(value).arg(path).arg(host)
    //     .arg(locale.toString(expires, "ddd, dd-MMM-yyyy hh:mm:ss") + " GMT");
    //the upper code maybe error when the param contains '%'

    ck += name + "=" + value + "; " ;
    ck += QString("path") + "=" + path + "; " ;
    ck += QString("domain") + "=" + host + "; " ;
    ck += QString("expires") + "=" + (locale.toString(expires, "ddd, dd-MMM-yyyy hh:mm:ss") + " GMT")  + "; " ;

    return ck;
    
}
