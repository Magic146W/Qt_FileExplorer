#ifndef FILEVIEWERDIALOG_H
#define FILEVIEWERDIALOG_H

#include <QDialog>
#include <QFile>

namespace Ui {
class FileViewerDialog;
}

class FileViewerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileViewerDialog(QWidget *parent = nullptr);
    ~FileViewerDialog();
    void loadTextFile(const QString& filePath);
    void openImage(const QString& filePath);


private:
    Ui::FileViewerDialog *ui;
};

#endif // FILEVIEWERDIALOG_H
