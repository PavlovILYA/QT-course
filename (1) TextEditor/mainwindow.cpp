#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QFontDialog>
#include <QSettings>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QFileInfo>
#include <QString>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    //ui->actionPaste->setEnabled(true);

    connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(documentModified()));

    //connect(ui->actionAboutQT, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    connect(ui->actionCut, SIGNAL(triggered()), ui->textEdit, SLOT(cut()));
    connect(ui->actionPaste, SIGNAL(triggered()), ui->textEdit, SLOT(paste()));
    connect(ui->actionUndo, SIGNAL(triggered()), ui->textEdit, SLOT(undo()));
    connect(ui->actionCopy, SIGNAL(triggered()), ui->textEdit, SLOT(copy()));
    connect(ui->actionRedo, SIGNAL(triggered()), ui->textEdit, SLOT(redo()));

    //connect(ui->textEdit, SIGNAL(copyAvailable(bool)), ui->actionPaste, SLOT(setEnabled(bool)));
    connect(ui->textEdit, SIGNAL(copyAvailable(bool)), ui->actionCopy, SLOT(setEnabled(bool)));
    connect(ui->textEdit, SIGNAL(copyAvailable(bool)), ui->actionCut, SLOT(setEnabled(bool)));
    connect(ui->textEdit, SIGNAL(undoAvailable(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
    connect(ui->textEdit, SIGNAL(redoAvailable(bool)), ui->actionRedo, SLOT(setEnabled(bool)));

    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    settings = new QSettings("MephiSoft", "Text Editor");
    QString viewFont = "viewFont";
    QFont default_font("Cooper Black", 13);
    QFont curFont = qvariant_cast<QFont>(settings -> value(viewFont, default_font));
    ui -> textEdit -> setFont(curFont);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    MainWindow *NewWindow = new MainWindow;
    NewWindow->show();
}

void MainWindow::documentModified()
{
    setWindowModified(true);
    //bool windowModified = true;
}


void MainWindow::on_actionSelectFont_triggered()
{
    bool ok;
    QString viewFont = "viewFont";
    QFont CurFont = ui->textEdit->font();
    //QFont CurFont = qvariant_cast<QFont>(settings -> value(viewFont));
    QFont NewFont = QFontDialog::getFont(&ok, CurFont, this);
    if (ok) {
        ui->textEdit->setFont(NewFont);
        settings -> setValue(viewFont, NewFont);
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "Text Editor", "Это приложение написал студент группы Б17-511 Павлов Илья. Он молодец! Подписывайтесь на его инстаграм @pavlov.jpg");
}

void MainWindow::on_actionAboutQT_triggered()
{
    QMessageBox::aboutQt(this, "Text Editor");
}

void MainWindow::closeEvent(QCloseEvent *e){
    if(isWindowModified() == true){
        switch(QMessageBox::warning(this, "Документ изменен",
           "Документ был изменен. "
           "Хотите сохранить изменения?\n"
           "Вы можете потерять несохраненные изменения.",
           QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
           QMessageBox::No)){
        case QMessageBox::Yes:
            saveFile();
            e->ignore();
            break;
        case QMessageBox::No:
            e->accept();
            break;
        case QMessageBox::Cancel:
            e->ignore();
            break;
        }
    }
    else{
        e->accept();
    }
}

MainWindow::MainWindow(const QString& fileName, QWidget *parent)
{

}

void MainWindow::loadFile(const QString &fileName)
{
    if (fileName.isEmpty()) {
        setFileName(QString());
    }
    if (!QFile::exists(fileName)){
            QMessageBox::warning(this, tr("title"), tr("TEXT"));
    }
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, tr("title"), tr("TEXT"));
        setFileName(QString());
    }
    else{
        QTextStream in(&file);
        ui->textEdit->setText(in.readAll());
        file.close();
        setFileName(fileName);
        this -> setWindowModified(false);
    }
}

void MainWindow::setFileName(const QString &fileName){
    m_fileName = fileName;
    QString title = QString("%1[*] - %2")
            .arg(m_fileName.isNull()?"новый":QFileInfo(m_fileName).fileName())
            .arg(QApplication::applicationName());
    this ->setWindowTitle(title);
}


void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            "Открыть документ", QDir::currentPath(), "Text documents (*.txt)");
    if (fileName.isNull()){
            return;
    }
    else{
        if (m_fileName.isNull() && !this->isWindowModified()){
            loadFile(fileName);
        }
        else{
            MainWindow newWindow(fileName);
            newWindow.show();
        }
    }
}

bool MainWindow::saveFileAs()
{
    QString newFileName = QFileDialog::getSaveFileName(this, "Сохранить документ",
                                                       m_fileName.isNull()?QDir::currentPath():m_fileName, "Text documents (*.txt)");
    if (newFileName.isNull()){
        return false;
    }
    else{
        setFileName(newFileName);
        return saveFile();
    }
}

bool MainWindow::saveFile()
{
    if (m_fileName.isNull()){
        return saveFileAs();
    }
    QFile file(m_fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        setFileName(QString());
        return false;
    }
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream in(&file);
    in << ui->textEdit->toPlainText();
    file.close();
    this ->setWindowModified(false);
    return true;
}
