#ifndef FEED_ITEM_WIDGET
#define FEED_ITEM_WIDGET


#include <QtCore>
#include <QtGui>
#include <QtWebKit>

#include "utils.h"

#include "ui_feeditemwidget.h"

class FeedRecord;

class FeedItemWidget : public QWidget
{
    Q_OBJECT;
public:
    FeedItemWidget(FeedRecord *feed, QWidget *parent = 0);
    ~FeedItemWidget();

    bool equalsTo(FeedRecord *f);
    bool similarToKeyword(QString keyword);

private:
    bool initFeedTemplate();

public slots:
    void show();

private slots:
    void activeLink(const QString &link);
    void hoverLink(const QString &link);
    void autoAdjustWebViewSize();
    void wkActiveLink(const QUrl &url);
    void wkHoverLink(const QString &link, const QString &title, const QString &content);
    void wkContextMenuRequested(const QPoint &pos);
    void wkChangeUrl(const QUrl &url);
    void wkChangeGeometry(const QRect &geom);
    void wkRequestRepaint(const QRect &dirtyRect);
    void wkRequestScroll ( int dx, int dy, const QRect & rectToScroll );
    void wkStatusBarMessage(const QString &text);
    void wkLoadFinished ( bool ok );
    void wkRequestDownload(const QNetworkRequest & request);
    void wkDealUnsupportedContent(QNetworkReply *reply);

protected:
    void showEvent ( QShowEvent * event );

private:
    FeedRecord *feed;
    Ui::FeedItemWidget ui_win;
    bool sizeAdjusted;
    QWebPage *wp;
};

#endif
