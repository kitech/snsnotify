
#include <QtCore>
#include <QtNetwork>

#include "utils.h"
#include "systemoption.h"
#include "liveupdate.h"
#include "snsnotify.h"

#include "modules/feed_record.h"

/*
  /sns/modver.lst
  /sns/modules/modname,modver
  TODO: add checksum of module file, like this modname,modver,checksum
  下载完模块后，由SNSNotify实例完成模块的更新，替换工作。
 */

LiveUpdate::LiveUpdate(QObject * parent) 
    : QThread(parent)
{
    modversUrl = "http://localhost/sns/modver.lst";
    modBaseUrl = "http://localhost/sns/modules/";

    this->sns_notify = (SNSNotify*)parent;
}
LiveUpdate::~LiveUpdate()
{
    
}

void LiveUpdate::run()
{
    this->getLocalModVers();
    this->getRemoteModVers();
    this->hasUpdateModules = this->findNewModoules();

    if(this->hasUpdateModules.count() > 0) {
        emit hasUpdateModulesFound();

        for(int i = 0; i < this->hasUpdateModules.count(); i ++) {
            QStringList module_version = this->hasUpdateModules.at(i).split(",");
            emit fetchModule(module_version.at(0), module_version.at(1).toInt());
            this->fetchModuleFile(module_version.at(0), module_version.at(1).toInt());
        }
    }
    //done
}

bool LiveUpdate::getLocalModVers()
{
    QString modulePath = so->modulePath;
    //void *handle = NULL;
    QLibrary *so_handle = NULL;
    QStringList moduleFileList ;
    QString tmpfname, modver;
    SiteConst * sc = NULL;
    SiteConst *(*get_site_const)(int) = NULL;
    int (*release_site_const)(SiteConst **) = NULL;
    int ver = 0;

    this->localModVers.clear();
    ver = this->sns_notify->get_version();
    this->localModVers.append(QString("snsmain,%2").arg(ver));

    moduleFileList = QDir(modulePath).entryList();
    q_debug()<<moduleFileList;
    
    for(int i = moduleFileList.count() - 1 ; i >= 0 ; i --) {
        tmpfname = moduleFileList.at(i);
        if(QLibrary::isLibrary(modulePath+"/"+tmpfname)) {
            //ok modules
            q_debug()<<"Loading :"<<(modulePath + "/" + tmpfname);
            so_handle = new QLibrary(modulePath + "/" + tmpfname);
            if(!so_handle->load()) {
                q_debug()<<"Load library error";
                continue;
            }
            *(void**)(&get_site_const) = so_handle->resolve("get_site_const");
            q_debug()<<"may error:"<<so_handle->errorString();
            Q_ASSERT(get_site_const != NULL);
            sc = get_site_const(0);
            
            ver = sc->get_version();
            modver = Utils::get_mod_name_from_file_name(tmpfname);
            modver = QString("%1,%2").arg(modver, QString("%1").arg(ver));
            this->localModVers.append(modver);

            *(void**)(&release_site_const) = so_handle->resolve("release_site_const");
            Q_ASSERT(release_site_const != NULL);
            release_site_const(&sc);
            sc = NULL;
            get_site_const = NULL;

            q_debug()<<tmpfname<<" : "<<ver;
            so_handle->unload();
            delete so_handle;
            so_handle = NULL;
        }else{
            
        }
    }
    q_debug()<<this->localModVers;
    return true;
    return false;
}
bool LiveUpdate::getRemoteModVers()
{
    QTcpSocket *sock = NULL;
    QUrl u(this->modversUrl);

    sock = new QTcpSocket();
    q_debug()<<u.host()<<u.port();
    sock->connectToHost(u.host(), u.port() == -1? 80 : u.port());
    if(!sock->waitForConnected()) {
        q_debug()<<"wait for connected error";
        return false;
    }

    QString request;
    request = 
        QString(
                "GET %1 HTTP/1.1\r\n"
                "Host: %2\r\n"
                "Connection: close\r\n"
                "\r\n"
                )
        .arg(u.path(), u.host());
    
    q_debug()<<request;
    sock->write(request.toAscii());
    if(!sock->waitForBytesWritten()) {
        q_debug()<<"wiat for write error";
        return false;
    }

    if(!sock->waitForReadyRead()) {
        q_debug()<<"wait read error";
    }
    QByteArray ba ;
    qint64 oklen;
    while(true) {
        oklen = sock->bytesAvailable();
        if(oklen > 0) {
            ba += sock->readAll();
        }else if(oklen == 0) {
            if(!sock->waitForReadyRead()) {
                q_debug()<<"wait read error";
                break;
            }
        }else{
            q_debug()<<"bytesAvalibale < 0";
            break;
        }
    }
    delete sock; sock = NULL;

    int dpos = -1;
    dpos = ba.indexOf("\r\n\r\n");
    Q_ASSERT(dpos != -1);

    dpos += 4;

    QByteArray data = ba.right(ba.length()-dpos);

    q_debug()<<data;

    QStringList moduleList = QString(data.trimmed()).split("\n");

    for(int i = 0; i < moduleList.count(); i ++) {
        this->remoteModVers.append(moduleList.at(i).trimmed());
    }
    
    q_debug()<<this->remoteModVers;

    return true;
    //return false;
}

QVector<QString> LiveUpdate::findNewModoules()
{
    QVector<QString> newModules;
    int ver;
    
    for(int i = 0 ; i < this->localModVers.count(); i ++) {
        QStringList module_version = this->localModVers.at(i).split(",");
        for(int j = 0 ; j < this->remoteModVers.count(); j ++) {
            QStringList rmodule_version = this->remoteModVers.at(j).split(",");
            if(rmodule_version.at(0) == module_version.at(0)) {
                if(rmodule_version.at(1).toInt() > module_version.at(1).toInt()) {
                    newModules.append(QString("%1,%2").arg(rmodule_version.at(0), rmodule_version.at(1)));
                }else{
                    
                }
            }
        }
    }

    q_debug()<<newModules;

    return newModules;
}

bool LiveUpdate::fetchModuleFile(QString name, int ver)
{
    QString modPathName = name + "," + QString("%1").arg(ver);
#ifdef WIN32
    modPathName += "-win32";
#else
    modPathName += "-linux";
#endif
    QTcpSocket *sock = NULL;
    QUrl u(this->modBaseUrl+modPathName);

    sock = new QTcpSocket();
    q_debug()<<u.host()<<u.port();
    sock->connectToHost(u.host(), u.port() == -1? 80 : u.port());
    if(!sock->waitForConnected()) {
        q_debug()<<"wait for connected error";
        return false;
    }

    QString request;
    request = 
        QString(
                "GET %1 HTTP/1.1\r\n"
                "Host: %2\r\n"
                "Connection: close\r\n"
                "\r\n"
                )
        .arg(u.path(), u.host());
    
    q_debug()<<request;
    sock->write(request.toAscii());
    if(!sock->waitForBytesWritten()) {
        q_debug()<<"wiat for write error";
        return false;
    }

    if(!sock->waitForReadyRead()) {
        q_debug()<<"wait read error";
    }
    QByteArray ba ;
    qint64 oklen;
    while(true) {
        oklen = sock->bytesAvailable();
        if(oklen > 0) {
            ba += sock->readAll();
        }else if(oklen == 0) {
            if(!sock->waitForReadyRead()) {
                q_debug()<<"wait read error";
                break;
            }
        }else{
            q_debug()<<"bytesAvalibale < 0";
            break;
        }
    }
    delete sock; sock = NULL;

    int dpos = -1;
    dpos = ba.indexOf("\r\n\r\n");
    Q_ASSERT(dpos != -1);

    dpos += 4;
    
    QByteArray header = ba.left(dpos);
    q_debug()<<header;
    QByteArray data = ba.right(ba.length()-dpos);

    //q_debug()<<data;
    
    QFile fp(so->modulePath+ modPathName);
    if(!fp.open(QIODevice::ReadWrite)) {
        q_debug()<<"open module file for write error";
        return false;
    }
    fp.resize(0);
    fp.write(data);
    fp.close();
    
    return true;
}

