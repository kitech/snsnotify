#ifndef ABOUT_H
#define ABOUT_H

#include <QtCore>
#include <QtGui>

#include "ui_about.h"

class About : public QDialog
{
    Q_OBJECT;
public:
    About(QWidget *parent = 0);
    ~About();
private:
    Ui::About  ui_win;
};


#endif

