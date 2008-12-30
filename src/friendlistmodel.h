#ifndef FRIEND_LIST_MODEL_H
#define FRIEND_LIST_MODEL_H

#include <QtCore>
#include <QtGui>

class FeedRecord;

class FriendListModel : public QAbstractListModel
{
    Q_OBJECT;
public:
    FriendListModel(QObject *parent = 0);
    ~FriendListModel();
    
    bool insertItem(int row, FeedRecord * rec);
    bool appendItem(FeedRecord * rec);
    bool removeItem(int row, FeedRecord * rec);

    Qt::ItemFlags flags ( const QModelIndex & index ) const;
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    virtual bool insertRows ( int row, int count, const QModelIndex & parent = QModelIndex() ) ;
    virtual bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

private:
    QList<FeedRecord*> friends;
    QList<QSize>  sizes;
};

#endif

