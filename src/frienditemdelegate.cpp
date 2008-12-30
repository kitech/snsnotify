
#include "utils.h"
#include "systemoption.h"
#include "useroption.h"
#include "frienditemwidget.h"
#include "frienditemdelegate.h"

FriendItemDelegate::FriendItemDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    this->setClipping(false);
}
FriendItemDelegate::~FriendItemDelegate()
{
    
}
QAbstractItemModel * p_model = NULL;
int p_row = 8888;
QSize  p_size = QSize(0, 0);

QWidget * FriendItemDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & option, 
                                             const QModelIndex & index ) const
{
    q_debug()<<"";
    QAbstractItemModel * model = (const_cast<QAbstractItemModel*>(index.model()));
    FriendItemWidget *fiw = NULL;
    FeedRecord *rec = NULL;
    QVariant dv;
    
    p_model = model;
    p_row = index.row();
    dv = model->data(index, Qt::EditRole);
    rec = (FeedRecord*)qVariantValue<void*>(dv);

    q_debug()<<rec<<fiw<<index;

    fiw = new FriendItemWidget(rec, parent);
    model->setData(index, QVariant(QSize(160, 120)), Qt::SizeHintRole);
    p_size = fiw->sizeHint();

    q_debug()<<rec<<fiw<<fiw->sizeHint();

    return fiw;    
    return NULL;
}
void FriendItemDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
{
    q_debug()<<""<<index.row();
    p_row = index.row();
}

void FriendItemDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
    q_debug()<<"";
    q_debug()<<p_model<<p_row;
    //QModelIndex index=p_model->index(p_row,0);
    //p_model->setData(index, QVariant(QSize(0,0)), Qt::SizeHintRole);
    p_row = 8888;
}

// void FriendItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
// {
//     //q_debug()<<"";    
//     QItemDelegate::paint(painter, option, index);
// }

void FriendItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QStyleOptionViewItem myopt = option;

    if(index.row() > p_row) {
        myopt.rect.setY(myopt.rect.y()+220);
        QItemDelegate::paint(painter, myopt, index);
    }else{
        QItemDelegate::paint(painter, option, index);
    }
    if(p_row == 0) {

    }

    //q_debug()<<""<<option.rect<<index<<p_row;    
}

QSize FriendItemDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QRect rect = option.rect;
    //q_debug()<<""<<rect; //QRect(0,16 296x16)
    //rect.setHeight(60);

    //q_debug()<<"";
    if(option.state == QStyle::State_Editing) {
        //return QSize(rect.width(), rect.height());
        return p_size;
    }else{
        return QSize(rect.width(), 50);
        return QItemDelegate::sizeHint(option, index);
    }
    
    return QSize(rect.width(), rect.height());
}

// QSize FriendItemDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
// {
//     QRect rect = option.rect;
//     q_debug()<<""<<rect; //QRect(0,16 296x16)
//     rect.setHeight(60);
//     q_debug()<<rect;

//     //q_debug()<<"";
//     if(option.state == QStyle::State_Editing) {
//         return QSize(rect.width(), rect.height());
//     }else{
//         return QSize(option.rect.width(), option.rect.height());
//     }
    
//     return QSize(rect.width(), rect.height());
// }

void FriendItemDelegate::updateEditorGeometry(QWidget *editor,
                          const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
    // QRect rect = option.rect;
    // q_debug()<<""<<rect; //QRect(0,16 296x16)
    // rect.setHeight(60);
    // q_debug()<<rect;
    // if(option.state == QStyle::State_Editing) {
    //     //return QSize(rect.width(), rect.height());
    //     editor->setGeometry(rect);
    // }else{
    //     //return QSizeo(option.rect.width(), option.rect.height());
    //     editor->setGeometry(option.rect);
    // }
    // return;
    // editor->setGeometry(rect);
}
