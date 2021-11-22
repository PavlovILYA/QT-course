#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include <QObject>
#include <QImage>
#include <memory>
#include <QMutex>


class MedianFilter : public QObject
{
    Q_OBJECT
friend class Processor;
public:
    explicit MedianFilter(QObject *parent = nullptr);
    virtual void apply(QImage& image, int threadsCount);
    void setFilterRadius(int radius);
    inline int filterRadius() const noexcept { return m_filterRadius; }
    void cancelProcessing();
signals:
    void started();
    void stopped();
    void progress(int percent);
    void filterRadiusChanged(int newRadius);
protected:
    struct StuffForFilter {
        QImage imagePart;
        QRect imagePartRect;
        MedianFilter* filter;
    };
    QImage getExtendedImage(const QImage& image, int extendingValue);
    void extendTopAndBottom(const QImage& image, int extendingValue, QPainter& painter);
    void extendRightAndLeftSides(const QImage& extendedImage, const QImage& image, int extendingValue, QPainter& painter);
    void cutImage(QVector<StuffForFilter*>& imageParts, int delta, QImage& image, int threadsCount);
    void applyFilterToImagePart(QImage& image, StuffForFilter* filterStuff);
    static StuffForFilter* applyFilterPartial(StuffForFilter* p);
    static void reduceToImage(QImage& image, StuffForFilter* p);
private:
    void addReadyPixels(int newReadyPixelsCount);
private:
    int m_totalPixelsCount;
    int m_pixelsReady;
    int m_pixelsPerCent;
    QSize m_totalImageSize;
    int m_filterRadius;
    bool m_isCanceled;
    QMutex m_mutex;
};

#endif // MEDIANFILTER_H
