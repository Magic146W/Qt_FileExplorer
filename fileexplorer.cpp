#include "fileexplorer.h"
#include <QCompleter>
#include <QFileSystemModel>
#include <QFileInfo>

FileExplorer* FileExplorer::instance = nullptr;

FileExplorer::FileExplorer(QObject *parent) : QObject(parent)
{
    fileSystemModel = new QFileSystemModel(this);
}

FileExplorer* FileExplorer::getInstance()
{
    if (!instance) {
        instance = new FileExplorer();
    }
    return instance;
}

void FileExplorer::setupFileSystemModel(const QString& selectedDirectory)
{
    selectedDirectoryPath = selectedDirectory;
    fileSystemModel->setRootPath(selectedDirectory);
    emit SelectionDone(fileSystemModel);
}

void FileExplorer::GetDirectoryContent()
{

}

void FileExplorer::OnTreeViewItemClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QString dirPath = index.data(Qt::DisplayRole).toString();
        QFileInfo fileInfo(dirPath);
        if (fileInfo.isDir()) {
           selectedDirectoryPath = dirPath;
           GetDirectoryContent();
        } else {
            return;
        }
    }
}
