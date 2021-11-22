#ifndef WORKER_H
#define WORKER_H

#include "buffer.h"

#include <QThread>
#include <QSemaphore>
#include <QDebug>
#include <QObject>


class Worker : public QThread
{
    Q_OBJECT
public:
    explicit Worker(int s1, int s2, int s3,
           Buffer * buf,
           QSemaphore * rawBytes,
           QSemaphore * readyBytes, QMutex * mutex, QObject *parent = nullptr);

signals:
    void bufferChanged();

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
    QSemaphore * m_rawBytes; // место, необработанное воркером
    QSemaphore * m_readyBytes; // место, обработанное воркером
    QMutex * m_mutex;
};

#endif // WORKER_H
