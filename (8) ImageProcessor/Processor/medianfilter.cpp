#include "medianfilter.h"

#include <QPainter>
#include <QDebug>
#include <QtConcurrent>


MedianFilter::MedianFilter(QObject *parent)
    : QObject(parent)
    , m_totalPixelsCount(0)
    , m_pixelsReady(0)
    , m_filterRadius(3)
{
}

void MedianFilter::apply(QImage &image, int threadsCount)
{

    m_mutex.lock();
    m_isCanceled = false;
    int delta = m_filterRadius;

    emit started();

    QVector<StuffForFilter*> imageParts;
    cutImage(imageParts, delta, image, threadsCount);

    image = QtConcurrent::blockingMappedReduced(imageParts, &MedianFilter::applyFilterPartial, &MedianFilter::reduceToImage, QtConcurrent::OrderedReduce);

    emit stopped();
}

void MedianFilter::cutImage(QVector<StuffForFilter*>& imageParts, int delta, QImage &image, int threadsCount)
{
    QImage extendedImage = getExtendedImage(image, delta);
    m_totalImageSize = image.size();
    m_totalPixelsCount = image.width() * image.height();
    m_pixelsReady = 0;
    m_pixelsPerCent = (m_totalPixelsCount + 100 - 1) / 100;
    m_mutex.unlock();
    int partHeight = (extendedImage.height() + threadsCount - 1) / threadsCount;
    int partWidth = (extendedImage.width() + threadsCount - 1) / threadsCount;
    for (int row = 0; row < threadsCount; ++row) {
        for (int column = 0; column < threadsCount; ++column) {
            auto rect = QRect{column * partWidth, row * partHeight, qMin(partWidth + delta * 2, extendedImage.width() - column * partWidth), qMin(partHeight + delta * 2, extendedImage.height() - row * partHeight)};
            auto img = extendedImage.copy(rect);
            rect.setSize(rect.size() - 2 * QSize(delta, delta));
            rect.setTop(row * partHeight);
            rect.setLeft(column * partWidth);
            auto filterStuff = new StuffForFilter;
            filterStuff->imagePart = img;
            filterStuff->imagePartRect = rect;
            filterStuff->filter = this;
            imageParts.push_back(filterStuff);
        }
    }
}

void MedianFilter::setFilterRadius(int radius)
{
    if (radius < 1 || radius > 100) {
        qDebug() << "wrong radius";
        return;
    }
    m_filterRadius = radius;
    emit filterRadiusChanged(radius);
}

void MedianFilter::cancelProcessing()
{
    m_mutex.lock();
    m_isCanceled = true;
    m_mutex.unlock();
}

QImage MedianFilter::getExtendedImage(const QImage &image, int extendingValue)
{
    QImage extendedImage(image.width() + extendingValue * 2, image.height() + 2 * extendingValue, QImage::Format_RGB32);

    QPainter painter;
    painter.begin(&extendedImage);
    painter.drawImage(QPoint{extendingValue, extendingValue}, image);

    extendTopAndBottom(image, extendingValue, painter);
    extendRightAndLeftSides(extendedImage, image, extendingValue, painter);
    painter.end();

    return extendedImage;
}

void MedianFilter::extendTopAndBottom(const QImage &image, int extendingValue, QPainter &painter)
{
    for (int x = 0; x < image.width(); ++x) {
        QPen pen;

        pen.setColor(image.pixelColor(x, 0));
        painter.setPen(pen);
        painter.drawLine(x + extendingValue, 0, x + extendingValue, extendingValue);

        pen.setColor(image.pixelColor(x, image.height() - 1));
        painter.setPen(pen);
        painter.drawLine(x + extendingValue, image.height() + extendingValue, x + extendingValue, image.height() + extendingValue * 2 - 1);
    }
}

void MedianFilter::extendRightAndLeftSides(const QImage &extendedImage, const QImage &image, int extendingValue, QPainter &painter)
{
    for (int y = 0; y < extendedImage.height(); ++y) {
        QPen pen;

        pen.setColor(extendedImage.pixelColor(extendingValue, y));
        painter.setPen(pen);
        painter.drawLine(0, y, extendingValue, y);

        pen.setColor(extendedImage.pixelColor(image.width() + extendingValue - 1, y));
        painter.setPen(pen);
        painter.drawLine(image.width() + extendingValue, y, image.width() + extendingValue * 2 - 1, y);
    }
}

void MedianFilter::applyFilterToImagePart(QImage &image, StuffForFilter* filterStuff)
{
    int width = filterStuff->imagePartRect.width();
    int height = filterStuff->imagePartRect.height();

    int delta = m_filterRadius;

    int size = (delta * 2 + 1) * (delta * 2 + 1);
    std::vector<int> r(size);
    std::vector<int> g(size);
    std::vector<int> b(size);
    int counter = 0;
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            filterStuff->filter->m_mutex.lock();
            if (filterStuff->filter->m_isCanceled) {
                filterStuff->filter->m_mutex.unlock();
                return;
            }
            filterStuff->filter->m_mutex.unlock();
            for (int i = - delta; i <= delta; ++i) {
                for (int j = - delta; j <= delta; ++j) {
                    auto rgb = filterStuff->imagePart.pixelColor(x + j + delta, y + i + delta).rgb();
                    int pos = (i + delta) * (2 * delta + 1) + j + delta;
                    r[pos] = qRed(rgb);
                    g[pos] = qGreen(rgb);
                    b[pos] = qBlue(rgb);
                }
            }
            std::sort(r.begin(), r.end());
            std::sort(g.begin(), g.end());
            std::sort(b.begin(), b.end());
            image.setPixel(x, y, qRgb(r[size / 2], g[size / 2], b[size / 2]));
            counter++;
            if (counter == filterStuff->filter->m_pixelsPerCent) {
                filterStuff->filter->addReadyPixels(counter);
                counter = 0;
            }
        }
    }
    addReadyPixels(counter);
}

void MedianFilter::addReadyPixels(int newReadyPixelsCount)
{
    m_pixelsReady += newReadyPixelsCount;
    emit progress(100 * m_pixelsReady / m_totalPixelsCount);
}

MedianFilter::StuffForFilter* MedianFilter::applyFilterPartial(StuffForFilter* filterStuff)
{
//    qDebug() << "apply filter partial";
    QImage image(filterStuff->imagePartRect.size(), QImage::Format_RGB32);

    filterStuff->filter->applyFilterToImagePart(image, filterStuff);

    filterStuff->imagePart = image;
    return filterStuff;
}

void MedianFilter::reduceToImage(QImage& image, StuffForFilter* filterStuff)
{
    if (image.isNull()) {
        image = QImage(filterStuff->filter->m_totalImageSize, QImage::Format_RGB32);
    }
    QPainter painter;
    //auto painter = QPainter(&image);
    painter.begin(&image);
    painter.drawImage(filterStuff->imagePartRect.topLeft(), filterStuff->imagePart);
    painter.end();
    delete filterStuff;
}
