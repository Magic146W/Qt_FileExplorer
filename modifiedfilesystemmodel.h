#ifndef MODIFIEDFILESYSTEMMODEL_H
#define MODIFIEDFILESYSTEMMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QFileInfoList>

class ModifiedFileSystemModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ModifiedFileSystemModel(QObject *parent = nullptr);
    void setFileData(const QString &path);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool isItemEditable(int row) const;
    void setItemEditable(int row, bool editable);
    QString getFilePathForIndex(const QModelIndex &index) const;
    QFileInfo getFileInfoForIndex(const QModelIndex &index) const;

private:
    QFileInfoList fileData;
    QList<bool> editabilityFlags;
    bool acceptsDirectories;
    QFileInfoList sortFilesInsideModel(const QFileInfoList &fileInfoList);

public slots:
    void shouldAcceptDirectories(bool acceptsDirectories);

};
#endif // MODIFIEDFILESYSTEMMODEL_H
