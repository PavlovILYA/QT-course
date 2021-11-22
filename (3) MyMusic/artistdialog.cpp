#include "artistdialog.h"
#include "ui_artistdialog.h"

ArtistDialog::ArtistDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArtistDialog)
{
    ui->setupUi(this);
    m_mapper.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    this->setWindowTitle(tr("Исполнитель"));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ок"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Отмена"));
}

ArtistDialog::~ArtistDialog()
{
    delete ui;
}

void ArtistDialog::setModel(MusicModel *model) {
    m_mapper.setModel(model);
}

void ArtistDialog::setModelIndex(const QModelIndex &index) {
    QModelIndex parent = index.parent();
    m_mapper.setRootIndex(parent);
    m_mapper.setCurrentModelIndex(index);
    m_mapper.addMapping(ui->lineEdit_artist,0);
    m_mapper.addMapping(ui->lineEdit_country,1);
    m_mapper.addMapping(ui->plainTextEdit_describe,2);
    m_mapper.addMapping(ui->image,3, "pixmap");
    m_mapper.setCurrentIndex(index.row());
}

void ArtistDialog::accept() {
    m_mapper.submit();
    QDialog::accept();
}

bool ArtistDialog::addArtist(MusicModel *model, const QModelIndex &parent) {
    setModel(model);
    int row = model->rowCount(parent);
    if(!model->insertRow(row, parent)) {
        return false;
    }
    QModelIndex index = model->index(row, 0, parent);
    setModelIndex(index);
    if (!exec()) {
        model->removeRow(row, parent);
        return false;
    }
    return true;
}

void ArtistDialog::on_pushButton_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Открытие изображения"), tr("/home"), tr("Image Files (*.png *.jpg *.bmp)"));
    if (fileName.isEmpty()) {
        return;
    }
    else {
        if (!image.load(fileName)) {
            QMessageBox::warning(this, tr("Открытие изображения"), tr("Файл не может быть открыт."),  QMessageBox::Close);
            return;
        }
        ui->image->setPixmap(image.scaled(180,180));
    }
}
