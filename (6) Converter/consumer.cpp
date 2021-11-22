#include "consumer.h"

Consumer::Consumer(int s1, int s2, int s3,
                   Buffer * buf,
                   QSemaphore * readyBytes,
                   QSemaphore * freeBytes, QMutex * mutex, QObject *parent) :
    QThread(parent),
    FileSize1(s1),
    FileSize2(s2),
    FileSize3(s3),
    m_buffer(buf),
    m_readyBytes(readyBytes),
    m_freeBytes(freeBytes),
    m_mutex(mutex)
{
    //FileSize1 = s1;
    //FileSize2 = s2;
    //FileSize3 = s3;
    DataSize = s1 + s2 + s3;
    //m_buffer = buf;
    BufSize = m_buffer->getSize();
    //m_readyBytes = readyBytes;
    //m_freeBytes = freeBytes;
    qDebug() << "start Consumer thread:" << QThread::currentThread();
}

void Consumer::run()
{
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    qDebug() << "Run called Consumer thread:"     << QThread::currentThread();
    char current;
    CustomData cd;
    int y = 0;
    while(count1+count2+count3 != DataSize)
    {
        qDebug() << "Consumer is awaiting: " << m_readyBytes->available();
        m_readyBytes->acquire();
        qDebug() << "Consumer is waking: " << m_readyBytes->available();

        for (int j=0; j<BufSize; j++)
        {
            qDebug() << "Consumer is searching " << count1 << "/" << FileSize1 << " " << count2 << "/" << FileSize2 << " " << count3 << "/" << FileSize3 << " ";
            if ((m_buffer->getStateOne(j) == 3) && (count1 == m_buffer->getNumberOne(j)) && (m_buffer->getTypeOne(j) == 1)) //по порядку из файла 1
            {
                current = m_buffer->getChar(j);
                y++;
                count1++;
                m_buffer->setStateOne(j, 1);
                int x = 0;
                for( int g=0; g<50000000; g++)
                {
                    x++;
                    x--;
                    x++;
                }
                qDebug() << "I Consumer CHANGE BUFFER    my thread:" << QThread::currentThread();
                emit Consumer::bufferChanged();
                cd.set(1, current);
                emit readyByte(cd);
                m_freeBytes->release();
                break;
            }
            if ((m_buffer->getStateOne(j) == 3) && (count2 == m_buffer->getNumberOne(j)) && (m_buffer->getTypeOne(j) == 2))// по порядку из файла 2
            {
                current = m_buffer->getChar(j);
                y++;
                count2++;
                m_buffer->setStateOne(j, 1);
                int x = 0;
                for( int g=0; g<50000000; g++)
                {
                    x++;
                    x--;
                    x++;
                }
                qDebug() << "I Consumer CHANGE BUFFER    my thread:" << QThread::currentThread();
                emit Consumer::bufferChanged();
                cd.set(2, current);
                emit readyByte(cd);
                m_freeBytes->release();
                break;
            }
            if ((m_buffer->getStateOne(j) == 3) && (count3 == m_buffer->getNumberOne(j)) && (m_buffer->getTypeOne(j) == 3))//по порядку из файла 3
            {
                current = m_buffer->getChar(j);
                y++;
                count3++;
                m_buffer->setStateOne(j, 1);
                int x = 0;
                for( int g=0; g<50000000; g++)
                {
                    x++;
                    x--;
                    x++;
                }
                qDebug() << "I Consumer CHANGE BUFFER    my thread:" << QThread::currentThread();
                emit Consumer::bufferChanged();
                cd.set(3, current);
                emit readyByte(cd);
                m_freeBytes->release();
                break;
            }
        }
    }

    while(m_readyBytes->available() != 0)
        m_readyBytes->acquire(1);

    qDebug() << "I Consumer I HAVE DONE" << QThread::currentThread() << " ready " << m_readyBytes->available();
    qDebug() << "I Consumer I HAVE DONE" << QThread::currentThread() << " free " << m_freeBytes->available();
}
