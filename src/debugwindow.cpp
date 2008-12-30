
#include "utils.h"
#include "snsnotify.h"
#include "debugwindow.h"

DebugWindow::DebugWindow(QWidget *parent)
    : QWidget(parent, Qt::Dialog)
{
    this->ui_win.setupUi(this);

}

DebugWindow::~DebugWindow()
{
    
}

void DebugWindow::setStyleSheet()
{
}
void DebugWindow::revertStyleSheet()
{
}

void DebugWindow::testPageFrame()
{
}
