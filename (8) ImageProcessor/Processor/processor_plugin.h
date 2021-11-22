#ifndef PROCESSOR_PLUGIN_H
#define PROCESSOR_PLUGIN_H

#include <QQmlExtensionPlugin>

class ProcessorPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;
};

#endif // PROCESSOR_PLUGIN_H
