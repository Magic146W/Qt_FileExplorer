#include "fileexplorer.h"
#include <QCompleter>
#include <QFileSystemModel>
#include <QFileInfo>

/**
 * @brief The FileExplorer class manages file exploration functionality within the MainWindow.
 *
 * This class implements a singleton pattern to handle tree and list view items' file exploration in the MainWindow.
 */
FileExplorer* FileExplorer::instance = nullptr;

FileExplorer::FileExplorer(QObject *parent) : QObject(parent)
{
    fileSystemModel = new QFileSystemModel(this);
}

/**
 * @brief Returns the instance of the FileExplorer singleton object with the specified parent.
 *
 * If the FileExplorer instance does not exist, it creates one and sets the specified parent.
 * This class follows the Singleton design pattern, ensuring only one instance exists throughout the application.
 *
 * @param parent The parent QObject.
 * @return FileExplorer* A pointer to the FileExplorer singleton instance.
 */
FileExplorer* FileExplorer::getInstance(QObject *parent) {
    if (!instance) {
        instance = new FileExplorer(parent);
    }
    return instance;
}

/**
 * @brief Sets up the file system model with the selected directory.
 *
 * @param selectedDirectory The selected directory path.
 */
void FileExplorer::setupFileSystemModel(const QString& selectedDirectory)
{
    QFileInfo fileInfo(selectedDirectory);

    if (fileInfo.isDir())
    {
        fileSystemModel->setRootPath(selectedDirectory);
    }

    emit SelectionDone(fileSystemModel);
}

/**
 * \brief Handles the event of a tree view item being clicked.
 *
 * \param index The index of the tree view item clicked.
 */
void FileExplorer::OnTreeViewItemClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QString dirPath = fileSystemModel->filePath(index);
        fileSystemModel->setRootPath(dirPath);
        emit SelectionDone(fileSystemModel);
    }
}

/**
 * @brief Handles the event of a list view item being clicked.
 *
 * @param index The index of the list view item clicked.
 */
void FileExplorer::OnListViewItemClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QString dirPath = fileSystemModel->filePath(index);
        emit FileSelected(dirPath);
    }
}
