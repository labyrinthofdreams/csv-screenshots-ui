#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "tablewidget.hpp"

TableWidget::TableWidget(QWidget *parent) :
    QTableWidget(parent),
    config("CsvScreenshotFinderConfig.ini", QSettings::IniFormat)
{
    setAcceptDrops(true);
}

QStringList TableWidget::mimeTypes() const
{
    return {"text/uri-list"};
}

bool TableWidget::dropMimeData(int row, int column, const QMimeData* data, Qt::DropAction action)
{
    const auto urls = data->urls();
    if(urls.size() != 1) {
        return false;
    }

    const QUrl& url = urls.first();

    const QDir outputDir(config.value("last_output_dir").toString());
    if(!outputDir.exists()) {
        QMessageBox::critical(this, tr("Invalid output path"), tr("Set valid output directory"));

        return false;
    }

    if(url.scheme() != "file") {
        return false;
    }

    const QString imageName = QString("%1.jpg").arg(item(row, 2)->text());
    const QString outputPath = outputDir.absoluteFilePath(imageName);
    qDebug() << imageName << outputPath;

    if(!QFile::exists(outputPath)) {
        const QString imagePath = url.toLocalFile();
        QImage image(imagePath);
        image.save(outputPath, "JPEG", 100);

        QImage thumb = image.scaledToWidth(150);

        auto item = new QTableWidgetItem;
        item->setData(Qt::DecorationRole, QPixmap::fromImage(thumb));
        setItem(row, 0, item);
        setRowHeight(row, thumb.height());
    }

    return true;
}
