#ifndef CONSUMER_H
#define CONSUMER_H

#include "buffer.h"
#include "customdata.h"

#include <QThread>
#include <QSemaphore>
#include <QDebug>
#include <QObject>


class Consumer : public QThread
{
    Q_OBJECT
public:

    explicit Consumer(int s1, int s2, int s3,
             Buffer * buf,
             QSemaphore * readyBytes,
             QSemaphore * freeBytes, QMutex * mutex, QObject *parent = nullptr);

signals:
    void bufferChanged();
    void readyByte(const CustomData&cd);

public slots:

    // QThread interface
protected:
    void run() override;

private:
    int DataSize;
    int BufSize;

    int FileSize1;
    int FileSize2;
    int FileSize3;

    Buffer * m_buffer;
    QSemaphore * m_readyBytes; // место, еще не освобожденное консьюмером
    QSemaphore * m_freeBytes; // место, освобожденное консьюмером
    QMutex * m_mutex;
};

#endif // CONSUMER_H
