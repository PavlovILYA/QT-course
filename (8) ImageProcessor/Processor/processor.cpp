#include "processor.h"

#include <QDebug>
#include <QFile>
#include <QImage>
#include <QQmlEngine>
#include <QThread>
#include <QtConcurrent>
#include <QtQuick/QQuickImageProvider>
#include <QQmlContext>


Processor::Processor(QObject* parent)
    : QObject(parent)
{

    QObject::connect(&m_medianFilter, &MedianFilter::started, this, &Processor::onProcessingStarted);
    QObject::connect(&m_medianFilter, &MedianFilter::progress, this, &Processor::onProgressChanged);
    QObject::connect(&m_medianFilter, &MedianFilter::stopped, this, &Processor::onProcessingStopped);

    QObject::connect(&m_medianFilter, &MedianFilter::filterRadiusChanged, this, &Processor::onMedianFilterRadiusChanged);

    m_threadsCount = QThreadPool::globalInstance()->maxThreadCount();
}

int Processor::threadsCount() const noexcept
{
    return m_threadsCount;
}

void Processor::applyMedianFilter(QUrl imageUrl)
{

    if (!m_mutex.tryLock()) {
        qDebug() << "processor is busy";
        return;
    }

    auto filepath = imageUrl.toLocalFile();
    m_resultImage = loadImage(filepath);
    if(!m_resultImage.isNull()) {
        emit processingStarted();
        auto future = QtConcurrent::run(&m_medianFilter, &MedianFilter::apply, std::ref(m_resultImage), threadsCount());
    } else {
        m_mutex.unlock();
    }
}

void Processor::setMedianFilterRadius(int radius)
{
    m_medianFilter.setFilterRadius(radius);
}

void Processor::copyImage(QUrl first, QUrl second)
{
    auto from = first.toLocalFile();
    auto to = second.toLocalFile();
    if (QFile::exists(to)) QFile::remove(to);
    if (!QFile::copy(from, to)) {
        qDebug() << "Failed to copy image" << from << "->" << to;
    } else {
        qDebug() << "Saved successfully to" << to;
    }
}

void Processor::cancelProcessing()
{
    m_medianFilter.cancelProcessing();
}

void Processor::setThreadsCount(int count)
{
    if (count < 1) {
        qDebug() << "bad threads count. Rejct changes";
        return;
    }
    m_threadsCount = count;
    qDebug() << "threads count:" << count;
    QThreadPool::globalInstance()->setMaxThreadCount(count);
    emit threadsCountChanged(count);
}

void Processor::onProcessingStarted()
{
    emit progressChanged(0);
}

void Processor::onProcessingStopped()
{
    QTemporaryFile file;
    file.open();
    file.close();
    auto newPath = file.fileName() + ".jpg";
    m_resultImage.save(newPath);
    emit processingFinished(newPath);
    m_mutex.unlock();
}

void Processor::onProgressChanged(int progress)
{
    emit progressChanged(progress);
}

void Processor::onMedianFilterRadiusChanged(int matrixSize)
{
    emit medianFilterRadiusChanged(matrixSize);
}

QImage Processor::loadImage(const QString &filename)
{
    QImage image(filename);
    return image;
}

