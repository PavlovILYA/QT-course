#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QPluginLoader>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDateTime>


#include "../SearchPlugin/IPlugin.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QMainWindow
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    //QStringList getPlugins(QString path);
    //void testPlugins(QStringList plugins);
    QFileInfoList findFiles(const QString &startDir, const QStringList &filters);


private slots:
    void on_browseButton_clicked();

    void on_startButton_clicked();

    void on_stopButton_clicked();

    void finish();

    void res(int i);

private:
    Ui::Widget *ui;

    QString dir_path;
    IPlugin * m_iplagin;
};
#endif // WIDGET_H
