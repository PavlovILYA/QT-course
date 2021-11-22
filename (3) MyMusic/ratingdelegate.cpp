#include "ratingdelegate.h"
#include "item.h"
#include <QModelIndex>
#include <QPainter>

QSize RatingDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const{
    return QSize(100,20);
}

void RatingDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const{
    int rate = 0;
    Item *item;
    int top, left;
    QBrush b_in(Qt::yellow);
    QBrush b_out(Qt::NoBrush);
    if(index.isValid() == false) {
        return;
    }
    if(index.parent().isValid() == false) {
        return;
    }
    top = option.rect.top();
    left = option.rect.left();
    painter->setPen(QPen(Qt::black, 1));
    item = static_cast<Item *>(index.internalPointer());
    if(!item->parent()->toAlbum())
        return;
    if(!item->toSong())
        return;
    if(index.column() == 2) {
        rate = index.data().toInt();
        if(rate > 5) rate = 5;
        if(rate < 0) rate = 0;
        for(int counter = 0; counter < 5; counter++) {
            if(rate > 0 && counter < rate)
                painter->setBrush(b_in);
            else
                painter->setBrush(b_out);
            painter->drawEllipse(QPoint(left + 10, top + 10), 8, 8);
            left += 20;
        }
    }
}

QWidget * RatingDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Item *item;
    if(index.isValid() == false) {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
    if(index.parent().isValid() == false) {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
    item = static_cast<Item *>(index.internalPointer());
    if(!item->parent()->toAlbum())
        return QStyledItemDelegate::createEditor(parent, option, index);
    if(!item->toSong())
        return QStyledItemDelegate::createEditor(parent, option, index);
    RatingWidget *editor = new RatingWidget(parent);
    connect(editor, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
    return editor;
}

void RatingDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    Item *item;
    int rate;
    if(index.isValid() == false) {
        QStyledItemDelegate::setEditorData(editor, index);
    }
    if(index.parent().isValid() == false) {
        QStyledItemDelegate::setEditorData(editor, index);
    }
    item = static_cast<Item *>(index.internalPointer());
    if(!item->parent()->toAlbum())
        QStyledItemDelegate::setEditorData(editor, index);
    if(!item->toSong())
        QStyledItemDelegate::setEditorData(editor, index);
    if(index.column() == 2) {
        rate = index.data().toInt();
        RatingWidget *Editor = qobject_cast<RatingWidget *>(editor);
        Editor->setRate(rate);
    }
    else
        QStyledItemDelegate::setEditorData(editor, index);
}

void RatingDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    Item *item;
    int rate;
    if(index.isValid() == false) {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
    if(index.parent().isValid() == false) {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
    item = static_cast<Item *>(index.internalPointer());
    if(!item->parent()->toAlbum())
        QStyledItemDelegate::setModelData(editor, model, index);
    if(!item->toSong())
        QStyledItemDelegate::setModelData(editor, model, index);
    if(index.column() == 2) {
        RatingWidget *Editor = qobject_cast<RatingWidget *>(editor);
        item->toSong()->m_rate = Editor->rate();
    }
    else
    QStyledItemDelegate::setModelData(editor, model, index);
}

void RatingDelegate::commitAndCloseEditor() {
    RatingWidget *editor = qobject_cast<RatingWidget *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
