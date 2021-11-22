#ifndef PUZZLEPIECE_H
#define PUZZLEPIECE_H
#include <QGraphicsPathItem>
#include <QPainter>
#include <QPainterPath>
#include <QSet>
#include <QMessageBox>
#include <QGraphicsScene>
#include "math.h"

class PuzzlePiece : public QGraphicsPathItem
{
public:
    enum Diretcion {NORTH, SOUTH, WEST, EAST};
    enum ConnectorPosition {NONE, OUT, IN};
    PuzzlePiece(ConnectorPosition north, ConnectorPosition east, ConnectorPosition south, ConnectorPosition west);
    ConnectorPosition connectors[4];
    void setPixmap(QPixmap newImage);
    QPixmap pixmap();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    qreal pieceSize = 100;
    QPoint _coordinates;
    QPoint coordinates();
    int number;
    void setCoordinates(QPoint);
    void setNumber(const QSize &s);
    void link(PuzzlePiece *neighbor, Diretcion dir);
    void checkNeighbors(QSet<QPoint> &checked);
    void findneighbor(Diretcion);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;


private:
    void constructShape(QPainterPath &);
    QPixmap image;
    //QPixmap image = QPixmap("/Users/pavlov/Desktop/Фото 21.11.2019, 22.12.jpg");
    PuzzlePiece* neighbors[4];
};

#endif // PUZZLEPIECE_H
