#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "listviewmanager.h"
#include "treeviewmanager.h"
#include "visualmodeupdater.h"
#include <QMainWindow>
#include <QSplitter>
#include <QFileSystemModel>
#include <QListView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ListViewManager& listViewManager;
    TreeViewManager& treeViewManager;
    VisualModeUpdater& visuals;
    QSplitter *splitter;

    void initializeMainWindow();
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);
    QSplitter* splitterLeftAndRightPanels();
    bool loadLayout();
    void saveLayout();
    bool isLightMode = true;
    bool isShowFiles = true;
    bool isGridLayout = true;
    void updateIcons();

public slots:
    void updateTreeView(const QString& path);
    void openFileViewerDialog(const QString &path, bool isImage);
    void refresh();

private slots:
    void on_QPushButton_AddFolder_clicked();
    void on_QPushButton_DeleteFile_clicked();
    void on_QPushButton_RenameFile_clicked();
    void on_QPushButton_CopyButton_clicked();
    void on_QPushButton_DirectoryButton_clicked();
    void on_QPushButton_ShowPanel_clicked();
    void on_QPushButton_LightDarkModePushButton_clicked();
    void on_QPushButton_LayoutPushButton_clicked();
    void on_QPushButton_HideFilesPushButton_clicked();

    void handleSplitterMoved(int pos, int index);

signals:
    void populateTreeView(const QString &path);
    void populateListView(const QString &path);
    void updateHideFilesFilter(bool isVisible);
    void updateLightModeBooleanData(bool isLight,bool isShowGrid,bool isShowFiles);

};
#endif // MAINWINDOW_H
