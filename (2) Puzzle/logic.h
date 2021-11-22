#ifndef LOGIC_H
#define LOGIC_H
#include "puzzlepiece.h"
#include <QGraphicsScene>
#include <QVector>
#include <QRect>
#include <QSize>
#include <QPixmap>
#include <QtGlobal>


class logic : public QGraphicsScene
{
public:
    logic();
    void setup(const QSize &, const QPixmap &);
    PuzzlePiece::ConnectorPosition reverse(PuzzlePiece::ConnectorPosition);
};

#endif // LOGIC_H
