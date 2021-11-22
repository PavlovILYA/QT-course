#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "albumdialog.h"
#include "artistdialog.h"
#include "ratingdelegate.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->treeView);

    model = new MusicModel();
    ui->treeView->setModel(model);

    RatingDelegate *delegate = new RatingDelegate();
    ui->treeView->setItemDelegateForColumn(2, delegate);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->action_edit->setEnabled(false);
    ui->action_del->setEnabled(false);
    ui->action_alb->setEnabled(false);
    ui->action_song->setEnabled(false);

    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(setModified()));
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(update_active_actions(QModelIndex)));

    setWindowTitle("Моя Музыка");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setModified() {
    setWindowModified(true);
}

void MainWindow::update_active_actions(QModelIndex index) {
    Item *cur_item=static_cast<Item*>(index.internalPointer());
    if(!cur_item or !index.isValid()) {
        ui->action_edit->setEnabled(false);
        ui->action_del->setEnabled(false);
        ui->action_alb->setEnabled(false);
        ui->action_song->setEnabled(false);
        return;
    }
    if(cur_item->toAlbum()) {
        ui->action_edit->setEnabled(true);
        ui->action_del->setEnabled(true);
        ui->action_alb->setEnabled(false);
        ui->action_song->setEnabled(true);
    }
    if(cur_item->toSong()) {
        ui->action_edit->setEnabled(false);
        ui->action_del->setEnabled(true);
        ui->action_alb->setEnabled(false);
        ui->action_song->setEnabled(false);
    }
    if(cur_item->toArtist()) {
        ui->action_edit->setEnabled(true);
        ui->action_del->setEnabled(true);
        ui->action_alb->setEnabled(true);
        ui->action_song->setEnabled(false);
    }


}

void MainWindow::on_action_art_triggered()
{
    ArtistDialog *dialog = new ArtistDialog();
    dialog->addArtist(model, QModelIndex());
    setModified();
}

void MainWindow::on_action_del_triggered()
{
    QModelIndex cur_index = ui->treeView->selectionModel()->currentIndex();
    model->removeRow(cur_index.row(), cur_index.parent());
    update_active_actions(ui->treeView->selectionModel()->currentIndex());
    setModified();
}

void MainWindow::on_action_edit_triggered()
{
    Item *cur_item=static_cast<Item*>(ui->treeView->selectionModel()->currentIndex().internalPointer());
    QModelIndex cur_index = ui->treeView->selectionModel()->currentIndex();
    QModelIndex parent = ui->treeView->selectionModel()->currentIndex().parent();

    if(cur_item->toAlbum()) {
            AlbumDialog *album = new AlbumDialog();
            album->setModel(model);
            album->setModelIndex(cur_index);
            album->show();
    }
    else
    if(cur_item->toArtist()) {
            ArtistDialog *artist = new ArtistDialog();
            artist->setModel(model);
            artist->setModelIndex(cur_index);
            artist->show();
    }
    else
    if(cur_item->toSong()) return;
}

void MainWindow::on_action_alb_triggered()
{
    QModelIndex cur_index = ui->treeView->selectionModel()->currentIndex();
    AlbumDialog *album= new AlbumDialog();
    album->addAlbum(model, cur_index);
    setModified();
}
void MainWindow::on_action_song_triggered()
{
    QModelIndex cur_index = ui->treeView->selectionModel()->currentIndex();
    int row = model->rowCount(cur_index);
    model->insertRow(row, cur_index);
    setModified();
}

bool MainWindow::on_action_save_triggered()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, tr("Сохранить"), tr("/home"), tr("*.db"));
    if (fileName.isEmpty()) return false;
    else {
        QFile file(fileName);
        if(file.open(QIODevice::WriteOnly)) {
            QDataStream out(&file);
            out << *(model->m_root);
            file.close();
            setWindowModified(false);
            return true;
        }
    }
    return false;
}
void MainWindow::on_action_open_triggered()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open"), tr("/home"), tr("*.db"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly)) {
            QDataStream in(&file);
            delete model->m_root;
            setWindowModified(false);
            model->m_root = new Item();
            ui->treeView->reset();
            in >> *(model->m_root);
            file.close();
            ui->action_edit->setEnabled(false);
            ui->action_del->setEnabled(false);
            ui->action_alb->setEnabled(false);
            ui->action_song->setEnabled(false);
        }
    }
    ui->treeView->reset();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if(isWindowModified()) {
        QMessageBox::StandardButton choose = QMessageBox::warning(this, tr("Сохранение документа"), tr("Обнаружены изменения. Сихранить их?"),
                             QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Discard);
        switch (choose)
        {
          case QMessageBox::Save:
              if(!this->on_action_save_triggered())
              {
                  event->ignore();
                  return;
              }
              event->accept();
              return;
          case QMessageBox::Discard:
              event->accept();
              return;
          case QMessageBox::Cancel:
              event->ignore();
              return;
        }
    }
    event->accept();
}

void MainWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
}

void MainWindow::on_action_triggered()
{
    QMessageBox::information(this, tr("Информация"), tr("ИНСТРУКЦИЯ\n\n1) Добавьте Исполнителя, чтобы была возможность добавлять Альбомы\n2) Добавьте Альбом, чтобы была возможность добавлять Песни\n3) Теперь добавьте Песни\n\nЧтобы растянуть колонки таблицы, наведите курсор на разделяющую линию и сдвиньте ее в любую сторону\n\nЧтобы редактировать Песни достаточно клкнуть дважды на нужный параметр (при добавлении новой Песни создается 'шаблон' заполненной Песни - отредактируйте ее, как Вам угодно)\n\nДля редактирования информации об Исполнителях или Альбомах нажмите на кнопку 'Изменить'\n\nСохраняйте Ваши коллекции (кнопка 'Сохранить')\n\nВы можете открыть файл с Вашей сохраненной коллекцией (кнопка 'Открыть')"));
}
