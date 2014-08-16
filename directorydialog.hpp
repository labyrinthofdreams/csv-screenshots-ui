#ifndef DIRECTORYDIALOG_HPP
#define DIRECTORYDIALOG_HPP

#include <QDialog>
#include <QSettings>

namespace Ui {
class DirectoryDialog;
}

class DirectoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DirectoryDialog(QWidget *parent = 0);
    ~DirectoryDialog();

private slots:
    void on_buttonAdd_clicked();

    void on_pushButton_clicked();

    void on_buttonRemove_clicked();

private:
    Ui::DirectoryDialog *ui;

    QSettings config;
};

#endif // DIRECTORYDIALOG_HPP
