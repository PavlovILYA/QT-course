#include "mainwindow.h"
#include <QtCore>
#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    a.setApplicationName("Text Editor");
    a.setApplicationVersion("0.1");
    a.setOrganizationName("MephiSoft");
    a.setOrganizationDomain("mephi.ru");
    a.setWindowIcon(QIcon("/Users/pavlov/pavlov/TextEditor/images/l1_images_new.png"));
}
