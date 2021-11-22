#include "logic.h"

logic::logic()
{

}

// получение парного коннектора
PuzzlePiece::ConnectorPosition logic::reverse(PuzzlePiece::ConnectorPosition pos)
{
    switch(pos){
        case PuzzlePiece::NONE: return PuzzlePiece::NONE;
        case PuzzlePiece::IN: return PuzzlePiece::OUT;
        case PuzzlePiece::OUT: return PuzzlePiece::IN;
    }
    return PuzzlePiece::NONE; // safeguard
}

void logic::setup(const QSize &size, const QPixmap &image)
{
    this->clear();

    QPixmap boundenImage = image.scaled(100*size.width(), 100*size.height());

    PuzzlePiece::ConnectorPosition storedWest = PuzzlePiece::NONE;
    QVector<PuzzlePiece::ConnectorPosition> prev(size.width(), PuzzlePiece::NONE);
    for(int row = 0; row < size.height(); ++row)
    {
        storedWest = PuzzlePiece::NONE;
        for(int col = 0; col < size.width(); ++col)
        {
            PuzzlePiece::ConnectorPosition curr[4]; // N, E, S, W

            curr[PuzzlePiece::NORTH] = reverse(prev[col]);
            curr[PuzzlePiece::EAST] = qrand() % 2 ? PuzzlePiece::IN : PuzzlePiece::OUT;
            curr[PuzzlePiece::SOUTH] = qrand() % 2 ? PuzzlePiece::IN : PuzzlePiece::OUT;
            curr[PuzzlePiece::WEST] = reverse(storedWest);

            if(col==size.width()-1)
                curr[PuzzlePiece::EAST] = PuzzlePiece::NONE;
            if(row==size.height()-1)
                curr[PuzzlePiece::SOUTH] = PuzzlePiece::NONE;

            PuzzlePiece *piece = new PuzzlePiece(curr[PuzzlePiece::NORTH], curr[PuzzlePiece::EAST],
                                                                  curr[PuzzlePiece::SOUTH], curr[PuzzlePiece::WEST]);

            piece->setCoordinates(QPoint(col, row));

            addItem(piece); // add item to scene
            piece->setFlag(QGraphicsItem::ItemIsMovable);
            piece->setFlag(QGraphicsItem::ItemSendsGeometryChanges);

            //piece->setPos(col*100, row*100); // ideal positions
            piece->setPos(size.width()*100*RAND_MAX/qrand(), size.height()*100*RAND_MAX/qrand());

            storedWest = curr[PuzzlePiece::EAST]; // store east for next column
            prev[col] = curr[PuzzlePiece::SOUTH];  // store south for next row

            QRect rect = piece->boundingRect().toRect(); // (0,0) в центре
            const int cellSize = 100;
            rect.translate(0.5*cellSize+col*cellSize, 0.5*cellSize+row*cellSize);
            QPixmap px = boundenImage.copy(rect);
            piece->setPixmap(px);
            piece->setNumber(size);


        }
    }
}
