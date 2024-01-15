#include "fileoperationsdialog.h"
#include "ui_fileoperationsdialog.h"
#include <QDir>
#include <QFile>
#include <QMessageBox>

/**
 * @file fileoperationsdialog.h
 * @brief The FileOperationsDialog class provides a dialog for file and folder operations.
 */

FileOperationsDialog::FileOperationsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileOperationsDialog)
{
    ui->setupUi(this);
}

FileOperationsDialog::FileOperationsDialog(const QString &path, const QChar &action, QWidget *parent)
    : QDialog(parent), ui(new Ui::FileOperationsDialog), actionIndicator(action), rootPath(path)
{
    if(path.isEmpty())
    {
        close();
    }

    ui->setupUi(this);
    loadNecessaryData();
}

FileOperationsDialog::~FileOperationsDialog()
{
    delete ui;
}

/**
 * \brief Loads necessary data based on the action indicator.
 */
void FileOperationsDialog::loadNecessaryData()
{
    QFileInfo fileInfo(rootPath);

    if(actionIndicator == 'd')
    {
        ui->QLabel_FileInformation->setText("You are trying to delete file: ");
        ui->QLabel_FullPath->setText(rootPath);
        ui->QLabel_FileName->setText(fileInfo.baseName());
        ui->QLabel_FileSuffix->setText(fileInfo.suffix());
        ui->QLineEdit_InputField->setText(fileInfo.baseName());
        ui->QLineEdit_InputField->setEnabled(false);
    }
    else if(actionIndicator == 'r')
    {
        ui->QLabel_FileInformation->setText("You are trying to rename file: ");
        ui->QLabel_FullPath->setText(rootPath);
        ui->QLabel_FileName->setText(fileInfo.baseName());
        ui->QLabel_FileSuffix->setText(fileInfo.suffix());
        ui->QLineEdit_InputField->setText(fileInfo.baseName());
    }
    else if(actionIndicator == 'c')
    {
        ui->QLabel_FileInformation->setText("You are trying to create folder at: ");

        if(fileInfo.isDir())
        {
            ui->QLabel_FullPath->setText(rootPath);
            ui->QLabel_FileName->setText(fileInfo.baseName());
        }
        else
        {
            ui->QLabel_FullPath->setText(fileInfo.absolutePath());
            ui->QLabel_FileName->setText(fileInfo.dir().dirName());
        }
    }
}

/**
 * \brief Deletes the file or directory at the provided rootPath.
 */
void FileOperationsDialog::deleteFile()
{
    QFile file(rootPath);
    if (file.exists())
    {
        if (file.remove())
        {
            emit refresh();
            close();
        }
        else
        {
            QDir dir(rootPath);

            if (dir.removeRecursively()) {
                emit refresh();
                close();
                return;
            }
            else
            {
                QMessageBox::critical(this, "Error", "Failed to delete file!");
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "Warning", "The file does not exist!");
    }
}

/**
 * \brief Renames the file specified by rootPath with the new name.
 */
void FileOperationsDialog::renameFile()
{
    QFile file(rootPath);
    QString newFileName = ui->QLineEdit_InputField->text();

    if (file.exists() && !newFileName.isEmpty())
    {
        QString newFilePath = QFileInfo(rootPath).absolutePath() + "/" + newFileName + QFileInfo(rootPath).suffix();
        QFile newFile(newFilePath);

        if (file.rename(newFilePath))
        {
            emit refresh();
            close();
            QMessageBox::information(this, "Success", "File renamed successfully!");
        }
        else
        {
            QMessageBox::critical(this, "Error", "Failed to rename file!");
        }
    }
    else
    {
        QMessageBox::warning(this, "Warning", "Please enter a valid file name!");
    }
}

/**
 * \brief Creates a folder at the specified rootPath with the provided name.
 * Handles folder creation based on existence and validity of the folder name.
 */
void FileOperationsDialog::addFolder()
{
    QDir directoryPath(rootPath);

    if (directoryPath.exists())
    {
        QString name = ui->QLineEdit_InputField->text();
        if (!name.isEmpty())
        {
            QString filePath = directoryPath.absolutePath() + "/" + name;
            QDir newDir(filePath);

            if (!newDir.exists())
            {
                if (newDir.mkdir(filePath))
                {
                    emit refresh();
                    close();
                    QMessageBox::information(this, "Success", "Folder created successfully!");
                }
                else
                {
                    QMessageBox::critical(this, "Error", "Failed to create folder!");
                }
            }
            else
            {
                QMessageBox::warning(this, "Warning", "Folder already exists!");
            }
        }
        else
        {
            QMessageBox::warning(this, "Warning", "Please enter a valid folder name!");
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", "The directory does not exist!");
    }
}

/**
 * \brief Slot triggered on 'Cancel' button click.
 */
void FileOperationsDialog::on_QPushButton_Cancel_clicked()
{
    close();
}

/**
 * \brief Slot triggered on 'Accept' button click.
 */
void FileOperationsDialog::on_QPushButton_Accept_clicked()
{
    if (actionIndicator == 'd')
    {
        deleteFile();
    }
    else if (actionIndicator == 'r')
    {
        renameFile();
    }
    else if (actionIndicator == 'c')
    {
        addFolder();
    }
    else
    {
        qDebug() << "Invalid action indicator.";
    }
}
