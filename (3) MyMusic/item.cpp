#include "item.h"

Item::Item() {
    m_parent = nullptr;
    m_name = QString("Unknown");
}

Item::Item(QString name) {
    m_parent = nullptr;
    m_name = name;
}

Item::~Item() {
    foreach(Item* p, m_children) {
        if (p) {
            delete p;
        }
    }
    m_children.clear();
}

void Item::setParent(Item *p) {
    if(m_parent == p) {
        return;
    }
    if(p) {
        if(m_parent) {
            m_parent->takeChild(m_parent->indexOf(p));
        }
        m_parent = p;
        m_parent->insertChild(this);
    }
    else m_parent=p;
}

void Item::insertChild(Item *c, int pos) {
    if(!c) {
        return;
    }
    if(m_children.contains(c)) {
        c->setParent(this);
        return;
    }
    if(pos < 0 | pos > m_children.size()) {
        m_children.insert(m_children.size(), c);
    }
    else {
        m_children.insert(pos, c);
    }
    c->setParent(this);
}

Item* Item::takeChild(int pos) {
    Item* c = m_children.takeAt(pos);
    if(!c) return c;
    else {
        c->setParent(nullptr);
        return c;
    }
}

Item *Item::childAt(int pos) const {
    Item* p = m_children.at(pos);
    return p;
}

QDataStream& operator<< (QDataStream &stream, const Artist &artist) {
    stream << artist.name()
           << artist.photo()
           << artist.country()
           << artist.comment();
    stream << artist.childCount();
    for (int i=0; i < artist.childCount(); i++) {
        Album const* album = artist.childAt(i)->toAlbum();
        if (album) {
            stream << *album;
        }
    }
    return stream;
}

QDataStream& operator<< (QDataStream &stream, const Album &album)
{
    stream << album.name()
           << album.cover()
           << album.genre()
           << album.year()
           << album.comment();
    stream << album.childCount();
    for (int i=0; i < album.childCount(); i++) {
        Song const* song = album.childAt(i)->toSong();
        if (song) {
            stream << *song;
        }
    }
    return stream;
}

QDataStream& operator<< (QDataStream &stream, const Song &song)
{
    stream << song.name()
           << song.duration()
           << song.rate()
           << song.comment();
    return stream;
}

QDataStream &operator >>(QDataStream &stream, Artist &artist)
{
    QString name;
    QPixmap photo;
    QString comment;
    QString country;
    int album_count;
    Album *alb;

    stream >> name
           >> photo
           >> country
           >> comment;
    artist.setName(name);
    artist.setCountry(country);
    artist.setPhoto(photo);
    artist.setComment(comment);

    stream >> album_count;
    for (int i =0; i<album_count; i++) {
        alb = new Album();
        stream >> *alb;
        artist.insertChild(alb,i);
    }
    return stream;
}

QDataStream &operator >>(QDataStream &stream, Album &album)
{

    QString name;
    QPixmap cover;
    QString genre;
    int year;
    QString comment;
    int song_count;
    Song *sng;

    stream >> name
           >> cover
           >> genre
           >> year
           >> comment;
    album.setName(name);
    album.setCover(cover);
    album.setGenre(genre);
    album.setYear(year);
    album.setComment(comment);

    stream >> song_count;
    for (int i =0; i<song_count; i++) {
        sng = new Song();
        stream >> *sng;
        album.insertChild(sng, i);
    }
    return stream;
}

QDataStream &operator >>(QDataStream &stream, Song &song)
{


    QString name;
    QTime duration;
    QString comment;
    int rate;

    stream >> name
           >> duration
           >> rate
           >> comment;
    song.setName(name);
    song.setDuration(duration);
    song.setRate(rate);
    song.setComment(comment);

    return stream;
}
