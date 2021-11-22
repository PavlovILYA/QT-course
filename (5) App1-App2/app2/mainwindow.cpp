#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stopButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::myFinished()
{
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    int sec;
    sec = timer.elapsed();
    ui->textEdit->append("PROCESS HAS BEEN KILLED");
    int Exit = myProcess->exitCode();
    ui->textEdit->append("EXIT CODE: " + QString::number(Exit));
    sec /= 1000;
    ui->textEdit->append("RUNTIME: " + QString::number(sec) + " seconds");
}

void MainWindow::readSTDOUT()
{
    //QProcess* myProcess = (QProcess*)sender();
    //myProcess->setReadChannel(QProcess::StandardOutput);
    QByteArray BA = myProcess->readAllStandardOutput();
    ui->textEdit->append("\n-----StandardOutput-----");
    ui->textEdit->append(QString::fromStdString(BA.toStdString()));
    ui->textEdit->append("-----StandardOutput-----\n");
}

void MainWindow::readSTDERR()
{
    //QProcess* myProcess = (QProcess*)sender();
    //myProcess->setReadChannel(QProcess::StandardError);
    QByteArray BA = myProcess->readAllStandardError();
    ui->textEdit->append("\n-----StandardError-----");
    ui->textEdit->append(QString::fromStdString(BA.toStdString()));
    ui->textEdit->append("-----StandardError-----\n");
}

void MainWindow::on_browseButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "/");
    //QString command = ".";
    //command.append(filePath);
    ui->lineEdit_browse->setText(filePath);
}

void MainWindow::on_startButton_clicked()
{
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    QString str_arg;
    if (ui->lineEdit_arguments->text() != "")
        str_arg.append(ui->lineEdit_arguments->text());
    QStringList arguments = str_arg.split(' ', QString::SkipEmptyParts);
    QObject *parent;
    myProcess = new QProcess(parent);

    //QString command = ".";
    //command.append(ui->lineEdit_browse->text());
    QString command = ui->lineEdit_browse->text();
    //myProcess->setProcessChannelMode(QProcess::MergedChannels);
    myProcess->start(command, arguments);
    timer.start();
    ui->textEdit->append("PROCESS HAS BEEN CREATED");

    connect(myProcess, &QProcess::readyReadStandardOutput, this, &MainWindow::readSTDOUT);
    connect(myProcess, &QProcess::readyReadStandardError, this, &MainWindow::readSTDERR);
    connect(myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::myFinished);
}

void MainWindow::on_stopButton_clicked()
{
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    myProcess->kill();
    //ui->textEdit->append("PROCESS HAS BEEN KILLED");
    //int Exit = myProcess->exitCode();
    //ui->textEdit->append("EXIT CODE: " + QString::number(Exit));
}
