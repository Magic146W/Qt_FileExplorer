#ifndef SECONDARYWINDOW_H
#define SECONDARYWINDOW_H

#include <QDialog>
#include <QString>
#include <QChar>


namespace Ui {
class SecondaryWindow;
}

class SecondaryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondaryWindow(QWidget *parent = nullptr);
    explicit SecondaryWindow(const QString &path, const QChar &action, QWidget *parent = nullptr);
    ~SecondaryWindow();

     void initializeDialog(const QString &rootPath, const QChar &actionIndicator);


signals:
     void selectionClear();


private slots:
     void on_CancelButton_clicked();
    void on_AcceptButton_clicked();


private:
    Ui::SecondaryWindow *ui;
    QChar actionIndicator;
    QString rootPath;

    void loadNecessaryData();
    void deleteFile();
    void renameFile();
    void addFolder();
};

#endif // SECONDARYWINDOW_H
