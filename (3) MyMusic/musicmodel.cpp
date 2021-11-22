#include "musicmodel.h"

MusicModel::MusicModel()
{
    m_root = new Item();
}

QModelIndex MusicModel::index(int row, int column, const QModelIndex &parent) const {
    Item *parentItem = m_root;

    if(parent.isValid())
    {
        parentItem = static_cast<Item *>(parent.internalPointer());
    }
    if(parentItem->childCount() > row)
        if(parentItem->childAt(row))
        {
            return createIndex(row, column, parentItem->childAt(row));
        }
    return QModelIndex();
}

QModelIndex MusicModel::parent(const QModelIndex &child) const {
    if(!child.isValid()) return QModelIndex();
    else {
        Item *child_item = static_cast<Item*>(child.internalPointer());
        if(child_item == m_root) return QModelIndex();
        if(child_item->parent() == m_root) return QModelIndex();
        if(!child_item->parent()) return QModelIndex();
        else {
            Item *parent = child_item->parent();
            if(!parent->parent()) return QModelIndex();
            else
            {
                Item *grandparent = parent->parent();
                return createIndex(grandparent->indexOf(parent),0,parent);
            }
        }
    }
}

QVariant MusicModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole) {
        switch (section)
        {
        case 0:
            return QObject::tr("Исполнитель");
        case 1:
            return QObject::tr("Продолжительность");
        case 2:
            return QObject::tr("Рейтинг");
        case 3:
            return QObject::tr("Комментарий");
        }
    }
    return QVariant();
}

int MusicModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 4;
}

int MusicModel::rowCount(const QModelIndex &parent) const {
    if(parent.isValid()) {
        Item *item;
        item = static_cast<Item *>(parent.internalPointer());
        return item->childCount();
    }
    return m_root->childCount();
}

QVariant MusicModel::MusicModel::data(const QModelIndex &index, int role) const
{

    if(role == Qt::DisplayRole)
    {
        if(index.isValid())
        {
            int coloumn = index.column();
            Item *item = static_cast<Item*>(index.internalPointer());
            if(item)
            {
                switch (coloumn)
                {
                case 0:
                    return item->name();
                    break;
                case 1:
                    if(item->toSong()) return item->toSong()->duration();
                    break;
                case 2:
                    if(item->toSong()) return item->toSong()->rate();
                    break;
                case 3:
                    if(item->toSong()) return item->comment();
                    break;

                }

            }
            else return QVariant();

        }
        else return QVariant();
    }
    if (role == Qt::EditRole)
    {
        if(index.isValid())
        {
            int coloumn = index.column();
            Item *item = static_cast<Item*>(index.internalPointer());
            if(item)
            {
                switch (coloumn)
                {
                case 0:
                    return item->name();
                    break;
                case 1:
                    if(item->toSong()) return item->toSong()->duration();
                    if(item->toAlbum()) return item->toAlbum()->genre();
                    if(item->toArtist()) return item->toArtist()->country();
                    break;
                case 2:
                    if(item->toSong()) return item->toSong()->rate();
                    if(item->toAlbum()) return item->toAlbum()->year();
                    if(item->toArtist()) return item->comment();
                    break;
                case 3:
                     if(item->toSong()) return item->comment();
                     if(item->toAlbum()) return item->comment();
                     if(item->toArtist()) return item->toArtist()->photo();
                    break;
                case 4:
                    if(item->toAlbum()) return item->toAlbum()->cover();
                    break;

                }

            }
            else return QVariant();

        }
        else return QVariant();
    }
    return QVariant();
}

bool MusicModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(index.isValid()) {
        if(role != Qt::EditRole && role != Qt::DisplayRole) return false;
        int coloumn = index.column();
        int row = index.row();
        Item * item = static_cast<Item *>(index.internalPointer());
        if (item) {
            switch (coloumn)
            {
                case 0:
                    item->setName(value.toString());
                    break;
                case 1:
                    if (item->toSong())
                    {
                        QTime duration = QTime::fromString( value.toString(), Qt::ISODate );
                        item->toSong()->setDuration(duration);
                    }
                    if (item->toAlbum()) item->toAlbum()->setGenre(value.toString());
                    if (item->toArtist()) item->toArtist()->setCountry(value.toString());
                    break;
                case 2:
                    if (item->toSong()) item->toSong()->setRate(value.toInt());
                    if (item->toAlbum()) item->toAlbum()->setYear(value.toInt());
                    if (item->toArtist()) item->toArtist()->setComment(value.toString());
                    break;
                case 3:
                    if (item->toAlbum()) item->setComment(value.toString());
                    if (item->toSong()) item->setComment(value.toString());
                    if (item->toArtist()) item->toArtist()->setPhoto(value.value<QPixmap>());
                    break;
                case 4:
                    if (item->toAlbum()) item->toAlbum()->setCover(value.value<QPixmap>());
            }
            emit dataChanged(index, index);
            return true;
        }
    }
    return false;

}

Qt::ItemFlags MusicModel::flags(const QModelIndex &index) const {
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool MusicModel::insertRow(int row, const QModelIndex &parent) {
    Item *p_item;
    if(parent.isValid()) p_item = static_cast<Item *>(parent.internalPointer());
    else p_item = m_root;
    Item* item;
    if(p_item==m_root) item = new Artist();
    if(p_item->toArtist()) item = new Album();
    if(p_item->toAlbum()) item = new Song();
    beginInsertRows(parent, row, row);
    p_item->insertChild(item, row);
    endInsertRows();
    return true;
}

bool MusicModel::removeRow(int row, const QModelIndex &parent){
    Item *p_item;
    if(parent.isValid()) p_item = static_cast<Item *>(parent.internalPointer());
    else p_item = m_root;
    if(p_item) {
        beginRemoveRows(parent, row, row);
        p_item->takeChild(row);
        endRemoveRows();
        return true;
    }
    else return false;

}

QDataStream& operator<< (QDataStream &stream, Item &root) {
    stream << root.childCount();
    for(int i=0; i < root.childCount(); i++) {
        Artist const* artist = root.childAt(i)->toArtist();
        if (artist) stream << *artist;
    }
    return stream;
}

QDataStream& operator>> (QDataStream &stream, Item &root) {
    int artist_count;
    Artist *art;
    stream >> artist_count;
    for (int i =0; i<artist_count; i++) {
        art = new Artist();
        stream >> *art;
        root.insertChild(art,i);
    }
    return stream;
}
