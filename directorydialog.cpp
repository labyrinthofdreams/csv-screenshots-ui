#include <QtCore>
#include <QtWidgets>
#include "directorydialog.hpp"
#include "ui_directorydialog.h"

DirectoryDialog::DirectoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DirectoryDialog),
    config("CsvScreenshotFinderConfig.ini", QSettings::IniFormat)
{
    ui->setupUi(this);

    ui->listDirectories->addItems(config.value("image_dirs").toStringList());
}

DirectoryDialog::~DirectoryDialog()
{
    delete ui;
}

void DirectoryDialog::on_buttonAdd_clicked()
{
    const QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory"));
    ui->listDirectories->addItem(dir);

    QStringList dirs = config.value("image_dirs").toStringList();
    dirs << dir;
    config.setValue("image_dirs", dirs);
}

void DirectoryDialog::on_pushButton_clicked()
{
    const QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory"));
    const QString selected = ui->listDirectories->currentItem()->text();
    ui->listDirectories->currentItem()->setText(dir);

    QStringList dirs = config.value("image_dirs").toStringList();
    dirs.removeOne(selected);
    dirs << dir;
    config.setValue("image_dirs", dirs);

    qDebug() << dirs;
}

void DirectoryDialog::on_buttonRemove_clicked()
{
    const int current = ui->listDirectories->currentRow();
    QListWidgetItem *item = ui->listDirectories->takeItem(current);

    QStringList dirs = config.value("image_dirs").toStringList();
    dirs.removeOne(item->text());
    config.setValue("image_dirs", dirs);

    delete item;
}
