#include "searchplugin.h"

SearchPlugin::SearchPlugin(QObject *parent)
{
    Q_UNUSED(parent)
}

SearchPlugin::~SearchPlugin()
{

}

void SearchPlugin::test(QString test)
{
    qDebug()  << "Test from plugin: " << test;
}

void SearchPlugin::catchParametrs(QString dir, QStringList filters, int days, QString oper, int kb)
{
    m_dir = dir;
    m_filters = filters;
    m_days = days;
    m_oper = oper;
    m_kb = kb;
}

bool SearchPlugin::myFilter(const QFileInfo &info)
{
    qDebug() << "my thread:" << QThread::currentThread();
    int count = 0;
    qDebug() << "kb: " << m_kb;
    if (m_kb != -1)
    {
        if (m_oper == ">")
        {
            if (info.size() > m_kb * 1000) count++;
        }
        if (m_oper == "<")
        {
            if (info.size() < m_kb * 1000) count++;
        }
    }
    else
        count++;
    qDebug() << "days from spin: " << m_days;
    if (m_days != -1)
    {
        qDebug() << "days real: " << info.birthTime().daysTo(QDateTime::currentDateTime());
        qDebug() << "days from spin: " << m_days;
        if (info.birthTime().daysTo(QDateTime::currentDateTime()) <= m_days)
            count++;
    }
    else
        count++;
    qDebug() << "count(2): " << count;
    return count == 2;

}

void SearchPlugin::stop()
{
    watcher.cancel();
    watcher.waitForFinished();
}

void SearchPlugin::start(QFileInfoList InfoFiles)
{
    m_future = QtConcurrent::filtered(InfoFiles, [=](const QFileInfo &f) {
        return myFilter(f);
    });
    watcher.setFuture(m_future);
}
