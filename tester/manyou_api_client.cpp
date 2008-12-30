
#include "../src/modules/md5.h"
#include "../src/utils.h"

#include "manyou_api_client.h"

//////////////////////////
QString md5StringSum(unsigned char *md5sum)
{
    const char *hexchars = "0123456789abcdef";
    unsigned char ch;
    unsigned char chh4, chl4;
    QString md5str;
    char c;
    for(int i = 0 ; i < 16; i ++) {
        //ch = this->md5sum[i];
        ch = md5sum[i];
        chh4 = ch >> 4 & 0x0F;
        chl4 = ch & 0x0F;
        Q_ASSERT(chh4 >= 0 && chh4 < 16);
        Q_ASSERT(chl4 >= 0 && chl4 < 16);
        c = hexchars[chh4];
        md5str += QChar(c);
        c = hexchars[chl4];
        md5str += QChar(c);
    }
    
    Q_ASSERT(md5str.length() == 32);
    return md5str;
}

///////////////////
Manyou_API_Client::Manyou_API_Client(QObject *parent)
    : QObject(parent)
{
    this->format = FMT_PHP;
}
Manyou_API_Client::~Manyou_API_Client()
{

}

void Manyou_API_Client::api_test(QString api_key, QString secret, QString session_key)
{
    this->api_key = api_key;
    this->secret = secret;
    this->session_key = session_key;

    q_debug()<<this->profile_setMYML("what", 1040351);
    q_debug()<<this->profile_setMYML("hello myml", 1040351);

    q_debug()<<this->profile_setMYML("what");
    q_debug()<<this->profile_setMYML("hello myml");

    q_debug()<<this->friend_get();
    
}

bool Manyou_API_Client::profile_setMYML(QString myml, int uid) 
{
    QMap<QString, QString> args;
    
    args["myml"] = myml;
    args["uid"] = QString("%1").arg(uid);
    
    return this->call_method("profile.setMYML", args);
}

bool Manyou_API_Client::profile_getMYML(int uid)
{
    QMap<QString, QString> args;
    
    args["uid"] = QString("%1").arg(uid);
    
    return this->call_method("profile.getMYML", args);    
}

bool Manyou_API_Client::friend_get()
{
    QMap<QString, QString> args;

    return this->call_method("notification.get", args);
}


bool Manyou_API_Client::friend_areFriends(int uid1, int uid2) 
{
    QMap<QString, QString> args;
    
    args["uid1"] = QString("%1").arg(uid1);
    args["uid2"] = QString("%1").arg(uid2);
    
    return this->call_method("friend.areFriends", args);
}
bool Manyou_API_Client::friend_getAppUsers()
{
    QMap<QString, QString> args;
        
    return this->call_method("friend.getAppUsers", args);
}

bool Manyou_API_Client::user_getLoggedInUser()
{
    QMap<QString, QString> args;
        
    return this->call_method("user.getLoggedInUser", args);
}
bool Manyou_API_Client::user_getLoggedInUserLevel()
{
    QMap<QString, QString> args;
        
    return this->call_method("user.getLoggedInUserLevel", args);
}
bool Manyou_API_Client::user_getInvitationURL(int sid) 
{
    QMap<QString, QString> args;

    args["sid"] = QString("%1").arg(sid);
        
    return this->call_method("user.getInvitationURL", args);
}
bool Manyou_API_Client::user_isAppAdded()
{
    QMap<QString, QString> args;
        
    return this->call_method("user.isAppAdded", args);
}
bool Manyou_API_Client::user_getinfo(QString uids, QString fields)
{
    QMap<QString, QString> args;

    args["uids"] = uids;
    args["fields"] = fields;
        
    return this->call_method("friend.getAppUsers", args);
}

bool Manyou_API_Client::notification_send(QString uids, QString msg)
{
    QMap<QString, QString> args;

    args["uids"] = uids;
    args["msg"] = msg;
    
    return this->call_method("user.getinfo", args);
}

bool Manyou_API_Client::notification_get()
{
    QMap<QString, QString> args;
        
    return this->call_method("notification.get", args);
}

bool Manyou_API_Client::feed_publishTemplatizedAction(
                                                      QString title_template, QString title_data, 
                                                      QString body_template, 
                                                      QString body_data, QString body_general, QString image_1, 
                                                      QString image_1_link,
                                                      QString image_2, QString image_2_link, QString image_3,
                                                      QString image_3_link, QString image_4, QString image_4_link, 
                                                      QString target_ids)
{
    QMap<QString, QString> args;

    args["title_template"] = title_template;
    args["title_data"] = title_data;
    args["body_template"] = body_template;
    args["body_data"] = body_data; 
    args["body_general"] = body_general;
    args["image_1"] = image_1;
    args["image_1_link"] = image_1_link;
    args["image_2"] = image_2;
    args["image_2_link"] = image_2_link;
    args["image_3"] = image_3;
    args["image_3_link"] = image_3_link;
    args["image_4"] = image_4;
    args["image_4_link"] = image_4_link;
    args["target_ids"] = target_ids;
    
    return this->call_method("feed.publishTemplatizedAction", args);    
}

bool Manyou_API_Client::site_get(int sid)
{
    QMap<QString, QString> args;

    args["sid"] = QString("%1").arg(sid);

    return this->call_method("site.get", args);
}

///////////
bool Manyou_API_Client::call_method(QString method, QMap<QString, QString> args)
{
    this->errno = 0;
    this->errmsg.clear();

    QString url = "http://api.manyou.com/openapi.php";
    QMap<QString, QString> myparams;
    QString mystr, md5str;
    QString tmp;
    unsigned char md5sum[16] = {0};

    myparams["method"] = method;
    myparams["session_key"] = this->session_key;
    myparams["api_key"] = this->api_key;
    myparams["format"] = "XML";
    myparams["v"] = "0.1";

    QMap<QString,QString>::iterator it;
    for(it = myparams.begin(); it != myparams.end(); it++) {
        mystr += QString("%1=%2&").arg(it.key(), it.value());
    }

    for(it = args.begin(); it != args.end(); it++) {
        mystr += QString("args[%1]=%2&").arg(it.key(), it.value());
    }
    tmp = mystr + this->secret;
    md5CheckSum(tmp.toAscii().data(), tmp.length(), md5sum);
    
    md5str = md5StringSum(md5sum);
    myparams["sig"] = md5str;

    //q_debug()<<mystr<<md5str<<myparams;

    QPair<int, QString> rp ;
    rp = this->post_request(url, args, myparams);
    
    //q_debug()<<rp;    

    return true;
}

QPair<int, QString>  
Manyou_API_Client::post_request(QString url, QMap<QString, QString> args, 
                                QMap<QString, QString> myparams)
{
    QPair<int, QString>  rp;
    QUrl uu(url);
    QTcpSocket sock;
    QString data;
    QString header;

    QMap<QString,QString>::iterator it;
    for (it = myparams.begin(); it != myparams.end(); it++) {
        data += QString("&%1=%2").arg(it.key(), QString(QUrl::toPercentEncoding(it.value())));
    }
    for (it = args.begin(); it != args.end(); it ++) {
        data += QString("&args[%1]=%2").arg(it.key(), QString(QUrl::toPercentEncoding(it.value())));
    }
    
    //q_debug()<<data<<QString("%1").arg(data.length());
    
    header = QString(
                     "POST %1 HTTP/1.0\r\n"
                     "Host: %2\r\n"
                     "Content-type: application/x-www-form-urlencoded\r\n"
                     "User-Agent: Manyou API PHP Client 0.1 (non-curl)\r\n"
                     "Content-length: %3\r\n"
                     "Cache: no\r\n"
                     "Connection: close\r\n"
                     "\r\n"
                     ).arg(uu.path(), uu.host(), QString("%1").arg(data.length()));

    //q_debug()<<header;

    //return rp;

    sock.connectToHost(uu.host(), uu.port(80));
    if (!sock.waitForConnected()) {
        q_debug()<<"connect error";
    } else {
        sock.write(QString(header + data).toAscii());
        if (!sock.waitForBytesWritten()) {
            q_debug()<<"write error";
        } else {
            QByteArray ba;
            int dpos = -1;
        read_more:
            if (sock.waitForReadyRead()) {
                ba = sock.readAll();
                //q_debug()<<ba;
                if(sock.isValid()) goto read_more;
            } else {
                //q_debug()<<"cant read??? or read done";
            }
            if ((dpos = ba.indexOf("\r\n\r\n")) != -1) {
                q_debug()<<ba.right(ba.length() - dpos).trimmed();
            }
        }
    }

    return rp;
}

