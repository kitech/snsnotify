#ifndef FEED_GEAR_BOX_H
#define FEED_GEAR_BOX_H

#include <QtCore>
#include <QtGui>

class FeedRecord;
class FeedItemWidget;

class FeedGearbox : public QObject
{
    Q_OBJECT;
public:
    FeedGearbox(QBoxLayout * feedContainerLayout, QObject *parent = 0);
    ~FeedGearbox();

    void addFeed(FeedRecord * feed);
    void showFeed(FeedItemWidget * feed);

private slots:
    void schedulerTimeout();
    void dynShowerTimeout();

private:
    QTimer timer;
    QTimer dynShower;
    QBoxLayout *container;    
    QVector<FeedRecord*> items;
    QVector<FeedItemWidget*> shows;
};

#endif

