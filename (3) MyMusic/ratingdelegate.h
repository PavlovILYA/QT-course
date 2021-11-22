#ifndef RATINGDELEGATE_H
#define RATINGDELEGATE_H
#include <QStyledItemDelegate>
#include "ratingwidget.h"

class RatingDelegate: public QStyledItemDelegate
{
public:
    RatingDelegate() {}
    QAbstractItemView *m_view;
    RatingDelegate(QAbstractItemView *view) { m_view = view; }
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const ;
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
public slots:
     void commitAndCloseEditor();
};

#endif // RATINGDELEGATE_H
