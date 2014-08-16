#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QSettings>

class QDragEnterEvent;
class QDropEvent;
class QImage;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_buttonBrowseOutput_clicked();

private:
    Ui::MainWindow *ui;

    QSettings config;

    void setRowImage(int row, const QImage& image);
};

#endif // MAINWINDOW_HPP
