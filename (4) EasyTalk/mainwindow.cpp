#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

Chat::Chat(QString nick, QWidget *parent): QWidget(parent){
  Socket = nullptr;
  Nick = nick;
  QHBoxLayout *layout = new QHBoxLayout(this);
  QVBoxLayout *messageLay = new QVBoxLayout;
  Online = new QListWidget(this);
  Online->setMaximumWidth(180);
  Messages = new QTextEdit;
  Post = new QLineEdit;
  Messages -> setReadOnly(true);
  messageLay->addWidget(Messages);
  messageLay->addWidget(Post);
  layout->addWidget(Online);
  layout->addLayout(messageLay);
  connect(Post, SIGNAL(returnPressed()), this, SLOT(send()));
}

void Chat::screen(const QString &nick, const QString &message){
  QTime time = QTime::currentTime();
  QString msg;
  if (nick == Nick)
    msg = QString("<font color=\"%1\">[%2] <b>%3</b></font>: %4").arg("#CC0033").arg(time.toString()).arg(nick).arg(message);
  else
    msg = QString("<font color=\"%1\">[%2] <b>%3</b></font>: %4").arg("#6699FF").arg(time.toString()).arg(nick).arg(message);
  Messages->append(msg);
  Post->clear();
}

QByteArray Chat::readMessage(const QString &nick, const QString &message) const{
  QByteArray tmp;
  QDataStream stream(&tmp, QIODevice::WriteOnly);
  stream<<nick<<message;
  return tmp;
}

QPair <QString, QString> Chat::comPAIRmessage(const QByteArray &mesg){
  QString nick, msg;
  QDataStream stream(mesg);
  stream >> nick >> msg;
  return qMakePair(nick, msg);
}

void Chat::send(){
  QByteArray tmp = readMessage(Nick, Post->text());
  Socket->writeDatagram(tmp, QHostAddress::Broadcast, PortSocket);
}

void Chat::power(int port){
  Socket = new QUdpSocket;
  PortSocket = port;
  Socket-> bind(PortSocket,QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);
  qDebug() << "Your port " << PortSocket;
  connect(Socket, SIGNAL(readyRead()), this, SLOT(Datagrams()));
  QByteArray ptr = readMessage(Nick, "#Welcome#");
  Socket->writeDatagram(ptr, QHostAddress::Broadcast, PortSocket);
}

void Chat::Datagrams(){
  QByteArray tmp;
  //QPair <QString, QString> pair = comPAIRmessage(tmp);
  while (Socket->hasPendingDatagrams()){
      tmp.resize(Socket->pendingDatagramSize());
      Socket->readDatagram(tmp.data(), tmp.size());
      QPair <QString, QString> pair = comPAIRmessage(tmp);
      if (pair.second == "#Welcome#"){
          if (pair.first != Nick){
              QList <QListWidgetItem*> au = Online->findItems(pair.first, Qt::MatchExactly);
              if (au.empty()){
                  Online->addItem(pair.first);
                  QByteArray tmp = readMessage(Nick, "#Welcome#");
                  Socket->writeDatagram(tmp, QHostAddress::Broadcast, PortSocket);
                }
            }
        } else if (pair.second == "#GoodBye#"){
          for (int i = 0; i < Online->count(); ++i){
              if (Online->item(i)->text() == pair.first){
                  Online->takeItem(i);
                  break;
                }
            }
        }
      else screen(pair.first, pair.second);
    }
}

Chat::~Chat(){
  QByteArray tmp = readMessage(Nick, "#GoodBye#");
  Socket->writeDatagram(tmp, QHostAddress::Broadcast, PortSocket);
}

