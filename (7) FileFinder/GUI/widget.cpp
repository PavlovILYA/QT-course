#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->SizeComboBox->addItem(">");
    ui->SizeComboBox->addItem("<");
    ui->SizeSpinBox->setMaximum(10000000);    // сколько нужно блинский блин?
}

Widget::~Widget()
{
    delete ui;
}

QFileInfoList Widget::findFiles(const QString &startDir, const QStringList &filters)
{
    QFileInfoList infoFiles;
    QDir dir(startDir);

    const auto files = dir.entryInfoList(filters,QDir::Files);
    for(const QFileInfo &file: files)
        infoFiles += file;


    const auto subdirs = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for(const QString &subdir: subdirs)
        infoFiles += findFiles(startDir + '/' + subdir, filters);
    return infoFiles;
}

void Widget::on_browseButton_clicked()
{
    dir_path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->browseLineEdit->setText(dir_path);
}

void Widget::on_startButton_clicked()
{
    if (!dir_path.size())
    {
        QMessageBox::warning(this, "Предупреждение", "Нужно выбрать директорию!");
        return;
    }

    ui->textEdit->clear();

    QStringList masks = {"*"};
    if (ui->FilterLineEdit->text().size())
        masks = ui->FilterLineEdit->text().split(' ');
    qDebug() << masks;

    int days = -1;
    if (ui->AgeCheckBox->isChecked())
    {
        qDebug() << "isDown! (if)";
        days = ui->AgeSpinBox->value();
    }
    qDebug() << "ui->AgeSpinBox->value(): " << ui->AgeSpinBox->value();
    qDebug() << "days: " << days;

    int Kb = -1;
    QString oper = "";
    if (ui->SizeCheckBox->isChecked())
    {
        Kb = ui->SizeSpinBox->value();
        oper = ui->SizeComboBox->currentText();
    }

    QFileInfoList fileNames = findFiles(dir_path, masks);

    //for (auto file : fileNames)
    //    qDebug() << "Loaded: " << file.birthTime().daysTo(QDateTime::currentDateTime());

    // получили все данные
#if defined(Q_OS_MAC)
    QDir dir = QDir::current();
    qDebug() << "Path:" << dir.path();
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();                                    // я сам пишу на макоси
    qDebug() << "Path:" << dir.path();
    dir.cdUp();
    qDebug() << "Path:" << dir.path();
    dir.cd("build-FileFinder-Desktop_Qt_5_13_1_clang_64bit-Debug");
    qDebug() << "Path:" << dir.path();
    dir.cd("SearchPlugin");
    qDebug() << "Path:" << dir.path();
    QString file = dir.path() + "/libSearchPlugin.dylib";

#elif defined(Q_OS_WIN)
    QDir dir = QDir::current();
    dir.cdUp();
    dir.cd("SearchPlugin/debug");
    QString file = dir.path() + "/SearchPlugin.dll";
#endif
    QPluginLoader loader(file);
    if(!loader.load()){
        qDebug() << "Error:" << loader.fileName() << loader.errorString();
        QMessageBox::warning(this, "Предупреждение", "Плагин не был найден! Дело в том, что я пишу на Macintosh и у меня QDir::current() работает немного по-другому (выдает /Users/pavlov/pavlov/build-FileFinder-Desktop_Qt_5_13_1_clang_64bit-Debug/GUI/GUI.app/Contents/MacOS). Чтобы заработало, нужно поменять строчки (строки с 95 по 100 в файле widget.cpp) - настроить путь до плагина (dll). Извините! Не было возможности запустить на другой операционной ссистеме. Не снижайте за это балл(");
        return;
    }

    qDebug() << "Loaded: " << loader.fileName();

    m_iplagin = qobject_cast<IPlugin*>(loader.instance());
    m_iplagin->test("Test has done successfully!");
    m_iplagin->catchParametrs(dir_path, masks, days, oper, Kb);

    // connect
    connect(&m_iplagin->watcher, &QFutureWatcher<void>::resultReadyAt, this, &Widget::res);
    connect(&m_iplagin->watcher, &QFutureWatcher<void>::canceled ,this, &Widget::finish);

    m_iplagin->start(fileNames);

    qDebug() << "Done!" ;

}

void Widget::on_stopButton_clicked()
{
    m_iplagin->stop();
}

void Widget::res(int i)
{
    QFileInfo res;
    res = m_iplagin->watcher.resultAt(i);
    ui->textEdit->append(res.filePath());
}

void Widget::finish()
{
    //QFileInfo res;
    //res = m_iplagin->watcher.result();
    //ui->textEdit->append(res.filePath());
}
