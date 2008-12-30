#ifndef DEBUG_WINDOW_H
#define DEBUG_WINDOW_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

#include "ui_debugwindow.h"

class SNSNotify;
class DebugWindow : public QWidget
{
    Q_OBJECT;
public:
    DebugWindow(QWidget *parent = 0);
    ~DebugWindow();

private slots:
    void setStyleSheet();
    void revertStyleSheet();

    void testPageFrame();

private:
    Ui::DebugWindow  ui_win;
    SNSNotify *notify_win;
    QString   orgCSS;
};

#endif

