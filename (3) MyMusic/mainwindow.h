#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "musicmodel.h"
#include <QCloseEvent>
#include <QTranslator>
#include <QLibraryInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);

public slots:
    void setModified();
    void update_active_actions(QModelIndex index);

private slots:
    void on_action_art_triggered();

    void on_action_del_triggered();

    void on_action_edit_triggered();

    void on_action_alb_triggered();

    void on_action_song_triggered();

    bool on_action_save_triggered();

    void on_action_open_triggered();

    void on_action_triggered();

private:
    Ui::MainWindow *ui;
    MusicModel *model;
    QTranslator myappTranslator;
    QTranslator s_myappTranslator;
};
#endif // MAINWINDOW_H
