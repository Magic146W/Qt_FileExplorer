#include "filelistmodel.h"
#include <QDir>
#include <QFileIconProvider>

/**
 * \brief The FileListModel class represents a custom model for file data representation in a QListView.
 *
 * This model inherits from QAbstractListModel and manages file data to be displayed in the QListView.
 */
FileListModel::FileListModel(QObject *parent) : QAbstractListModel(parent) {}

/**
 * \brief Sets file data from the specified directory path to the model.
 *
 * Clears the existing data and populates the model with file information (excluding '.' and '..' entries)
 * from the given directory path.
 *
 * \param path The directory path containing file data.
 */
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

/**
 * \brief Returns the number of rows in the model.
 *
 * \param parent The parent QModelIndex.
 * \return int The number of rows in the model (size of the file data list).
 */
int FileListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return fileData.size();
}

/**
 * \brief Returns the data to be displayed at a specified model index and role.
 *
 * Retrieves the file name for the DisplayRole and icon for the DecorationRole
 * at the given QModelIndex.
 *
 * \param index The model index.
 * \param role The requested role (DisplayRole or DecorationRole).
 * \return QVariant The data associated with the specified index and role.
 */
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
