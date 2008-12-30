#ifndef FRIEND_ITEM_DELEGATE_H
#define FRIEND_ITEM_DELEGATE_H

#include <QtCore>
#include <QtGui>

class FriendItemDelegate : public QItemDelegate
{
    Q_OBJECT;
public:
    FriendItemDelegate(QObject *parent = 0);
    ~FriendItemDelegate();

    virtual QWidget * createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void setEditorData ( QWidget * editor, const QModelIndex & index ) const;
    virtual void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
    virtual QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:

};

#endif
