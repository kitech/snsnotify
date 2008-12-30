#include "utils.h"
#include "systemoption.h"
#include "threadfunctor.h"

void FunctorGarbage::fetchDone(bool error)
{
    QHttp *http = static_cast<QHttp*>(sender());
    QFile *fh = static_cast<QFile*>(http->currentDestinationDevice());
    if(error) {
        q_debug()<<http->errorString();
    }
    //fh->close();
    delete fh;
    //http->deleteLater();
    delete http;
    q_debug()<<"free fetch done";
}

static FunctorGarbage fg(0);

/*
  can read txt, html, icon, css, ....
  but fetch cgi's url is not recomanded.
  this function is blocked, should be call at thread, such as QtConcurrent::run
*/
int functorFetchIcon(QString url)
{
    //q_debug()<<url;
    //Q_ASSERT(url.right(4).compare(QString(".gif"), Qt::CaseInsentive) == 0);
    bool  needRepeat = false;
    int   repeatTimes = 0;

    QString fileName = "";
    QString savePath = so->cachePath + "/";
    if(!QDir().exists(savePath)) {
        QDir().mkdir(savePath);
    }

    QUrl u(url);
    QList<QString> sl = url.split(QChar('/'));
    QString filename = sl.at(sl.count()-1);
    if(filename.indexOf(".") == -1) {
        filename += ".gif";
    }
    QFile *fh = new QFile(savePath + filename);
    if(fh->exists()) {
        if(QFileInfo(savePath + sl.at(sl.count()-1)).size() > 0) {            
            delete fh;
            return -1;
        }
        fh->remove();
    }    
    //q_debug()<<"open file"<<u.path();
    if(fh->open(QIODevice::ReadWrite|QIODevice::Unbuffered)) {
        QTcpSocket *sock = new QTcpSocket();
        sock->connectToHost(u.host(), u.port(80));
        //q_debug()<<"get file"<<u.path();
        if(sock->waitForConnected()) {
            //q_debug()<<"get file"<<u.path();
            QString header = QString(
                                     "GET %1 HTTP/1.0\r\n"
                                     "Host: %2\r\n"
                                     "User-Agent: Opera/9.52 (X11; Linux i686; U; zh-cn)\r\n"
                                     "Connection: close\r\n"
                                     "\r\n"
                                     ).arg(u.path(), u.host());
            //q_debug()<<header;
            sock->write(header.toAscii());
            sock->waitForBytesWritten();
            if(sock->waitForReadyRead()) {
                header.clear();
                QByteArray ba = sock->readAll();
                int pos = ba.indexOf("\r\n\r\n");
                if(pos == -1) {
                    q_debug()<<"read img data error";
                }else{
                    //q_debug()<<ba.left(pos);
                    ba = ba.right(ba.length()-pos -4);
                    fh->write(ba);
                }
            }
            delete sock;
            fh->close();
        }
    }else{
        int en = fh->error();
        q_debug()<<fh->errorString();
        delete fh;        
        return en;
    }

    return 0;
}
