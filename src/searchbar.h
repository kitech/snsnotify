#ifndef SEARCH_BAR_H
#define SEARCH_BAR_H

#include <QtGui>
#include "ui_searchbar.h"

class SearchBar : public QWidget
{
    Q_OBJECT;
public:
    SearchBar(QWidget *parent = 0);
    ~SearchBar();

private:
    Ui::SearchBar ui_win;
};

#endif
