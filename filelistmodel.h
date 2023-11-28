#ifndef FILELISTMODEL_H
#define FILELISTMODEL_H

#include <QAbstractListModel>
#include <QFileInfoList>

class FileListModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit FileListModel(QObject *parent = nullptr);
    QFileInfoList fileData;

    void setFileData(const QString &path);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};
#endif // FILELISTMODEL_H
