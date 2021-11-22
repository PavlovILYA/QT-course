#ifndef PUZZLESIZEWIDGET_H
#define PUZZLESIZEWIDGET_H

#include <QWidget>
#include <QFrame>
#include<QPixmap>
#include<QPainter>
#include<QApplication>
#include<QSize>
#include<QMouseEvent>

class PuzzleSizeWidget : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(QSize value_l READ value_l WRITE setValue)
    Q_PROPERTY(QSize minimum_l READ minimum_l WRITE setMinimum)
    Q_PROPERTY(QSize maximum_l READ maximum_l WRITE setMaximum)
    Q_PROPERTY(QPixmap preview_l READ preview_l WRITE setPreview)

public:
    explicit PuzzleSizeWidget(QWidget *parent = nullptr);
    QSize sizeHint() const;
    void paintEvent(QPaintEvent *);

    QSize value_l(){return size;}
    QSize minimum_l(){return min;}
    QSize maximum_l(){return max;}
    QPixmap preview_l(){return preview;}

    QSize size;
    QSize max;
    QSize min;
    QPixmap preview;
    QSize value() const;
    QSize maximum() const;
    QSize minimum() const;
    QPixmap preview_f() const;

    void renderValue(QPainter *painter);
    void renderGrid(QPainter *painter);
    QSize cellSize() const;
    QPoint cellAt(QPoint &dot);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void valueChanged(const QSize&);
    void horizontalValueChanged(int);
    void verticalValueChanged(int);

public slots:
    void setValue(const QSize &s);
    void setMinimum(const QSize& s);
    void setMaximum(const QSize& s);
    void setPreview(const QPixmap& s);
};

#endif // PUZZLESIZEWIDGET_H
