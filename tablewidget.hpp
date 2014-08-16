#ifndef TABLEWIDGET_HPP
#define TABLEWIDGET_HPP

#include <QTableWidget>
#include <QSettings>

class QStringList;

class TableWidget : public QTableWidget
{
    Q_OBJECT

    QSettings config;

public:
    explicit TableWidget(QWidget *parent = 0);

signals:

public slots:

protected:
    QStringList mimeTypes() const override;
    bool dropMimeData(int row, int column, const QMimeData *data, Qt::DropAction action) override;
};

#endif // TABLEWIDGET_HPP
