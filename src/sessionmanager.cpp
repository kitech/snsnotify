
#include <QDataStream>

#include "utils.h"
#include "systemoption.h"

#include "sessionmanager.h"

SessionManager::SessionManager(QObject *parent)
    :QObject(parent)
{
    this->dataFileName = so->dotPath + QString("sess.db");
    this->changed = 0;
}

SessionManager::~SessionManager()
{
    //q_debug()<<"";
    if(this->changed == 1) {
        this->save();
    }
}

bool SessionManager::addUser(QString name, QString password)
{
    if(this->userAccount.count() == 0) {
        this->getAllNames();
    }
    QMap<QChar, QString> elem;
    char key = MK_USERNAME;

    for(int i = 0 ; i < this->userAccount.count(); i++) {
        elem = this->userAccount.at(i);
        if(elem[QChar(key)] == name) {
            this->userAccount[i][QChar(MK_PASSWORD)] = password;
            return true;
        }
    }
    elem[QChar(MK_USERNAME)] = name;
    elem[QChar(MK_PASSWORD)] = password;

    this->userAccount.append(elem);
    this->changed = 1;

    return true;
}
bool SessionManager::containsUser(QString name)
{
    QMap<QChar, QString> elem;
    char key = MK_USERNAME;
    for(int i = 0 ; i < this->userAccount.count(); i++) {
        elem = this->userAccount.at(i);
        if(elem[QChar(key)] == name) {
            return true;
        }
    }
    return false;
}
QString SessionManager::getUserPassword(QString name)
{
    QString password;
    QStringList nameLists;
    if(this->userAccount.count() == 0) {
        nameLists = this->getAllNames();
    }
    
    QMap<QChar, QString> elem;
    char key = MK_USERNAME;
    for(int i = 0 ; i < this->userAccount.count(); i++) {
        elem = this->userAccount.at(i);
        if(elem[QChar(key)] == name) {
            password = elem[QChar(MK_PASSWORD)];
            return password;
        }
    }
    q_debug()<<"User record not found:"<<name;
    return password;
}
QStringList SessionManager::getAllNames()
{
    QStringList nameLists;

    if(this->userAccount.count() == 0) {
        if(QFile::exists(this->dataFileName)) {
            QFile fp(this->dataFileName);
            fp.open(QIODevice::ReadOnly);
            QDataStream ds(&fp);
            ds>>this->userAccount ; 
            fp.close();
        }
    }
    QMap<QChar, QString> elem;
    char key = MK_USERNAME;
    for(int i = 0 ; i < this->userAccount.count(); i++) {
        elem = this->userAccount.at(i);
        nameLists<<elem[QChar(key)];
    }
    //q_debug()<<this->userAccount;

    return nameLists;
}

void SessionManager::save()
{
    QFile fp(this->dataFileName);
    if(!fp.open(QIODevice::ReadWrite)) {
        q_debug()<<"Open file error";
    }
    fp.resize(0);
    QDataStream ds(&fp);
    ds<<this->userAccount ; 
    fp.close();
    //q_debug()<<"save session done"<<this->dataFileName;
}
