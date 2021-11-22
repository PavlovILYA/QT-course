#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QDebug>
#include <QString>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QFuture>
#include <QFutureWatcher>

class IPlugin{
public:
    virtual ~IPlugin() {};
    virtual void test(QString test) = 0;
    virtual void catchParametrs(QString dir, QStringList filters, int days, QString oper, int kb) = 0;
    virtual void start(QFileInfoList InfoFiles) = 0;
    virtual bool myFilter(const QFileInfo &info) = 0;
    virtual void stop() = 0;
    QFuture<QFileInfo> m_future;
    QFutureWatcher<QFileInfo> watcher;
};

// Decleare interface
Q_DECLARE_INTERFACE(IPlugin, "Ilya Pavlov is a cool dude")

#endif // IPLUGIN_H
