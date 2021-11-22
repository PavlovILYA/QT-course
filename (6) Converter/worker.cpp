#include "worker.h"

Worker::Worker(int s1, int s2, int s3,
               Buffer * buf,
               QSemaphore * rawBytes,
               QSemaphore * readyBytes, QMutex * mutex, QObject *parent) : QThread(parent)
{
    FileSize1 = s1;
    FileSize2 = s2;
    FileSize3 = s3;
    DataSize = s1 + s2 + s3;
    m_buffer = buf;
    BufSize = m_buffer->getSize();
    m_rawBytes = rawBytes;
    m_readyBytes = readyBytes;
    m_mutex = mutex;
    qDebug() << "start Worker thread:" << QThread::currentThread();
}

void Worker::run()
{
    qDebug() << "Run called Worker thread:"     << QThread::currentThread();
    char current;
    for (int i=0; i<DataSize; i++)
    {
        m_rawBytes->acquire();


        for (int j=0; j<BufSize; j++)
        {
            if (m_buffer->getStateOne(j) == 2)
            {
                current = m_buffer->getChar(j);
                if (islower(current))
                    current = toupper(current);
                else
                    current = tolower(current);
                // current inverse
                m_buffer->setChar(j, current);
                m_buffer->setStateOne(j, 3);
//                int x = 0;
//                for( int g=0; g<1000000; g++)
//                {
//                    x++;
//                    x--;
//                    x++;
//                }
                qDebug() << "I Worker CHANGE BUFFER    my thread:" << QThread::currentThread();
                emit bufferChanged();
                int x = 0;
                for( int g=0; g<70000000; g++)
                {
                    x++;
                    x--;
                    x++;
                }
                qDebug() << "I Worker. increasing is near: " << m_readyBytes->available();
                m_readyBytes->release();
                qDebug() << "I Worker. Increasing has done: " << m_readyBytes->available();

                break;
            }
        }
    }
    m_readyBytes->release(DataSize);
    qDebug() << "I Consumer I HAVE DONE" << QThread::currentThread() << " raw " << m_rawBytes->available();
    qDebug() << "I Consumer I HAVE DONE" << QThread::currentThread() << " ready " << m_readyBytes->available();

}
