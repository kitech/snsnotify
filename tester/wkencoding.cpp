#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QtWebKit>

#include "wkencoding.h"

void MyHandler::finished(bool)
{
    QWebPage *wp = (QWebPage*)sender();
    qDebug()<<"ha";

    //delete wp;
    wp->deleteLater();
    //wp = NULL;
}


int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    MyHandler *mh = new MyHandler();
    QWebPage *wp = new QWebPage();
    wp->mainFrame()->load(QUrl("http://u.discuz.net/home/"));
    QObject::connect(wp, SIGNAL(loadFinished(bool)), mh, SLOT(finished(bool)));

    return a.exec();
}
