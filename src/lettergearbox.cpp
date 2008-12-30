#include "utils.h"
#include "useroption.h"
#include "feeditemwidget.h"
#include "lettergearbox.h"

LetterGearbox::LetterGearbox(QBoxLayout * feedContainerLayout, QObject *parent)
    : QObject(parent)
{
    this->container = feedContainerLayout;

    int intval = 30;
    this->timer.setInterval(intval); //is 30/1000 s = 3/100 s
    QObject::connect(&this->timer, SIGNAL(timeout()), this, SLOT(schedulerTimeout()));

    this->dynShower.setInterval(intval/3);
    QObject::connect(&this->dynShower, SIGNAL(timeout()), this, SLOT(dynShowerTimeout()));
}


LetterGearbox::~LetterGearbox()
{
}

void LetterGearbox::schedulerTimeout()
{
    FeedRecord *feed = NULL;
    FeedItemWidget *fiw = NULL;

    if(this->items.count()>0) {
        feed = this->items.at(0);        
        this->items.remove(0);

        fiw = new FeedItemWidget(feed);
        this->container->insertWidget(0, fiw);
        //fiw->show();
    }
    if(this->items.count() == 0) {
        this->timer.stop();
    }
}

void LetterGearbox::addFeed(FeedRecord* feed)
{
    if(this->items.contains(feed)) {
        q_warning()<<"duplicate feed record?"<<feed;
    }else{
        this->items.append(feed);
        if(!this->timer.isActive()) {
            this->timer.start();
        }
    }
}

void LetterGearbox::showFeed(FeedItemWidget * feed)
{
    if(this->shows.contains(feed)) {
        q_warning()<<"duplicate feed record?"<<feed;
    }else{
        this->shows.append(feed);
        if(!this->dynShower.isActive()) {
            this->dynShower.start();
        }
    }
}
void LetterGearbox::dynShowerTimeout()
{
    FeedItemWidget *fiw = NULL;

    if(this->shows.count()>0) {
        fiw = this->shows.at(0);        
        this->shows.remove(0);
        
        fiw->setVisible(true);
    }
    if(this->shows.count() == 0) {
        this->dynShower.stop();
    }
}

