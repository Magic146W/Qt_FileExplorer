#include "listviewmanager.h"
#include "qlineedit.h"
#include <QListView>
#include <QFileSystemModel>

/**
* @file listviewmanager.h
* @brief Manages the interaction and behavior of the QListView component within the application.
*/

ListViewManager::ListViewManager()
{
    modifiedFileSystemModel = new ModifiedFileSystemModel(this);
    connect(this, &ListViewManager::shouldAcceptDirectories, modifiedFileSystemModel, &ModifiedFileSystemModel::shouldAcceptDirectories);
}

ListViewManager::~ListViewManager()
{

}

void ListViewManager::initialize(QListView* listView)
{
    this->listView = listView;
}

ListViewManager& ListViewManager::instance()
{
    static ListViewManager instance;
    return instance;
}

/**
 * \brief Populates the file viewer based on the provided file system model.
 * Updates the file viewer with the contents of the specified directory or model.
 *
 * \param model The file system model used to populate the file viewer.
 */
void ListViewManager::setModelForListView(const QString &path)
{
    modifiedFileSystemModel->setFileData(path);
    listView->setModel(modifiedFileSystemModel);
    listView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

/**
 * @brief Initiates the renaming process on long clicks for the selected item in the ListView.
 * Displays a QLineEdit for renaming, pre-filled with the current filename, and updates the view upon renaming completion or cancellation.
 *
 * @param index The QModelIndex of the clicked item in the QListView.
 */
void ListViewManager::onListViewItemLongClicked(const QModelIndex &index)
{
    if (index.isValid())
    {
        QLineEdit *lineEdit = new QLineEdit(listView);

        lineEdit->setAlignment(Qt::AlignCenter);
        lineEdit->setGeometry(listView->visualRect(index));

        ModifiedFileSystemModel *fileSystemModel = qobject_cast<ModifiedFileSystemModel*>(listView->model());

        if (fileSystemModel)
        {
            QFileInfo fileInfo = fileSystemModel->getFileInfoForIndex(index);

            QString originalFilename = fileInfo.fileName();

            lineEdit->setText(fileInfo.fileName());
            lineEdit->selectAll();

            connect(lineEdit, &QLineEdit::returnPressed, [this, fileInfo, lineEdit, originalFilename]()
                    {
                        handleRenaming(fileInfo, lineEdit, originalFilename);
                    });

            connect(lineEdit, &QLineEdit::editingFinished, [this, lineEdit, originalFilename]()
                    {
                        if (!lineEdit->hasFocus())
                            handleCancelEditing(lineEdit, originalFilename);
                    });

            lineEdit->setFocus();
            lineEdit->show();
        }
    }
}

/**
 * @brief Handles the completion of the renaming process.
 *
 * @param fileInfo Information about the file being renamed.
 * @param lineEdit Pointer to the QLineEdit for editing the file name.
 * @param originalFilename The original filename before editing.
 */
void ListViewManager::handleRenaming(const QFileInfo &fileInfo, QLineEdit *lineEdit, const QString &originalFilename)
{
    QString newFilename = lineEdit->text();

    if (!newFilename.isEmpty() && newFilename != originalFilename)
    {
        QString newPath = fileInfo.absolutePath() + QDir::separator() + newFilename;

        if (QFile::rename(fileInfo.filePath(), newPath))
        {
            this->setModelForListView(fileInfo.absolutePath());
        }
        else
        {
            qDebug() << tr("Error: Failed to rename file.");
        }
    }

    lineEdit->deleteLater();
}

/**
 * @brief Handles the cancellation of the renaming process.
 *
 * @param lineEdit Pointer to the QLineEdit for editing the file name.
 * @param originalFilename The original filename before editing.
 */
void ListViewManager::handleCancelEditing(QLineEdit *lineEdit, const QString &originalFilename)
{
    if (lineEdit->text() != originalFilename)
    {
        lineEdit->setText(originalFilename);
    }

    lineEdit->deleteLater();
}

/**
 * @brief Handles double clicks on items in the ListView.
 *
 * @param index The QModelIndex of the double-clicked item in the QListView.
 */
void ListViewManager::onListViewItemDoubleClicked(const QModelIndex &index)
{
    if (index.isValid())
    {
        QString filePath = modifiedFileSystemModel->getFilePathForIndex(index);
        QFileInfo fileInfo(filePath);
        if (fileInfo.isDir())
        {
            emit updateViewData(filePath);
        }
        else
        {
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                return;
            }

            QFileInfo fileInfo(filePath);

            if (fileInfo.suffix().compare("png", Qt::CaseInsensitive) == 0 ||
                fileInfo.suffix().compare("jpg", Qt::CaseInsensitive) == 0 ||
                fileInfo.suffix().compare("jpeg", Qt::CaseInsensitive) == 0 ||
                fileInfo.suffix().compare("bmp", Qt::CaseInsensitive) == 0 ||
                fileInfo.suffix().compare("gif", Qt::CaseInsensitive) == 0 ||
                fileInfo.suffix().compare("tiff", Qt::CaseInsensitive) == 0)
            {
                emit callFileViewerDialog(filePath, true);
            }
            else if (fileInfo.suffix().compare("txt", Qt::CaseInsensitive) == 0 ||
                     fileInfo.suffix().compare("text", Qt::CaseInsensitive) == 0)
            {
                emit callFileViewerDialog(filePath, false);
            }
        }
    }
}

/**
 * @brief Retrieves the file path of the selected item in the ListView.
 *
 * @param index The QModelIndex of the selected item in the QListView.
 * @return The file path of the selected item, or an empty string if the index is invalid.
 */
QString ListViewManager::listViewSelectedItemPath(const QModelIndex &index)
{
    if (index.isValid())
    {
        return modifiedFileSystemModel->getFilePathForIndex(index);
    }
    return "";
}
