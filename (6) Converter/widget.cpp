#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->textEditRes1->setStyleSheet("color: black;"
                            "background-color: rgb(255, 204, 204);"
                            "selection-color: rgb(255, 204, 204);"
                            "selection-background-color: black;");
    ui->lineEditFile1->setStyleSheet("color: black;"
                                     "background-color: rgb(255, 204, 204);"
                                     "selection-color: rgb(255, 204, 204);"
                                     "selection-background-color: black;");
    ui->textEditRes2->setStyleSheet("color: black;"
                            "background-color: rgb(255, 255, 204);"
                            "selection-color: rgb(255, 255, 204);"
                            "selection-background-color: black;");
    ui->lineEditFile2->setStyleSheet("color: black;"
                                     "background-color: rgb(255, 255, 204);"
                                     "selection-color: rgb(255, 255, 204);"
                                     "selection-background-color: black;");
    ui->textEditRes3->setStyleSheet("color: black;"
                            "background-color: rgb(180, 204, 255);"
                            "selection-color: rgb(180, 204, 255);"
                            "selection-background-color: black;");
    ui->lineEditFile3->setStyleSheet("color: black;"
                                     "background-color: rgb(180, 204, 255);"
                                     "selection-color: rgb(180, 204, 255);"
                                     "selection-background-color: black;");
    ui->textBrowser->setStyleSheet("color: black;"
                                   "background-color: rgb(204, 204, 204);"
                                   "selection-color: rgb(204, 204, 204);"
                                   "selection-background-color: black;");
    ui->spinBox->setValue(5);
    ui->spinBox->setMaximum(300);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::printBuffer()
{
    // добавить мьютекс!!!
    m_mutex->lock();
    ui->textBrowser->clear();
    for (int i=0; i<BufSize; i++)
    {
        QString cur;
        if (buffer->getStateOne(i) == 1)
        {
            cur = "<font color=#808080>пусто</font>";
            ui->textBrowser->append(cur);
        }
        else if (buffer->getTypeOne(i) == 1)
        {
            cur = "<font color=#ff0000>";
            cur.append(buffer->getChar(i));
            cur.append("</font>");
            ui->textBrowser->append(cur);
        }
        else if (buffer->getTypeOne(i) == 2)
        {
            cur = "<font color=#ffff00>";
            cur.append(buffer->getChar(i));
            cur.append("</font>");
            ui->textBrowser->append(cur);
        }
        else if (buffer->getTypeOne(i) == 3)
        {
            cur = "<font color=#0000ff>";
            cur.append(buffer->getChar(i));
            cur.append("</font>");
            ui->textBrowser->append(cur);
        }
    }
    qDebug() << "I JUST PRINT CHANGED BUFFER    my thread:" << QThread::currentThread();
    m_mutex->unlock();
}

void Widget::printChar(const CustomData &cd)
{
    m_mutex->lock();
    QString qstr = "";
    if (cd.m_integer == 1)
    {
        qstr = ui->textEditRes1->toPlainText();
        qstr.append(cd.m_char);
        ui->textEditRes1->clear();
        ui->textEditRes1->setText(qstr);
    }
    else if (cd.m_integer == 2)
    {
        qstr = ui->textEditRes2->toPlainText();
        qstr.append(cd.m_char);
        ui->textEditRes2->clear();
        ui->textEditRes2->setText(qstr);
    }
    else if (cd.m_integer == 3)
    {
        qstr = ui->textEditRes3->toPlainText();
        qstr.append(cd.m_char);
        ui->textEditRes3->clear();
        ui->textEditRes3->setText(qstr);
    }
    qDebug() << "I INSERT CHAR    my thread:" << QThread::currentThread();
    m_mutex->unlock();
}


void Widget::on_pushButtonFile1_clicked()
{
    FileName1 = QFileDialog::getOpenFileName(this, tr("Открыть текстовый файл"), "/", tr("Text files (*.txt)"));
    ui->lineEditFile1->setText(FileName1);
    QFile file1(FileName1);
    file1.open(QIODevice::ReadOnly);
    QTextStream stream(&file1);
    QString all = stream.readAll();
    FileSize1 = all.length();
    file1.close();
}

void Widget::on_pushButtonFile2_clicked()
{
    FileName2 = QFileDialog::getOpenFileName(this, tr("Открыть текстовый файл"), "/", tr("Text files (*.txt)"));
    ui->lineEditFile2->setText(FileName2);
    QFile file2(FileName2);
    file2.open(QIODevice::ReadOnly);
    QTextStream stream(&file2);
    QString all = stream.readAll();
    FileSize2 = all.length();
    file2.close();
}

void Widget::on_pushButtonFile3_clicked()
{
    FileName3 = QFileDialog::getOpenFileName(this, tr("Открыть текстовый файл"), "/", tr("Text files (*.txt)"));
    ui->lineEditFile3->setText(FileName3);
    QFile file3(FileName3);
    file3.open(QIODevice::ReadOnly);
    QTextStream stream(&file3);
    QString all = stream.readAll();
    FileSize3 = all.length();
    file3.close();
}

void Widget::on_pushButtonStart_clicked()
{
    if (FileSize1 == -1 || FileSize2 == -1 || FileSize3 == -1)
        QMessageBox::warning(this, "Предупреждение", "Нужно выбрать все 3 файла!");
    else
    {
        ui->textEditRes1->clear();
        ui->textEditRes2->clear();
        ui->textEditRes3->clear();
        ui->textBrowser->clear();
        BufSize = ui->spinBox->value();
        DataSize = FileSize1 + FileSize2 + FileSize3;
        buffer = new Buffer(BufSize);

        freeBytes = new QSemaphore(BufSize);
        rawBytes = new QSemaphore(0);
        readyBytes = new QSemaphore(0);
        m_mutex = new QMutex();

        qDebug() << "create semaphores";

        qDebug() << "GUI thread:" << QThread::currentThread();

        producer = new Producer(FileName1, FileName2, FileName3, FileSize1, FileSize2, FileSize3, buffer, freeBytes, rawBytes, m_mutex);
        worker = new Worker(FileSize1, FileSize2, FileSize3, buffer, rawBytes, readyBytes, m_mutex);
        consumer = new Consumer(FileSize1, FileSize2, FileSize3, buffer, readyBytes, freeBytes, m_mutex);

        qDebug() << "connect1:" << connect(producer,&Producer::bufferChanged, this ,&Widget::printBuffer);
        qDebug() << "connect2:" << connect(worker,&Worker::bufferChanged, this ,&Widget::printBuffer);
        qDebug() << "connect3:" << connect(consumer, &Consumer::bufferChanged, this, &Widget::printBuffer);
        qDebug() << "connect4:" << connect(consumer,&Consumer::readyByte, this ,&Widget::printChar);
        qDebug() << "connect4:" << connect(consumer,&Consumer::finished, this ,&Widget::finished);

        producer->start();
        worker->start();
        consumer->start();

        //producer->wait();
        //worker->wait();
        //consumer->wait();
    }
}

void Widget::finished()
{
    delete buffer;
    delete freeBytes;
    delete rawBytes;
    delete readyBytes;
    delete m_mutex;

}
