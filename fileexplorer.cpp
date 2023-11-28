#include "fileexplorer.h"
#include <QCompleter>
#include <QFileSystemModel>
#include <QFileInfo>

FileExplorer* FileExplorer::instance = nullptr;

FileExplorer::FileExplorer(QObject *parent) : QObject(parent)
{
    fileSystemModel = new QFileSystemModel(this);
}

FileExplorer* FileExplorer::getInstance(QObject *parent) {
    if (!instance) {
        instance = new FileExplorer(parent);
    }
    return instance;
}

void FileExplorer::setupFileSystemModel(const QString& selectedDirectory)
{
    QFileInfo fileInfo(selectedDirectory);

    if (fileInfo.isDir())
    {
        fileSystemModel->setRootPath(selectedDirectory);
    }

    emit SelectionDone(fileSystemModel);
}

void FileExplorer::OnTreeViewItemClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QString dirPath = fileSystemModel->filePath(index);
        fileSystemModel->setRootPath(dirPath);
        emit SelectionDone(fileSystemModel);
    }
}

void FileExplorer::OnListViewItemClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QString dirPath = fileSystemModel->filePath(index);
        emit FileSelected(dirPath);
    }
}
