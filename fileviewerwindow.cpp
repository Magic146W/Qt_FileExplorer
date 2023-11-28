#include "fileviewerwindow.h"
#include "ui_fileviewerwindow.h"
#include <QTextStream>
#include <QFileDialog>
#include <QFile>
#include <QTextEdit>
#include <QLabel>
#include <QPixmap>

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
