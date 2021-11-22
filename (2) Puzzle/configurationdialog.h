#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include "puzzlesizewidget.h"
#include "puzzlepiece.h"
#include "logic.h"
#include <QDialog>
#include <QMessageBox>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
namespace Ui {
class ConfigurationDialog;
}
QT_END_NAMESPACE

class ConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigurationDialog(QWidget *parent = nullptr);
    ~ConfigurationDialog();
    QString imageFilePath();
    QSize puzzleSize();

private slots:

    void on_browse_button_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ConfigurationDialog *ui;
    QString File;
};

#endif // CONFIGURATIONDIALOG_H
