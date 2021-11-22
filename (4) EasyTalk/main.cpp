#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCommandLineParser parser;
    QCommandLineOption port_(QStringList() << "p" << "port", ".");
    QCommandLineOption nick_(QStringList() << "n" << "nick", ".");
    parser.addOption(port_);
    parser.addOption(nick_);
    parser.process(a);
    QStringList arg = parser.positionalArguments();
    if (!parser.isSet(port_))
        arg.insert(1, "2048");
    if (!parser.isSet(nick_))
        arg.insert(0, "NewUser");
    if (arg.at(0).toInt())
        arg.swapItemsAt(0, 1);
    Chat Chat(arg.at(0));
    if (arg.at(1).toInt() < 2048){
        qDebug() << arg.at(1).toInt();
        arg[1] = QString::number(arg.at(1).toInt() + 3000);
        qDebug() << "Your port was increase with 3000 points";
    }
    Chat.setWindowTitle(QString("EasyTalk. [%1@%2]").arg(arg.at(0)).arg(arg.at(1)));
    Chat.power(arg.at(1).toInt());
    Chat.show();

    MainWindow w;
    //w.show();
    return a.exec();
}
