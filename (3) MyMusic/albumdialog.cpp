#include "albumdialog.h"
#include "ui_albumdialog.h"

AlbumDialog::AlbumDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlbumDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Альбом"));
    ui->lineEdit_artist->setReadOnly(true);
    m_mapper.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ок"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Отмена"));
}

AlbumDialog::~AlbumDialog()
{
    delete ui;
}

void AlbumDialog::setModel(MusicModel *model) {
    m_mapper.setModel(model);
}

void AlbumDialog::setModelIndex(const QModelIndex &index) {
    QModelIndex parent = index.parent();
    Item *parent_item = static_cast<Item*>(index.parent().internalPointer());
    ui->lineEdit_artist->setText(parent_item->name());

    m_mapper.setRootIndex(parent);
    m_mapper.setCurrentModelIndex(index);

    m_mapper.addMapping(ui->lineEdit_name,0);
    m_mapper.addMapping(ui->lineEdit_ganre,1);
    m_mapper.addMapping(ui->spinBox,2);
    m_mapper.addMapping(ui->plainTextEdit_describe,3);
    m_mapper.addMapping(ui->image,4, "pixmap");
    m_mapper.setCurrentIndex(index.row());
}

void AlbumDialog::accept() {
    m_mapper.submit();
    QDialog::accept();
}

bool AlbumDialog::addAlbum(MusicModel *model, const QModelIndex &parent) {

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

void AlbumDialog::on_pushButton_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Открытие изображения"), tr("/home"), tr("Image Files (*.png *.jpg *.bmp)"));
    if (fileName.isEmpty()) {
        return;
    }
    else {
        if (!image.load(fileName)) {
            QMessageBox::warning(this, tr("Открытие изображения"), tr("Файл не может быть открыт."), QMessageBox::Close);
            return;
        }
        ui->image->setPixmap(image.scaled(170,170));
    }
}
