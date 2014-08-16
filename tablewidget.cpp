#include <QtCore>
#include <QtGui>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtWidgets>
#include "tablewidget.hpp"

TableWidget::TableWidget(QWidget *parent) :
    QTableWidget(parent),
    config("CsvScreenshotFinderConfig.ini", QSettings::IniFormat),
    netManager(nullptr),
    droppedRow(0)
{
    netManager = new QNetworkAccessManager(this);

    connect(netManager, SIGNAL(finished(QNetworkReply*)),
            this,       SLOT(requestFinished(QNetworkReply*)));

    setAcceptDrops(true);
}

QStringList TableWidget::mimeTypes() const
{
    return {"text/uri-list"};
}

bool TableWidget::dropMimeData(int row, int column, const QMimeData* data, Qt::DropAction action)
{
    Q_UNUSED(column);
    Q_UNUSED(action);

    const auto urls = data->urls();
    if(urls.size() != 1) {
        return false;
    }

    const QDir outputDir(config.value("last_output_dir").toString());
    if(!outputDir.exists()) {
        QMessageBox::critical(this, tr("Invalid output path"), tr("Set valid output directory"));

        return false;
    }

    const QUrl& url = urls.first();
    // Firefox gives a local file path while Chrome gives an http url
    if(url.scheme() == "http" || url.scheme() == "https") {
        // Disable widget while downloading
        setEnabled(false);
        droppedRow = row;
        netManager->get(QNetworkRequest(url));
    }
    else {
        const QString imageName = QString("%1.jpg").arg(item(row, 2)->text());
        const QString outputPath = outputDir.absoluteFilePath(imageName);
        if(!QFile::exists(outputPath)) {
            const QString imagePath = url.toLocalFile();
            QImage image(imagePath);
            image.save(outputPath, "JPEG", 100);
            setThumbnail(row, image.scaledToWidth(150));
        }
    }

    return true;
}

void TableWidget::setThumbnail(int row, const QImage& thumb)
{
    auto item = new QTableWidgetItem;
    item->setData(Qt::DecorationRole, QPixmap::fromImage(thumb));
    setItem(row, 0, item);
    setRowHeight(row, thumb.height());
}

void TableWidget::requestFinished(QNetworkReply* reply)
{
    QImage image;
    image.loadFromData(reply->readAll());
    if(image.isNull()) {
        QMessageBox::critical(this, tr("Invalid image data"), tr("Not a valid image"));
        return;
    }

    const QDir outputDir(config.value("last_output_dir").toString());
    const QString imageName = QString("%1.jpg").arg(item(droppedRow, 2)->text());
    const QString outputPath = outputDir.absoluteFilePath(imageName);
    if(!QFile::exists(outputPath)) {
        image.save(outputPath, "JPEG", 100);
        setThumbnail(droppedRow, image.scaledToWidth(150));
    }

    setEnabled(true);
}
