#ifndef MANYOU_API_CLIENT_H
#define MANYOU_API_CLIENT_H

#include <QtCore>
#include <QtNetwork>

class Manyou_API_Client : public QObject
{
    Q_OBJECT;
public:
    Manyou_API_Client(QObject *parent = 0);
    ~Manyou_API_Client();
    
    enum{FMT_PHP, FMT_XML, FMT_JSON};

public:
    void api_test(QString api_key, QString secret, QString session_key);

    bool profile_setMYML(QString myml, int uid = 0) ;
    bool profile_getMYML(int uid = 0);    
    bool friend_get();
    bool friend_areFriends(int uid1, int uid2) ;
    bool friend_getAppUsers();
    bool user_getLoggedInUser();
    bool user_getLoggedInUserLevel();
    bool user_getInvitationURL(int sid) ;
    bool user_isAppAdded();
    bool user_getinfo(QString uids, QString fields = QString());
    bool notification_send(QString uids, QString msg);
    bool notification_get();
    bool feed_publishTemplatizedAction(
        QString title_template, QString title_data, QString body_template, 
        QString body_data, QString body_general, QString image_1, QString image_1_link,
        QString image_2, QString image_2_link, QString image_3,
        QString image_3_link, QString image_4, QString image_4_link, 
        QString target_ids = QString());
    bool site_get(int sid);

private:
    bool call_method(QString method, QMap<QString, QString> args);
    QPair<int, QString>  post_request(QString url, QMap<QString, QString> args, QMap<QString, QString> myparams);

private:
    QString user;
    QString friends;
    QString added;
    QString api_key;
    QString secret;
    int     errno;
    QString errmsg;
    
    int  format;
    QString session_key; // from cookie
};

#endif
