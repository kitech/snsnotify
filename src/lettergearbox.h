#ifndef LETTER_GEAR_BOX_H
#define LETTER_GEAR_BOX_H

#include <QtCore>
#include <QtGui>

class FeedRecord;
class FeedItemWidget;

class LetterGearbox : public QObject
{
    Q_OBJECT;
public:
    LetterGearbox(QBoxLayout * feedContainerLayout, QObject *parent = 0);
    ~LetterGearbox();

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

