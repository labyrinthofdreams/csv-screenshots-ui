#include <string>
#include <vector>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "mainwindow.hpp"
#include "sfcsv/sfcsv.h"
#include "ui_mainwindow.h"

static QStringList parseCsv(const QString& in) {
    QStringList result;

    std::vector<std::string> parsed;
    const std::string line = in.toStdString();
    sfcsv::parse_line(line, std::back_inserter(parsed), ',');
    for(auto&& res : parsed) {
        result.append(QString::fromStdString(res));
    }

    return result;
}

static QStringList parseFile(const QString& in) {
    QStringList results;

    QFile inFile(in);
    if(!inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Unable to open file");
    }

    QTextStream stream(&inFile);
    stream.setCodec("UTF-8");
    while(!stream.atEnd()) {
        const QString line = stream.readLine().remove("\n");
        results << line;
    }

    return results;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    config("CsvScreenshotFinderConfig.ini", QSettings::IniFormat)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered() try
{
    const QString openedFile = QFileDialog::getOpenFileName(this, tr("Open..."),
                                                            config.value("last_opened").toString());
    if(openedFile.isEmpty()) {
        return;
    }

    config.setValue("last_opened", openedFile);

    QStringList lines = parseFile(openedFile);
    const QString header = lines.takeFirst();
    QStringList headerParsed = parseCsv(header);
    headerParsed.prepend("Thumbnail");

    ui->tableCsv->clear();
    ui->tableCsv->setColumnCount(headerParsed.size() + 1);
    ui->tableCsv->setHorizontalHeaderLabels(headerParsed);
    ui->tableCsv->setRowCount(lines.size());
    ui->tableCsv->verticalHeader()->hide();
    ui->tableCsv->setColumnWidth(0, 155);

    QProgressDialog progress(tr("Loading images"), tr("Cancel"), 0, lines.size() - 1);
    progress.setMinimumDuration(0);
    progress.setModal(true);
    QDir inputDir("F:\\ICM_Highest_Rated\\Screenshots");
    for(int row = 0; row < lines.size(); ++row) {
        const QStringList columns = parseCsv(lines.at(row));

        for(int column = 0; column < columns.size(); ++column) {
            auto item = new QTableWidgetItem(columns.at(column));
            ui->tableCsv->setItem(row, column + 1, item);
        }

        if(columns.size() >= 2) {
            const QString imageName = QString("%1.jpg").arg(columns.at(1));
            const QString imagePath = inputDir.absoluteFilePath(imageName);
            //qDebug() << imagePath;
            if(QFile::exists(imagePath)) {
                QImage image(imagePath);
                setRowImage(row, image);
            }
        }

        progress.setValue(row);
        if(progress.wasCanceled()) {
            ui->tableCsv->setRowCount(row);
            break;
        }
    }
}
catch(const std::exception& exc) {
    QMessageBox::critical(this, tr("CSV Error"), QString(exc.what()));
}

void MainWindow::on_buttonBrowseOutput_clicked()
{
    const auto outputPath =
            QFileDialog::getExistingDirectory(this,
                                              tr("Select output directory"),
                                              config.value("last_output_dir").toString());
    if(outputPath.isEmpty()) {
        return;
    }

    config.setValue("last_output_dir", outputPath);
}

void MainWindow::setRowImage(const int row, const QImage& image)
{
    QPixmap thumb = QPixmap::fromImage(image.scaledToWidth(150));
    auto item = new QTableWidgetItem;
    item->setData(Qt::DecorationRole, thumb);
    //item->setData(Qt::SizeHintRole, thumb.size());
    //item->setSizeHint(thumb.size());
    ui->tableCsv->setItem(row, 0, item);
    ui->tableCsv->setRowHeight(row, thumb.height());
}
