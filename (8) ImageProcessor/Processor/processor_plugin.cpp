#include "processor_plugin.h"
#include "processor.h"

#include <qqml.h>

void ProcessorPlugin::registerTypes(const char *uri)
{
    // @uri ProcessingPlugin
    qmlRegisterType<Processor>(uri, 1, 0, "Processor");
}

