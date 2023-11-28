#include "secondarywindow.h"
#include "ui_secondarywindow.h"
#include <QDir>
#include <QFile>
#include <QMessageBox>

SecondaryWindow::SecondaryWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondaryWindow)
{
    ui->setupUi(this);
}

/**
 * \brief Constructor for SecondaryWindow with specific parameters.
 *
 * Constructs a SecondaryWindow object with specified action and path, and the specified parent.
 * Closes the window if the provided path is empty.
 * Initializes the UI and loads necessary data.
 *
 * \param path The path for file or folder.
 * \param action The action indicator for 'c'reate, 'r'ename, or 'd'elete.
 * \param parent The parent widget.
 */
SecondaryWindow::SecondaryWindow(const QString &path, const QChar &action, QWidget *parent)
    : QDialog(parent), ui(new Ui::SecondaryWindow), actionIndicator(action), rootPath(path)
{
    if(path.isEmpty())
    {
        close();
    }

    ui->setupUi(this);
    loadNecessaryData();
}

SecondaryWindow::~SecondaryWindow()
{
    delete ui;
}

/**
 * \brief Loads necessary data based on the action indicator.
 * Sets up UI labels and text based on the provided action and file/folder path.
 */
void SecondaryWindow::loadNecessaryData()
{
    QFileInfo fileInfo(rootPath);

    if(actionIndicator == 'd')
    {
        ui->InformationPathLabel->setText(rootPath);
        ui->Label_Name->setText("You are trying to delete file: ");

        ui->InformationNameLabel->setText(fileInfo.baseName());
        ui->InformationSuffixLabel->setText(fileInfo.suffix());
        ui->FileLineEdit->setText(fileInfo.baseName());

        ui->FileLineEdit->setEnabled(false);
    }
    else if(actionIndicator == 'r')
    {
        ui->InformationPathLabel->setText(rootPath);

        ui->InformationNameLabel->setText(fileInfo.baseName());
        ui->InformationSuffixLabel->setText(fileInfo.suffix());
        ui->FileLineEdit->setText(fileInfo.baseName());

        ui->Label_Name->setText("You are trying to rename file: ");
    }
    else if(actionIndicator == 'c')
    {
        if(fileInfo.isDir())
        {
            ui->InformationPathLabel->setText(rootPath);
            ui->InformationNameLabel->setText(fileInfo.baseName());

        }else
        {
            ui->InformationPathLabel->setText(fileInfo.absolutePath());
            ui->InformationNameLabel->setText(fileInfo.dir().dirName());
        }

        ui->Label_Name->setText("You are trying to create folder at: ");
    }
}

/**
 * \brief Slot triggered on 'Accept' button click.
 * Performs respective action based on the action indicator ('d', 'r', 'c').
 * Emits signal 'selectionClear' after action completion.
 */
void SecondaryWindow::on_AcceptButton_clicked()
{
    if (actionIndicator == 'd')
    {
        deleteFile();
    } else if (actionIndicator == 'r')
    {
        renameFile();
    } else if (actionIndicator == 'c')
    {
        addFolder();
    } else
    {
        qDebug() << "Invalid action indicator.";
    }
    emit selectionClear();
}

/**
 * \brief Slot triggered on 'Cancel' button click.
 * Closes the window.
 */
void SecondaryWindow::on_CancelButton_clicked()
{
    close();
}

/**
 * \brief Deletes the file or directory at the provided rootPath.
 * Handles deletion based on existence and type of the item.
 */
void SecondaryWindow::deleteFile()
{
    QFile file(rootPath);
    if (file.exists())
    {
        if (file.remove())
        {
            QMessageBox::information(this, "Success", "File deleted successfully!");
            close();
        } else
        {
            QDir dir(rootPath);

            if (dir.removeRecursively()) {
                QMessageBox::information(this, "Success", "Directory deleted successfully!");
                close();
                return;
            } else {
                QMessageBox::critical(this, "Error", "Failed to delete file!");
            }
        }
    } else
    {
        QMessageBox::warning(this, "Warning", "The file does not exist!");
    }
}

/**
 * \brief Renames the file specified by rootPath with the new name.
 * Performs file rename based on existence and validity of the new name.
 */
void SecondaryWindow::renameFile()
{
    QFile file(rootPath);
    QString newFileName = ui->FileLineEdit->text();

    if (file.exists() && !newFileName.isEmpty())
    {
        QString newFilePath = QFileInfo(rootPath).absolutePath() + "/" + newFileName + QFileInfo(rootPath).suffix();
        QFile newFile(newFilePath);

        if (file.rename(newFilePath))
        {
            close();
            QMessageBox::information(this, "Success", "File renamed successfully!");
        } else
        {
            QMessageBox::critical(this, "Error", "Failed to rename file!");
        }
    } else
    {
        QMessageBox::warning(this, "Warning", "Please enter a valid file name!");
    }
}

/**
 * \brief Creates a folder at the specified rootPath with the provided name.
 * Handles folder creation based on existence and validity of the folder name.
 */
void SecondaryWindow::addFolder()
{
    QDir directoryPath(rootPath);

    if (directoryPath.exists())
    {
        QString name = ui->FileLineEdit->text();
        if (!name.isEmpty())
        {
            QString filePath = directoryPath.absolutePath() + "/" + name;
            QDir newDir(filePath);

            if (!newDir.exists())
            {
                if (newDir.mkdir(filePath))
                {
                    close();
                    QMessageBox::information(this, "Success", "Folder created successfully!");
                } else
                {
                    QMessageBox::critical(this, "Error", "Failed to create folder!");
                }
            } else
            {
                QMessageBox::warning(this, "Warning", "Folder already exists!");
            }
        } else
        {
            QMessageBox::warning(this, "Warning", "Please enter a valid folder name!");
        }
    } else
    {
        QMessageBox::critical(this, "Error", "The directory does not exist!");
    }
}
