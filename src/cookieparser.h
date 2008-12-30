#ifndef COOKIE_PARSER_H
#define COOKIE_PARSER_H

#include <QtCore>
#include <QtSql>
#include <QtNetwork>

//parser cookies for web host from browser cookie storage
//IE  windows
//Opera   windows , linux, unix
//FireFox  windows, linux, unix


class CookieParser : public QObject
{
    Q_OBJECT;
public:
    enum {BROWSER_IE, BROWSER_OPERA, BROWSER_FIREFOX};
    CookieParser(QObject *parent = 0);
    ~CookieParser();

    static QString getLogName();
    
    static QString toCookieString(QString name, QString value, QString host,
                                  QString path, QString expires);

    static QString toCookieString(QString name, QString value, QString host,
                                  QString path, QDateTime expires);

    static QList<QPair<QString, QString> > getAllCookies(int browser, QString path="");

    static QList<QPair<QString, QString> > getIECookies(QString path="");
    static QList<QPair<QString, QString> > getOperaCookies(QString path="");
    static QList<QPair<QString, QString> > getFireFoxCookies(QString path="");

    static QList<QNetworkCookie> getIECookiesEx(QString path="");
    static QList<QNetworkCookie> getOperaCookiesEx(QString path="");
    static QList<QNetworkCookie> getFireFoxCookiesEx(QString path="");
    
    static QList<QPair<QString, QString> > getIECookiesByHost(QString host, QString path="");
    static QList<QNetworkCookie> getIECookiesByHostEx(QString host, QString path="");
    static QList<QPair<QString, QString> > getOperaCookiesByHost(QString host, QString path="");
    static QList<QPair<QString, QString> > getFireFoxCookiesByHost(QString host, QString path="");
    static QList<QNetworkCookie> getFireFoxCookiesByHostEx(QString host, QString path="");

    //chrome
    static  QList<QPair<QString, QString> > getChromeCookies(QString path);
    static QList<QNetworkCookie> getChromeCookiesEx(QString path);
    static QList<QPair<QString, QString> > getChromeCookiesByHost(QString host, QString path);
    static QList<QNetworkCookie> getChromeCookiesByHostEx(QString host, QString path);
};

#endif
