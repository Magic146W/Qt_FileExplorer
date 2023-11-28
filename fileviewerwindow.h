#ifndef FILEVIEWERWINDOW_H
#define FILEVIEWERWINDOW_H

#include <QDialog>
#include <QFile>


namespace Ui {
class FileViewerWindow;
}

class FileViewerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FileViewerWindow(QWidget *parent = nullptr);
    ~FileViewerWindow();

    void loadTextFile(const QString& filePath);
    void openImage(const QString& filePath);


private:
    Ui::FileViewerWindow *ui;
};

#endif // FILEVIEWERWINDOW_H
