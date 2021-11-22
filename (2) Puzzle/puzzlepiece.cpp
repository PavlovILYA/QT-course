#include "puzzlepiece.h"

PuzzlePiece::PuzzlePiece(ConnectorPosition north, ConnectorPosition east, ConnectorPosition south, ConnectorPosition west)
{
    connectors[NORTH] = north;
    connectors[EAST] = east;
    connectors[SOUTH] = south;
    connectors[WEST] = west;

    neighbors[NORTH] = nullptr;
    neighbors[EAST] = nullptr;
    neighbors[SOUTH] = nullptr;
    neighbors[WEST] = nullptr;

    QPainterPath paintPath;
    constructShape(paintPath);
    setPath(paintPath);
}

void PuzzlePiece::constructShape(QPainterPath &paintPath)
{
    paintPath.moveTo(-pieceSize/2, -pieceSize/2);
    switch (connectors[NORTH]){
    case NONE:
        paintPath.lineTo(pieceSize/2, -pieceSize/2);
        break;
    case IN:
        paintPath.lineTo(-pieceSize/6, -pieceSize/2);
        paintPath.arcTo(-pieceSize/6, -2*pieceSize/3, pieceSize/3, pieceSize/3, 180, 180);
        paintPath.lineTo(pieceSize/2, -pieceSize/2);
        break;
    case OUT:
        paintPath.lineTo(-pieceSize/6, -pieceSize/2);
        paintPath.arcTo(-pieceSize/6, -2*pieceSize/3, pieceSize/3, pieceSize/3, 180, -180);
        paintPath.lineTo(pieceSize/2, -pieceSize/2);
        break;
    }
    switch (connectors[EAST]){
    case NONE:
        paintPath.lineTo(pieceSize/2, pieceSize/2);
        break;
    case IN:
        paintPath.lineTo(pieceSize/2, -pieceSize/6);
        paintPath.arcTo(pieceSize/3, -pieceSize/6, pieceSize/3, pieceSize/3, 90, 180);
        paintPath.lineTo(pieceSize/2, pieceSize/2);
        break;
    case OUT:
        paintPath.lineTo(pieceSize/2, -pieceSize/6);
        paintPath.arcTo(pieceSize/3, -pieceSize/6, pieceSize/3, pieceSize/3, 90, -180);
        paintPath.lineTo(pieceSize/2, pieceSize/2);
        break;
    }
    switch (connectors[SOUTH]){
    case NONE:
        paintPath.lineTo(-pieceSize/2, pieceSize/2);
        break;
    case IN:
        paintPath.lineTo(pieceSize/6, pieceSize/2);
        paintPath.arcTo(-pieceSize/6, pieceSize/3, pieceSize/3, pieceSize/3, 0, 180);
        paintPath.lineTo(-pieceSize/2, pieceSize/2);
        break;
    case OUT:
        paintPath.lineTo(pieceSize/6, pieceSize/2);
        paintPath.arcTo(-pieceSize/6, pieceSize/3, pieceSize/3, pieceSize/3, 0, -180);
        paintPath.lineTo(-pieceSize/2, pieceSize/2);
        break;
    }
    switch (connectors[WEST]){
    case NONE:
        paintPath.lineTo(-pieceSize/2, -pieceSize/2);
        break;
    case IN:
        paintPath.lineTo(-pieceSize/2, pieceSize/6);
        paintPath.arcTo(-2*pieceSize/3, -pieceSize/6, pieceSize/3, pieceSize/3, -90, 180);
        paintPath.lineTo(-pieceSize/2, -pieceSize/2);
        break;
    case OUT:
        paintPath.lineTo(-pieceSize/2, pieceSize/6);
        paintPath.arcTo(-2*pieceSize/3, -pieceSize/6, pieceSize/3, pieceSize/3, -90, -180);
        paintPath.lineTo(-pieceSize/2, -pieceSize/2);
        break;
    }
    paintPath.closeSubpath();
}

void PuzzlePiece::setPixmap(QPixmap newImage)
{
    image = newImage;
    update();
}

QPixmap PuzzlePiece::pixmap()
{
    return image;
}

void PuzzlePiece::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setPen(QPen(Qt::black, 0.5));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPath(path());
    painter->setClipPath(path());
    QRect rect = boundingRect().toRect();
    painter->drawPixmap(rect.x(), rect.y(), pixmap());
}

QPoint PuzzlePiece::coordinates()
{
    return _coordinates;
}

void PuzzlePiece::setCoordinates(QPoint newCoordinates)
{
    _coordinates = newCoordinates;
}

void PuzzlePiece::link(PuzzlePiece* neighbor, Diretcion dir)
{
    neighbors[dir] = neighbor;
    switch(dir){
        case NORTH:
            neighbor->neighbors[SOUTH] = this;
            break;
        case EAST:
            neighbor->neighbors[WEST] = this;
            break;
        case SOUTH:
            neighbor->neighbors[NORTH] = this;
            break;
        case WEST:
            neighbor->neighbors[EAST] = this;
            break;
    }
}

void PuzzlePiece::findneighbor(PuzzlePiece::Diretcion dir)
{
    if (neighbors[dir]) return;
    PuzzlePiece* element;
    switch(dir){
        case NORTH:
            element = (PuzzlePiece*)scene()->itemAt(pos().x(), pos().y() - pieceSize, QTransform());
            if (element && (coordinates() - QPoint(0, 1)) == element->coordinates() && abs(pos().x() - element->pos().x()) < 10 &&
                                                                                  abs(pos().y() - pieceSize - element->pos().y()) < 10)
            {
                link(element, NORTH);
                element->setPos(pos().x(), pos().y() - pieceSize);
            }
            break;
        case EAST:
            element = (PuzzlePiece*)scene()->itemAt(pos().x() + pieceSize, pos().y(), QTransform());
            if (element && (coordinates() + QPoint(1, 0)) == element->coordinates() && abs(pos().x() + pieceSize - element->pos().x()) < 10 &&
                                                                                   abs(pos().y() - element->pos().y()) < 10)
            {
                link(element, EAST);
                element->setPos(pos().x() + pieceSize, pos().y());
            }
            break;
        case SOUTH:
            element = (PuzzlePiece*)scene()->itemAt(pos().x(), pos().y() + pieceSize, QTransform());
            if (element && (coordinates() + QPoint(0, 1)) == element->coordinates() && abs(pos().x() - element->pos().x()) < 10 &&
                                                                                  abs(pos().y() + pieceSize - element->pos().y()) < 10)
            {
                link(element, SOUTH);
                element->setPos(pos().x(), pos().y() + pieceSize);
            }
            break;
        case WEST:
            element = (PuzzlePiece*)scene()->itemAt(pos().x() - pieceSize, pos().y(), QTransform());
            if (element && (coordinates() - QPoint(1, 0)) == element->coordinates() && abs(pos().x() - pieceSize - element->pos().x()) < 10 &&
                                                                                  abs(pos().y() - element->pos().y()) < 10)
            {
                link(element, WEST);
                element->setPos(pos().x() - pieceSize, pos().y());
            }
            break;
    }
}

void PuzzlePiece::checkNeighbors(QSet<QPoint> &checked)
{
    if(checked.contains(coordinates()))
        return; // условия прерывания рекурсии
    checked.insert(coordinates()); // запомнить посещенный элемент
    findneighbor(NORTH);     	// найти N соседа
    findneighbor(EAST);      	// найти E соседа
    findneighbor(SOUTH);     	// найти S соседа
    findneighbor(WEST);      	// найти W соседа

    // рекурсивные вызовы:
    if(neighbors[NORTH])
        neighbors[NORTH]->checkNeighbors(checked);
    if(neighbors[EAST])
        neighbors[EAST]->checkNeighbors(checked);
    if(neighbors[SOUTH])
        neighbors[SOUTH]->checkNeighbors(checked);
    if(neighbors[WEST])
        neighbors[WEST]->checkNeighbors(checked);
}

void PuzzlePiece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QSet<QPoint> set;
    checkNeighbors(set);
    if(set.count() == number)
        QMessageBox::information(0, QString("Поздравляем!"), QString("Вы победили!"));
    QGraphicsItem::mouseReleaseEvent(event);
}

void PuzzlePiece::setNumber(const QSize &s)
{
    number = s.width() * s.height();
}

QVariant PuzzlePiece::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change==ItemPositionHasChanged)
    {
        QPoint newPose=value.toPoint();
        if (neighbors[NORTH])
            neighbors[NORTH]->setPos(newPose.x(),newPose.y()-pieceSize);
        if (neighbors[SOUTH])
            neighbors[SOUTH]->setPos(newPose.x(),newPose.y()+pieceSize);
        if (neighbors[WEST])
            neighbors[WEST]->setPos(newPose.x()-pieceSize,newPose.y());
        if (neighbors[EAST])
            neighbors[EAST]->setPos(newPose.x()+pieceSize,newPose.y());
    }
    return QGraphicsItem::itemChange(change, value);
}

inline uint qHash (const QPoint & key)
{
    return qHash (QPair<int,int>(key.x(), key.y()) );
}
