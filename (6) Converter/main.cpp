#include "widget.h"
#include "customdata.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<CustomData>();

    Widget w;
    w.setStyleSheet("QPushButton#pushButtonStart {background-color: rgb(153, 204, 102);border-radius: 5px;padding: 10.5px;}QPushButton#pushButtonStart:pressed {background-color: rgb(204, 204, 204);}");
    w.show();
    return a.exec();
}
