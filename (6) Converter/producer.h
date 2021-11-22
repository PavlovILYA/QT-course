#ifndef PRODUCER_H
#define PRODUCER_H

#include "buffer.h"

#include <QThread>
#include <QSemaphore>
#include <QFile>
#include <QtGlobal>
#include <QDebug>
#include <QObject>


class Producer : public QThread
{
    Q_OBJECT
public:
    explicit Producer(QString f1, QString f2, QString f3, int s1, int s2, int s3,
             Buffer * buf,
             QSemaphore * freeBytes,
             QSemaphore * rawBytes, QMutex * mutex, QObject *parent = nullptr);

signals:
    void bufferChanged();

    // QThread interface
protected:
    void run() override;

private:
    int DataSize;
    int BufSize;

    QString FileName1;
    int FileSize1;
    QString FileName2;
    int FileSize2;
    QString FileName3;
    int FileSize3;

    Buffer * m_buffer;
    QSemaphore * m_freeBytes; // место свободное, для записи продюсером
    QSemaphore * m_rawBytes; // место, куда продюсер уже записал данные
    QMutex * m_mutex;
};

#endif // PRODUCER_H
