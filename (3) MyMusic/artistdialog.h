#ifndef ARTISTDIALOG_H
#define ARTISTDIALOG_H
#include <QPixmap>
#include <QMessageBox>
#include <QFileDialog>
#include <QDataWidgetMapper>
#include <musicmodel.h>
#include <QDialog>

namespace Ui {
class ArtistDialog;
}

class ArtistDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ArtistDialog(QWidget *parent = nullptr);
    ~ArtistDialog();
    void setModel(MusicModel*model);
    void setModelIndex(const QModelIndex &model);
    void accept();
    bool addArtist(MusicModel *model, const QModelIndex &parent);

private slots:
    void on_pushButton_clicked();

private:
    Ui::ArtistDialog *ui;
    QDataWidgetMapper m_mapper;
    QPixmap image;
};

#endif // ARTISTDIALOG_H
