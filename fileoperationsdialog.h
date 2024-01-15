#ifndef FILEOPERATIONSDIALOG_H
#define FILEOPERATIONSDIALOG_H

#include "visualmodeupdater.h"
#include <QDialog>
#include <QString>
#include <QChar>

namespace Ui {
class FileOperationsDialog;
}

class FileOperationsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileOperationsDialog(QWidget *parent = nullptr);
    explicit FileOperationsDialog(const QString &path, const QChar &action, QWidget *parent = nullptr);
    ~FileOperationsDialog();

    void initializeDialog(const QString &rootPath, const QChar &actionIndicator);

signals:
    void selectionClear();
    void refresh();

private slots:
    void on_QPushButton_Cancel_clicked();
    void on_QPushButton_Accept_clicked();

private:
    Ui::FileOperationsDialog *ui;
    QChar actionIndicator;
    QString rootPath;

    void loadNecessaryData();
    void deleteFile();
    void renameFile();
    void addFolder();
};

#endif // FILEOPERATIONSDIALOG_H
