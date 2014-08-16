#ifndef TABLEWIDGET_HPP
#define TABLEWIDGET_HPP

#include <QSettings>
#include <QTableWidget>

class QImage;
class QNetworkAccessManager;
class QNetworkReply;
class QStringList;

class TableWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit TableWidget(QWidget *parent = 0);

signals:

public slots:

protected:
    QStringList mimeTypes() const override;
    bool dropMimeData(int row, int column, const QMimeData *data, Qt::DropAction action) override;

private:
    QSettings config;

    QNetworkAccessManager *netManager;

    int droppedRow;

    void setThumbnail(int row, const QImage& thumb);

private slots:
    void requestFinished(QNetworkReply *reply);
};

#endif // TABLEWIDGET_HPP
