#ifndef MUSICMODEL_H
#define MUSICMODEL_H
#include <item.h>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QTime>

class MusicModel : public QAbstractItemModel
{
public:
    Item *m_root;
    MusicModel();
    QModelIndex parent(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    bool insertRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    QModelIndex root_index() {return createIndex(0,0,m_root);}
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

QDataStream& operator<< (QDataStream &stream, Item &root);
QDataStream& operator>> (QDataStream &stream, Item &root);

#endif // MUSICMODEL_H
