#include "ratingwidget.h"

RatingWidget::RatingWidget(QWidget *parent) :
    QWidget(parent)
{
    setMouseTracking(true);
    setAutoFillBackground(true);
}

void RatingWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QBrush b_in(Qt::magenta);
    QBrush b_out(Qt::NoBrush);
    painter.setPen(QPen(Qt::black, 1));
    int rate = 0;
    rate = m_rate;
    if(rate > 5) rate = 5;
    if(rate < 0) rate = 0;
    int top;
    int left;
    top = 0;
    left = 0;
    for(int counter = 0; counter < 5; counter++) {
        if(rate > 0 && counter < rate)
            painter.setBrush(b_in);
        else
            painter.setBrush(b_out);
        painter.drawEllipse(QPoint(left + 10, top + 10), 8, 8);
        left += 20;
    }
}

void RatingWidget::mouseMoveEvent(QMouseEvent *event) {
    int rate = rateAt(event->x());
    if (rate != -1) {
        m_rate = rate;
        update();
    }
}

void RatingWidget::mouseReleaseEvent(QMouseEvent * event) {
    emit editingFinished();
}

int RatingWidget::rateAt(int x) {
    int rate = (x / (100 / 5)) + 1;
    if (rate < 0 || rate > 5)
        return -1;
    return rate;
}
