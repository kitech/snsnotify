#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <QtCore>

class SessionManager : public QObject
{
    Q_OBJECT;
public:
    SessionManager(QObject *parent = 0);
    ~SessionManager();

    enum {MK_MIN = 0, MK_USERNAME='a', MK_PASSWORD='b', MK_MAX};

    bool addUser(QString name, QString password);
    bool containsUser(QString name);

    QStringList getAllNames();
    QString getUserPassword(QString name);

private:
    void save();

private:
    QVector<QMap<QChar, QString> > userAccount;
    QString dataFileName;
    char changed ;
};

#endif
