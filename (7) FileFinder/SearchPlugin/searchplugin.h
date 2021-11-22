#ifndef SEARCHPLUGIN_H
#define SEARCHPLUGIN_H

#include <QObject>
#include <QtDebug>
#include <QtPlugin>
#include <QString>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent>

#include "SearchPlugin_global.h"
#include "IPlugin.h"

class SEARCHPLUGIN_EXPORT SearchPlugin : public QObject, public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID  "Ilya Pavlov is a cool dude")
    Q_INTERFACES(IPlugin)
public:
    explicit  SearchPlugin(QObject* parent = nullptr);
    ~SearchPlugin();

signals:

public slots:

    // IPlugin interface
public:
    void test(QString test) override;
    void catchParametrs(QString dir, QStringList filters, int days, QString oper, int kb) override;
    void start(QFileInfoList InfoFiles) override;
    bool myFilter(const QFileInfo &info) override;
    void stop() override;



private:
    QString m_dir;
    QStringList m_filters;
    int m_days;
    QString m_oper;
    int m_kb;




};

#endif // SEARCHPLUGIN_H
