#include "puzzlesizewidget.h"

PuzzleSizeWidget::PuzzleSizeWidget(QWidget *parent) : QFrame(parent)
{
    size = QSize(2,2);
    min = QSize(2,2);
    max = QSize(8,8);
}

QSize PuzzleSizeWidget::sizeHint() const {
   return QSize(300,300);
}

QSize PuzzleSizeWidget::value() const {
   return size;
}

QSize PuzzleSizeWidget::maximum() const
{
    return max;
}

QSize PuzzleSizeWidget::minimum() const
{
    return min;
}

QPixmap PuzzleSizeWidget::preview_f() const
{
    return preview;
}

void PuzzleSizeWidget::setValue(const QSize &s) {
   if(size == s)
       return;
   QSize old = size;
   if(s.height()<min.height()||s.width()<min.width()||s.height()>max.height()||s.width()>max.width())
       return;
   size = s;
   emit valueChanged(s);
   if(old.width() != s.width())
       emit horizontalValueChanged(s.width());
   if(old.height() != s.height())
       emit verticalValueChanged(s.height());
   update();

}

void PuzzleSizeWidget::setMinimum(const QSize &s)
{
    if(s==min)
        return;
    if ((size.height()<=s.height())&&(size.width()<=s.width()))
        min=s;
    update();

}

void PuzzleSizeWidget::setMaximum(const QSize &s)
{
    if(s==max)
        return;
    if ((size.height()<=s.height())&&(size.width()<=s.width()))
        max=s;
    update();
}

void PuzzleSizeWidget::setPreview(const QPixmap &s)
{
    preview=s;
    update();
}

void PuzzleSizeWidget::paintEvent(QPaintEvent *)
{
   QPainter painter(this);
   renderValue(&painter);
   renderGrid(&painter);
}

QSize PuzzleSizeWidget::cellSize() const {
   int w = width();
   int h = height();
   int mw = maximum().width();
   int mh = maximum().height();
   int extent = qMin(w/mw, h/mh);
   return QSize(extent,extent).expandedTo(QApplication::globalStrut()).expandedTo(QSize(4,4));
}

void PuzzleSizeWidget::renderValue(QPainter *painter)
{
    QSize cell_size = cellSize();
    if(preview.isNull())
    {
        painter->setBrush(Qt::gray);
        painter->drawRect(0,0,value().width()*cell_size.width(),value().height()*cell_size.height());
    }
    else
    {
        painter->drawPixmap(0,0,preview.scaled(cell_size.width()*value().width(), cell_size.height()*value().height()));
    }

}

void PuzzleSizeWidget::renderGrid(QPainter *painter)
{
    painter->setBrush(Qt::NoBrush);
    QSize cell_size = cellSize();
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            painter->drawRect(i*cell_size.width(), j*cell_size.width(), cell_size.width(), cell_size.height());
}

QPoint PuzzleSizeWidget::cellAt(QPoint &dot)
{
    QSize cell_size = cellSize();
    int x = dot.x() / cell_size.width() + 1;
    int y = dot.y() / cell_size.height() + 1;
    QPoint new_dot = QPoint(x,y);
    return new_dot;
}

void PuzzleSizeWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint dot = event->pos();
    QPoint cell = cellAt(dot);
    if(dot.x()<width() && dot.y()<height())
    {
        setValue(QSize(cell.x(), cell.y()));
    }
}

void PuzzleSizeWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint dot = event->pos();
    QPoint cell = cellAt(dot);
    if(dot.x()<width() && dot.y()<height())
    {
        setValue(QSize(cell.x(), cell.y()));
    }
}
