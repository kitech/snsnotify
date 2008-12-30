#ifndef FRIEND_ITEM_WIDGET_H
#define FRIEND_ITEM_WIDGET_H

#include <QtCore>
#include <QtGui>

#include <QtWebKit>

#include "utils.h"

#include "ui_frienditemwidget.h"

class FeedRecord;

class FriendItemWidget : public QWidget
{
    Q_OBJECT;
public:
    FriendItemWidget(FeedRecord *rec, QWidget *parent = 0);
    ~FriendItemWidget();

    bool equalsTo(FeedRecord *f);
    bool similarToKeyword(QString keyword);

private:
    FeedRecord *rec;
    Ui::FriendItemWidget ui_win;

};

#endif
