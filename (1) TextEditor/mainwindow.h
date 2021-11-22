#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QFontDialog>
#include <QSettings>
#include <QTextStream>
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    MainWindow(const QString& filename, QWidget *parent = 0);

    ~MainWindow();

    QSettings *settings;

protected:
    void closeEvent(QCloseEvent *e);

private slots:
    void on_actionNew_triggered();

    void documentModified();

    void on_actionSelectFont_triggered();

    void on_actionAbout_triggered();

    void on_actionAboutQT_triggered();

    bool saveFile();

    bool saveFileAs();

    void on_actionOpen_triggered();

private:
    Ui::MainWindow *ui;

    QString m_fileName;

    void loadFile(const QString &);

    void setFileName(const QString &);
};
#endif // MAINWINDOW_H
