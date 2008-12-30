#include "utils.h"
#include "cookieparser.h"

///home/gzl/.ies4linux/ie6/drive_c/windows/profiles/gzl/Cookies/gzl@u.discuz[1].txt

QString CookieParser::getLogName()
{
    QString user ;

#ifdef Q_OS_WIN
    //TODO
    user = "";
    user = QString(getenv("USERNAME"));
#else
    user = QString(getenv("LOGNAME"));
#endif

    return user;
}

static QString getIECookiePath()
{
    QString home = QDir::homePath();
    QString user = CookieParser::getLogName();
    QString path ;
    QString fmt;

#ifdef Q_OS_WIN
    //C:\Documents and Settings\Administrator\Cookies
    path = home + "/Cookies/";
#else
    fmt = "%1/.ies4linux/ie6/drive_c/windows/profiles/%2/Cookies/";
    path = QString(fmt).arg(home).arg(user);
#endif
    //q_debug()<<user<<path;

    return path;
}

static QString getIECookieFileName(QString host, int which = -1)
{
    QString fname, fname2;
    QString path = getIECookiePath();
    QString hh = host;
    QString fmt = "%1@%2[%3].txt";
    QString tmp;
    QString user = CookieParser::getLogName();
    int lastDotPos = -1;

    if(hh.startsWith("www.")) {
        hh = hh.right(hh.length()-4);
    }
    if((lastDotPos = hh.lastIndexOf(".")) != -1) {
        hh = hh.left(lastDotPos-1);
    }
    q_debug()<<hh;
    
    fname = QString(fmt).arg(user).arg(hh).arg("1");
    fname2 = QString(fmt).arg(user).arg(hh).arg("2");

    if(QFile::exists(fname)) return fname;
    else if(QFile::exists(fname2)) return fname2;
    else{
        q_debug()<<"can not find it's cookie file";
    }
    return QString();
}

static QList<QNetworkCookie> parse_ie_cookie_string(QString &str)
{
    QList<QNetworkCookie> cookies;
    QStringList ckblock = str.split("*");
    QStringList ckelem;
    
    for(int i = 0 ; i < ckblock.count(); i++) {
        if(ckblock.at(i).trimmed().length() == 0) continue;
        ckelem = ckblock.at(i).trimmed().split("\n");
        if(ckelem.count() == 0) continue;
        //q_debug()<<ckelem;
        QNetworkCookie cookie;
        cookie.setName(ckelem.at(0).toAscii());
        cookie.setValue(ckelem.at(1).toAscii());
        cookie.setDomain(ckelem.at(2).left(ckelem.at(2).length()-1));        //drop last '/'
        cookie.setPath("/");
        QDateTime date = QDateTime::fromTime_t(ckelem.at(4).trimmed().toULong());
        cookie.setExpirationDate(date);

        cookies.append(cookie);
    }

    return cookies;
}

QList<QPair<QString, QString> > CookieParser::getIECookies(QString path)
{
    QList<QPair<QString, QString> > cookies;
    QList<QNetworkCookie> tcookies = CookieParser::getIECookiesEx(path);
    QNetworkCookie nc;
    QString ckstr;

    for(int i = 0 ; i < tcookies.count(); i++) {
        nc = tcookies.at(i);
        ckstr = CookieParser::toCookieString(nc.name().data(), nc.value().data(),
                                             nc.domain(), nc.path(), nc.expirationDate());
        cookies.append(QPair<QString,QString>("Cookie", ckstr));
    }

    return cookies;
}

QList<QNetworkCookie> CookieParser::getIECookiesEx(QString path)
{
    QList<QNetworkCookie> cookies;
    QString apath = getIECookiePath();
    QStringList fltlist;
    fltlist<<"*.txt";
    QStringList flist = QDir(apath).entryList(fltlist);
    int fc = flist.count();

    //q_debug()<<flist;
    
    QList<QNetworkCookie> ack;
    QString tmp;
    QByteArray ba;
    QFile fh;
    for(int i = 0 ; i < fc ; i++) {
        tmp = apath + "/" + flist.at(i);
        fh.setFileName(tmp);
        fh.open(QIODevice::ReadOnly);
        ba = fh.readAll();
        fh.close();
        
        tmp = QString(ba.data()).trimmed();
        ack = parse_ie_cookie_string(tmp); 
        
        for(int j=0; j < ack.count(); j++) {
            cookies.append(ack.at(j));
        }
    }

    return cookies;
}

QList<QPair<QString, QString> > CookieParser::getIECookiesByHost(QString host, QString path)
{
    QList<QPair<QString, QString> > cookies;
    QList<QNetworkCookie> tcookies;
    QNetworkCookie nc;
    QString ckstr;

    tcookies = CookieParser::getIECookiesByHostEx(host, path);
    for(int i = 0 ; i < tcookies.count(); i++) {
        nc = tcookies.at(i);
        ckstr = CookieParser::toCookieString(nc.name().data(), nc.value().data(),
                                             nc.domain(), nc.path(), nc.expirationDate());
        cookies.append(QPair<QString,QString>("Cookie", ckstr));
    }

    return cookies;
}

QList<QNetworkCookie> CookieParser::getIECookiesByHostEx(QString host, QString path)
{
    QList<QNetworkCookie> cookies;
    QList<QNetworkCookie> tcookies;
    QNetworkCookie nc;
    
    tcookies = CookieParser::getIECookiesEx(path);
    for(int i = 0; i < tcookies.count(); i ++) {
        nc = tcookies.at(i);
        if(host.endsWith(nc.domain())) {
            cookies.append(nc);
        }
    }

    return cookies;    
}








