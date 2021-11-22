#ifndef WIDGET_H
#define WIDGET_H

#include "consumer.h"
#include "producer.h"
#include "worker.h"
#include "buffer.h"
#include "customdata.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QObject>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QMainWindow
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

signals:

public slots:
    void printBuffer();
    void printChar(const CustomData&cd);

private slots:
    void on_pushButtonFile1_clicked();

    void on_pushButtonFile2_clicked();

    void on_pushButtonFile3_clicked();

    void on_pushButtonStart_clicked();

    void finished();

private:
    Ui::Widget *ui;

    QString FileName1;
    int FileSize1 = -1;
    QString FileName2;
    int FileSize2 = -1;
    QString FileName3;
    int FileSize3 = -1;

    int DataSize;
    int BufSize;
    Buffer * buffer;

    QMutex * m_mutex;
    QSemaphore * freeBytes;
    QSemaphore * rawBytes;
    QSemaphore * readyBytes;
    Producer * producer;
    Consumer * consumer;
    Worker * worker;
};
#endif // WIDGET_H
