#include "filelistmodel.h"
#include <QDir>
#include <QFileIconProvider>

FileListModel::FileListModel(QObject *parent) : QAbstractListModel(parent) {}

void FileListModel::setFileData(const QString &path)
{
    beginResetModel();

    QDir directory(path);

    QFileInfoList fileInfoList = directory.entryInfoList(QDir::Files);

    fileData.clear();

    for (const QFileInfo &fileInfo : fileInfoList)
    {
        if (fileInfo.fileName() != "." && fileInfo.fileName() != "..")
        {
            fileData.append(fileInfo);
        }
    }

    endResetModel();
}

int FileListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return fileData.size();
}

QVariant FileListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= fileData.size())
    {
        return QVariant();
    }

    const QFileInfo &fileInfo = fileData.at(index.row());

    if (role == Qt::DisplayRole)
    {
        return fileInfo.fileName();
    } else if (role == Qt::DecorationRole)
    {
        QFileIconProvider iconProvider;
        QIcon icon = iconProvider.icon(fileInfo);
        return icon;
    }

    return QVariant();
}
