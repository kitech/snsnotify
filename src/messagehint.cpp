#include "utils.h"
#include "messagehint.h"

MessageHint::MessageHint(QWidget *parent)
    : QWidget(parent, Qt::ToolTip|Qt::MSWindowsFixedSizeDialogHint	)
{
    this->ui_win.setupUi(this);

    this->timer.setInterval(50);
    QObject::connect(&this->timer, SIGNAL(timeout()), this, SLOT(dynShowTimeout()));
    this->hideDida.setInterval(10*1000);
    QObject::connect(&this->hideDida, SIGNAL(timeout()), this, SLOT(hideDidaTimeout()));

    this->msgTpl = tr("You have %1 new feed.");

    QPixmap pixmap("bg.png");
    //this->setPixmap(pixmap);
    this->setMask(pixmap.mask());
}
MessageHint::~MessageHint()
{

}

void MessageHint::show(QString msg)
{
    QDesktopWidget dw;
    int num = dw.numScreens();
    QRect rect = dw.screenGeometry();
    QRect geom = this->geometry();
    int x,y;

    x = rect.width() - geom.width() - 20;
    //y = rect.height() - geom.height();
    y = rect.height();

    //q_debug()<<x<<y<<rect<<geom<<dw.availableGeometry();

    this->move(x, y);    
    this->ui_win.label->setText(msg);
    QWidget::show();

    this->timer.start();
}
void MessageHint::dynShowTimeout()
{
    QRect rect = QDesktopWidget().availableGeometry();
    QRect geom = this->geometry();
    int x,y;

    x = geom.x();
    y = geom.y() - 10;

    this->move(x, y);
    //q_debug()<<geom;
    if(y+this->height() <= rect.height()) {
        this->timer.stop();
        this->hideDida.start();
    }
}

void MessageHint::hideDidaTimeout()
{
    this->hide();
    this->hideDida.stop();
}
