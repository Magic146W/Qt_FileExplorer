#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fileexplorer.h"
#include "noemptyfoldersmodel.h"
#include "secondarywindow.h"
#include "filelistmodel.h"
#include "fileviewerwindow.h"
#include <QSplitter>

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
    void on_LightDarkModeCheckBox_stateChanged(int state);
    void on_LayoutCheckBox_stateChanged(int state);
    void on_HideFilesCheckBox_stateChanged(int state);
    void on_DirectoryButton_clicked();
    void on_CopyButton_clicked();
    void on_AddFolder_clicked();
    void on_RenameFile_clicked();
    void on_DeleteFile_clicked();

    void onListViewItemDoubleClicked(const QModelIndex &index);
    void ShowTree(QFileSystemModel* model);
    void PopulateFileViewer(QFileSystemModel* model);
    void handleFileContainerFrameResize();
    void clearListViewSelection();


public slots:
    void GetSelectedItemPath(QString path);


private:
    Ui::MainWindow *ui;
    QWidget *mainWidget;
    QVBoxLayout *mainLayout;
    QMap<QObject*, QString> tooltipMap;
    QFileSystemModel *localFileSystemModel;
    NoEmptyFoldersModel *noEmptyfolderModel;
    FileListModel *fileListModel;
    QSplitter *splitter;

    QString selectedItemPath;
    QString treeViewSelectedItemPath();
    void splitterLeftAndRightPanels();
    void changeLightDarkMode();
    void loadLayout();
    void loadStyleSheet();
    void saveLayout();
    QString updateIconColorName(QString filePath);
    void updateIconsToMode();


protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

signals:
    void resized();

};
#endif // MAINWINDOW_H
