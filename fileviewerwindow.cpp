#include "fileviewerwindow.h"
#include "ui_fileviewerwindow.h"
#include <QTextStream>
#include <QFileDialog>
#include <QFile>
#include <QTextEdit>
#include <QLabel>
#include <QPixmap>
/**
 * \brief The FileViewerWindow class manages the display of text and image files in a QDialog window.
 */

/**
 * \brief Constructs a FileViewerWindow object with the specified parent.
 *
 * \param parent The parent QWidget.
 */
FileViewerWindow::FileViewerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileViewerWindow)
{
    ui->setupUi(this);
}

FileViewerWindow::~FileViewerWindow()
{
    delete ui;
}

/**
 * \brief Loads and displays the content of a text file in a QTextEdit within the dialog window.
 *
 * Reads the content of the text file at the specified file path, limits the text to 100 lines, and displays it
 * within a QTextEdit widget in the dialog window.
 *
 * \param filePath The path of the text file to be displayed.
 */
void FileViewerWindow::loadTextFile(const QString& filePath)
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
    while (!textStream.atEnd() && lineCount < 100) {
        fileContent += textStream.readLine() + "\n";
        ++lineCount;
    }

    file.close();

    textEdit->setPlainText(fileContent);
    textEdit->setReadOnly(true);
    ui->ShowFileWidget->layout()->addWidget(textEdit);
}

/**
 * \brief Opens and displays an image file in a QLabel within the dialog window.
 *
 * Loads the image file at the specified file path, scales it to fit the dialog window while maintaining aspect ratio,
 * and displays it within a QLabel widget in the dialog window.
 *
 * \param filePath The path of the image file to be displayed.
 */
void FileViewerWindow::openImage(const QString& filePath)
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
        ui->ShowFileWidget->layout()->addWidget(imageLabel);
    }
}
