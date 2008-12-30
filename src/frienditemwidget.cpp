#include "systemoption.h"
#include "useroption.h"

#include "frienditemwidget.h"

FriendItemWidget::FriendItemWidget(FeedRecord *rec, QWidget *parent)
    : QWidget(parent)
{
    this->ui_win.setupUi(this);

    QString avata_url;
    
    this->rec = rec;
    //this->ui_win.label->setText(rec->fuid);
    this->ui_win.label->setText(rec->fusername);

    avata_url = rec->account->sc->avataUrl(rec->fuid, SiteConst::AVATA_SMALL);
    //this->ui_win.webView->setUrl(QUrl("http://www.discuz.net:80/uc_server/data/avatar/000/81/78/93_avatar_small.jpg"));
    this->ui_win.webView->setUrl(QUrl(avata_url));
}
FriendItemWidget::~FriendItemWidget()
{
    
}

bool FriendItemWidget::equalsTo(FeedRecord *f)
{
    return this->rec->equalsTo(f);
}
bool FriendItemWidget::similarToKeyword(QString keyword)
{
    return false;
}
