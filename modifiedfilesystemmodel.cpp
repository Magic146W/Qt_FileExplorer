#include "modifiedfilesystemmodel.h"
#include <QDir>
#include <QFileIconProvider>

/**
 * @file modifiedfilesystemmodel.h
 * \brief The ModifiedFileSystemModel class represents a custom model for file data representation in a QListView.
 * This model inherits from QAbstractListModel and manages file data to be displayed in the QListView.
 */
ModifiedFileSystemModel::ModifiedFileSystemModel(QObject *parent) : QAbstractListModel(parent) {}

/**
 * \brief Sets file data from the specified directory path to the model.
 *
 * \param path The directory path containing file data.
 */
void ModifiedFileSystemModel::setFileData(const QString &path)
{
    beginResetModel();

    QDir directory(path);
    QFileInfoList fileInfoList;

    if (acceptsDirectories)
    {
        fileInfoList = sortFilesInsideModel(directory.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot));
    }
    else
    {
        fileInfoList = sortFilesInsideModel(fileInfoList = directory.entryInfoList(QDir::Files));
    }

    fileData.clear();

    for (const QFileInfo &fileInfo : fileInfoList)
    {
        fileData.append(fileInfo);
    }

    endResetModel();
}

/**
 * @brief Sorts the given list of QFileInfo objects.
 * Directories are prioritized first, followed by files sorted by name in a case-insensitive manner.
 *
 * @param fileInfoList The list of QFileInfo objects to be sorted.
 * @return The sorted QFileInfoList.
 */
QFileInfoList ModifiedFileSystemModel::sortFilesInsideModel(const QFileInfoList &fileInfoList)
{
    QFileInfoList sortedList = fileInfoList;

    std::sort(sortedList.begin(), sortedList.end(), [](const QFileInfo &a, const QFileInfo &b)
              {
                  if (a.isDir() && !b.isDir())
                  {
                      return true;
                  }
                  else if (!a.isDir() && b.isDir())
                  {
                      return false;
                  }
                  else
                  {
                      return QString::compare(a.fileName(), b.fileName(), Qt::CaseInsensitive) < 0;
                  }
              });

    return sortedList;
}

/**
 * \brief Returns the number of rows in the model.
 *
 * \param parent The parent QModelIndex.
 * \return int The number of rows in the model (size of the file data list).
 */
int ModifiedFileSystemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return fileData.size();
}

/**
 * \brief Returns the data to be displayed at a specified model index and role.
 *
 * \param index The model index.
 * \param role The requested role (DisplayRole or DecorationRole).
 * \return QVariant The data associated with the specified index and role.
 */
QVariant ModifiedFileSystemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= fileData.size())
    {
        return QVariant();
    }

    const QFileInfo &fileInfo = fileData.at(index.row());

    if (role == Qt::DisplayRole)
    {
        return fileInfo.fileName();
    }
    else if (role == Qt::DecorationRole)
    {
        QFileIconProvider iconProvider;
        QIcon icon = iconProvider.icon(fileInfo);
        return icon;
    }
    else if (role == Qt::ItemIsEditable)
    {
        return isItemEditable(index.row());
    }

    return QVariant();
}

/**
 * @brief Checks if the item at the specified row is editable.
 *
 * @param row The row index of the item.
 * @return bool Returns true if the item is editable; otherwise, returns false.
 */
bool ModifiedFileSystemModel::isItemEditable(int row) const
{
    if (row < 0 || row >= editabilityFlags.size())
        return false;

    return editabilityFlags.at(row);
}

/**
 * @brief Sets the editability status of the item at the specified row.
 *
 * @param row The row index of the item.
 * @param editable The new editability status for the item.
 */
void ModifiedFileSystemModel::setItemEditable(int row, bool editable)
{
    if (row >= 0 && row < editabilityFlags.size())
    {
        editabilityFlags[row] = editable;
        emit dataChanged(index(row, 0), index(row, 0), {Qt::ItemIsEditable});
    }
}

/**
 * @brief Returns the file path for the given model index.
 *
 * @param index The QModelIndex for which to retrieve the file path.
 * @return The file path as a QString.
 */
QString ModifiedFileSystemModel::getFilePathForIndex(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() >= fileData.size())
    {
        return QString();
    }

    const QFileInfo &fileInfo = fileData.at(index.row());
    return fileInfo.filePath();
}

/**
 * @brief Returns the QFileInfo for the given model index.
 *
 * @param index The QModelIndex for which to retrieve the QFileInfo.
 * @return The QFileInfo object.
 */
QFileInfo ModifiedFileSystemModel::getFileInfoForIndex(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() >= fileData.size())
    {
        return QFileInfo();
    }

    return fileData.at(index.row());
}

/**
 * @brief Controls whether the model should include directories (including '.' and '..') in the displayed file data.
 *
 * @param acceptsDirectories If true, directories are included in the model; if false, they are excluded.
 */
void ModifiedFileSystemModel::shouldAcceptDirectories(bool acceptsDirectories)
{
    this->acceptsDirectories = acceptsDirectories;
}
