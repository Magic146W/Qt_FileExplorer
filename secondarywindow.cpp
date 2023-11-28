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

void SecondaryWindow::on_CancelButton_clicked()
{
    close();
}


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
