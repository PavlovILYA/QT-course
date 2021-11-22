#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtWidgets>
#include <QUdpSocket>
#include <QListWidget>

#include <QTime>
#include <QTextEdit>
#include <QLabel>
#include <QListWidget>
#include <QLayout>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Chat: public QWidget
{
    Q_OBJECT
public:
    Chat();
    Chat(QString nick, QWidget *parent = nullptr);
    ~Chat();
    void power (int port = 1024);

public slots:
  void send();
  void screen(const QString &nick, const QString &message);
  void Datagrams();

private:
  QString Nick;
  QByteArray readMessage(const QString &nick, const QString &message)const;
  QPair<QString,QString> comPAIRmessage(const QByteArray &mesg);
  QListWidget *Online;
  QTextEdit *Messages;
  QLineEdit *Post;
  QUdpSocket *Socket;
  quint16 PortSocket;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
