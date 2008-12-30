
#include "chatwindow.h"

ChatWindow::ChatWindow(QString uid, QString username, QWidget *parent)
    : QMainWindow(parent)
{
    center_win = new QWidget(this);
    this->ui_win.setupUi(center_win);
    this->setCentralWidget(center_win);

    this->uid = uid;
    this->username = username;

    this->setWindowTitle(this->center_win->windowTitle().arg(uid));
    this->ui_win.label->setText("");
    this->ui_win.label_2->setText(username);
    this->ui_win.label_3->setText("");

    QObject::connect(this->ui_win.toolButton, SIGNAL(clicked()), this, SLOT(sendMsg()));
}

ChatWindow::~ChatWindow()
{
    
}

void ChatWindow::sendMsg()
{
    QString msg = this->ui_win.textEdit->toPlainText().trimmed();
    if(!msg.isEmpty()) {
        emit this->sendMsg(uid, msg);
        this->ui_win.textBrowser->append(msg);
        this->ui_win.textEdit->clear();
    }else{
        this->ui_win.textEdit->setToolTip(tr("No Message Content"));
    }
}
