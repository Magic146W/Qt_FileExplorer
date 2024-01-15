#include "fileviewerdialog.h"
#include "ui_fileviewerdialog.h"
#include <QTextStream>
#include <QFileDialog>
#include <QFile>
#include <QTextEdit>
#include <QLabel>
#include <QPixmap>

/**
 * @file fileviewerdialog.h
 * @brief The FileViewerDialog class manages the display of text and image files in a QDialog window.
 */

FileViewerDialog::FileViewerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileViewerDialog)
{
    ui->setupUi(this);
}

FileViewerDialog::~FileViewerDialog()
{
    delete ui;
}

/**
 * \brief The FileViewerDialog class manages the display of text and image files in a QDialog window.
 */

/**
 * \brief Loads and displays the content of a text file in a QTextEdit within the dialog window.
 *
 * \param filePath The path of the text file to be displayed.
 */
void FileViewerDialog::loadTextFile(const QString& filePath)
{
    QTextEdit* textEdit = new QTextEdit(this);

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        delete textEdit;
        close();
        return;
    }

    QTextStream textStream(&file);

    QString fileContent;
    int lineCount = 0;
    while (!textStream.atEnd() && lineCount < 100)
    {
        fileContent += textStream.readLine() + "\n";
        ++lineCount;
    }

    file.close();

    textEdit->setPlainText(fileContent);
    textEdit->setReadOnly(true);
    ui->QFrame_FileViewer->layout()->addWidget(textEdit);
}

/**
 * \brief Opens and displays an image file in a QLabel within the dialog window.
 *
 * \param filePath The path of the image file to be displayed.
 */
void FileViewerDialog::openImage(const QString& filePath)
{
    QLabel* imageLabel = new QLabel(this);
    QPixmap image(filePath);
    if (!image.isNull())
    {
        QSize dialogSize = this->size();
        QSize imageSize = image.size();
        QSize scaledSize = imageSize.scaled(dialogSize, Qt::KeepAspectRatio);
        imageLabel->setPixmap(image.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        this->resize(scaledSize);
        ui->QFrame_FileViewer->layout()->addWidget(imageLabel);
    }
}
