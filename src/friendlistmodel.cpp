#include <QtGui>

#include "utils.h"
#include "systemoption.h"

#include "friendlistmodel.h"

FriendListModel::FriendListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    
}

FriendListModel::~FriendListModel()
{

}
Qt::ItemFlags FriendListModel::flags ( const QModelIndex & index ) const
{
    //return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
    return QAbstractListModel::flags(index) ;
}

int FriendListModel::rowCount(const QModelIndex & parent // = QModelIndex()
                              ) const
{
    //q_debug()<<this->friends.count();
    return this->friends.count();
}

bool FriendListModel::insertItem(int row, FeedRecord * rec)
{
    q_debug()<<"here";
    QModelIndex idx;
    this->insertRows(row, 1, idx);
    idx = this->index(row, 0, idx);
    this->setData(idx, qVariantFromValue((void*)rec));
    return true;
}
bool FriendListModel::appendItem(FeedRecord * rec)
{
    return this->insertItem(this->friends.count(), rec);
}
bool FriendListModel::removeItem(int row, FeedRecord * rec)
{

    return true;
}

QVariant FriendListModel::data ( const QModelIndex & index, int role // = Qt::DisplayRole
                                 ) const
{
    //q_debug()<<"here"<<index.row();
    if(role == Qt::DecorationRole) {
        //q_debug()<<"here"<<index.row();
        return (QPixmap(so->iconPath + "/noavatar_small.gif"));
    }
    FeedRecord *fr = this->friends.at(index.row());
    if(role == Qt::DisplayRole) {
        //q_debug()<<"here"<<index.row();
        QVariant d = fr->fusername;
        //q_debug()<<"here"<<index.row()<<index.column()<<fr<<d<<this->friends;
        return d;
    }
    if(role == Qt::EditRole) {
        //q_debug()<<"here"<<index.row();
        return qVariantFromValue((void*)fr);
    }
    if(role == Qt::SizeHintRole) {
        QSize size = this->sizes.at(index.row());
        if(size.width() == 0) {
            return QVariant(QSize(160, 50));
            return QVariant();
        }else{
            return QVariant(size);
        }
    }
    if(role == Qt::FontRole) {
        QFont font;
        font.setBold(true);
        //font.setStretch(200);
        font.setPointSize(12);
        return font;
        return QVariant(18);
    }
    return QVariant();
}
bool FriendListModel::setData ( const QModelIndex & index, const QVariant & value, int role // = Qt::EditRole
                                )
{
    q_debug()<<"here";
    //void dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight )
    //emit dataChanged();
    if(role == Qt::SizeHintRole) {
        QSize size = value.toSize();
        this->sizes[index.row()] = size;
        emit dataChanged(index, index);
        return true;
    }else if(role == Qt::EditRole){
        FeedRecord *rec = (FeedRecord*)qVariantValue<void*>(value);
        this->friends[index.row()] = rec;
        emit dataChanged(index, index);
        return true;
    }
    
    return false;
}
bool FriendListModel::insertRows ( int row, int count, const QModelIndex & parent // = QModelIndex()
                  ) 
{
    //q_debug()<<"here";
    //const QModelIndex & parent, int first, int last
    beginInsertRows(QModelIndex(), row, count);
    for(int i = 0 ; i < count ; i ++) {
        this->friends.insert(row, 0);
        this->sizes.insert(row, QSize(0,0));
    }
    //
    endInsertRows();
    return true;
}
bool FriendListModel::removeRows ( int row, int count, const QModelIndex & parent // = QModelIndex()
                  )
{
    //beginRemoveRows ( const QModelIndex & parent, int first, int last )

    //endRemoveRows();
    return true;
}
