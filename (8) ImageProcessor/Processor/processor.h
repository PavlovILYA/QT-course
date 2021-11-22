#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QObject>
#include <QVariant>
#include <QUrl>
#include <QMutex>
#include <QImage>
#include <QtConcurrent>

#include "medianfilter.h"


class Processor : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Processor)
    Q_PROPERTY(int threadsCount READ threadsCount WRITE setThreadsCount NOTIFY threadsCountChanged)
    Q_PROPERTY(int medianFilterRadius READ medianFilterRadius NOTIFY medianFilterRadiusChanged)
public:
    explicit Processor(QObject* parent = nullptr);
    int threadsCount() const noexcept;
    ~Processor() override = default;
    Q_INVOKABLE void applyMedianFilter(QUrl imagePath);
    Q_INVOKABLE void setMedianFilterRadius(int radius);
    Q_INVOKABLE void copyImage(QUrl first, QUrl second);
    Q_INVOKABLE void cancelProcessing();
    inline int medianFilterRadius() const noexcept { return m_medianFilter.filterRadius(); }
    Q_INVOKABLE void setThreadsCount(int count);
signals:
    void processingStarted();
    void processingFinished(QString imagePath);
    void progressChanged(int progress);
    void threadsCountChanged(int threadsCount);
    void matrixSizeChanged();
    void medianFilterRadiusChanged(int radius);
private slots:
    void onProcessingStarted();
    void onProcessingStopped();
    void onProgressChanged(int progress);
    void onMedianFilterRadiusChanged(int radius);
private:
    QMutex m_mutex;
    MedianFilter m_medianFilter;
    QImage m_resultImage;
    int m_threadsCount;
    QImage loadImage(const QString& filename);
};

#endif // PROCESSOR_H
