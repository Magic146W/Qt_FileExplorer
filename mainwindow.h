#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fileexplorer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_LightDarkMode_stateChanged(int state);

    void on_LayoutCheckBox_stateChanged(int state);

    void on_HideFilesCheckBox_stateChanged(int state);

    void on_SearchIcon_clicked();

    void on_DirectoryButton_clicked();

    void OnObjectEntered(QObject* obj);

    void OnObjectLeft(QObject* obj);

    void SetToolTipBehavior(QObject* object, const QString& tooltipText);

    void ShowTree(QFileSystemModel* model);

private:
    Ui::MainWindow *ui;

    void MainWindowDisplayChanges();

    FileExplorer *fileExplorer;
    QWidget *mainWidget;
    QVBoxLayout *mainLayout;
    QMap<QObject*, QString> tooltipMap;
};
#endif // MAINWINDOW_H
