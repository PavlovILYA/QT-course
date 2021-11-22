#include "configurationdialog.h"
#include "ui_configurationdialog.h"
#include "QFileDialog"

ConfigurationDialog::ConfigurationDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ConfigurationDialog)
{
    ui->setupUi(this);
    setWindowTitle("Собери пазлы!");
}

ConfigurationDialog::~ConfigurationDialog()
{
    delete ui;
}


void ConfigurationDialog::on_browse_button_clicked()
{
    File = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "/", tr("Images (*.png *.jpg *.bmp)"));
    ui->lineEdit->setText(File);
    ui->frame->setPreview(QPixmap(File));
    update();
}

QString ConfigurationDialog::imageFilePath()
{
    return ui->lineEdit->text();
}

QSize ConfigurationDialog::puzzleSize()
{
    return ui->frame->size;
}

void ConfigurationDialog::on_buttonBox_accepted()
{
    if(File.isNull())
    {
        QMessageBox::warning(this, tr("Ошибочка"), tr("Нужно выбрать изображение, чтобы играть!"));
        ConfigurationDialog* w = new ConfigurationDialog;
        w->show();
    }
    else
    {
        QGraphicsView* view = new QGraphicsView;
        logic* scene = new logic;
        scene->setup(puzzleSize(), QPixmap(imageFilePath()));
        view->setScene(scene);
        view->show();
        view->setWindowTitle("Игровое поле");
    }
    QDialog::accept();
    //this->close();
}
void ConfigurationDialog::on_buttonBox_rejected()
{
    this->close();
}
