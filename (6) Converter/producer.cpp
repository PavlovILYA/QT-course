#include "producer.h"

Producer::Producer(QString f1, QString f2, QString f3, int s1, int s2, int s3,
                   Buffer * buf,
                   QSemaphore * freeBytes,
                   QSemaphore * rawBytes, QMutex * mutex, QObject *parent) : QThread(parent)
{
    FileName1 = f1;
    FileName2 = f2;
    FileName3 = f3;
    FileSize1 = s1;
    FileSize2 = s2;
    FileSize3 = s3;
    DataSize = s1 + s2 + s3;
    m_buffer = buf;
    BufSize = m_buffer->getSize();
    m_freeBytes = freeBytes;
    m_rawBytes = rawBytes;
    m_mutex = mutex;
    qDebug() << "start Producer thread:" << QThread::currentThread();
}

void Producer::run()
{
    qDebug() << "Run called Producer thread:"     << QThread::currentThread();
    QFile file1(FileName1);
    file1.open(QIODevice::ReadOnly);
    QFile file2(FileName2);
    file2.open(QIODevice::ReadOnly);
    QFile file3(FileName3);
    file3.open(QIODevice::ReadOnly);
    char current;
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;

    for (int i=0; i<qMax(qMax(FileSize1, FileSize2), FileSize3); i++)
    {
        if (file1.getChar(&current))   // если еще есть символы, прочитали из 1 файла
        {
            m_freeBytes->acquire();  //дождались, когда будут свободные ячейки в буфере
            for (int j=0; j<BufSize; j++)
            {
                if (m_buffer->getStateOne(j) == 1) // если ячейка свободна
                {
                    m_buffer->setChar(j, current);
                    m_buffer->setTypeOne(j, 1);
                    m_buffer->setStateOne(j, 2);
                    m_buffer->setNumberOne(j, count1);
                    count1++;
                    qDebug() << "I Producer CHANGE BUFFER    my thread:" << QThread::currentThread();
                    emit bufferChanged();
                    m_rawBytes->release();
                    break;
                }
            }
        }
        if (file2.getChar(&current))
        {
            m_freeBytes->acquire();
            for (int j=0; j<BufSize; j++)
            {
                if (m_buffer->getStateOne(j) == 1)
                {
                    m_buffer->setChar(j, current);
                    m_buffer->setTypeOne(j, 2);
                    m_buffer->setStateOne(j, 2);
                    m_buffer->setNumberOne(j, count2);
                    count2++;
                    qDebug() << "I Producer CHANGE BUFFER    my thread:" << QThread::currentThread();
                    emit bufferChanged();
                    m_rawBytes->release();
                    break;
                }
            }
        }
        if (file3.getChar(&current))
        {
            m_freeBytes->acquire();
            for (int j=0; j<BufSize; j++)
            {
                if (m_buffer->getStateOne(j) == 1)
                {
                    m_buffer->setChar(j, current);
                    m_buffer->setTypeOne(j, 3);
                    m_buffer->setStateOne(j, 2);
                    m_buffer->setNumberOne(j, count3);
                    count3++;
                    qDebug() << "I Producer CHANGE BUFFER    my thread:" << QThread::currentThread();
                    emit bufferChanged();
                    m_rawBytes->release();
                    break;
                }
            }
        }
    }

    file1.close();
    file2.close();
    file3.close();
    qDebug() << "I Consumer I HAVE DONE" << QThread::currentThread() << " free " << m_freeBytes->available();
    qDebug() << "I Consumer I HAVE DONE" << QThread::currentThread() << " raw " << m_rawBytes->available();

}
