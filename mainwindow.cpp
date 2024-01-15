#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "fileviewerdialog.h"
#include "fileoperationsdialog.h"
#include "itemnamemodifierdelegate.h"
#include "longclickhandler.h"
#include "visualmodeupdater.h"
#include <QStandardItemModel>
#include <QSettings>
#include <QSplitter>
#include <QFileSystemModel>
#include <QClipboard>
#include <QFileDialog>
#include <QResizeEvent>
#include <QEvent>

/**
 * @file mainwindow.h
 * @brief The MainWindow class is responsible for managing the user interface (UI) and serves as the main controller
 * for the FileManager application. It acts as the main brain, coordinating various functionalities and interactions.
 */


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , listViewManager(ListViewManager::instance())
    , treeViewManager(TreeViewManager::instance())
    , visuals((VisualModeUpdater::instance()))
{
    ui->setupUi(this);

    listViewManager.instance().initialize(ui->QListView_FileViewer);
    treeViewManager.instance().initialize(ui->QTreeView_MainTree);

    connect(this, &MainWindow::populateTreeView, &treeViewManager.instance(), &TreeViewManager::updateModelForTreeView);
    connect(ui->QTreeView_MainTree, &QTreeView::clicked, &treeViewManager.instance(), &TreeViewManager::treeViewSelectedItemPath);
    connect(&treeViewManager.instance(), &TreeViewManager::updateViewData, this, &MainWindow::updateTreeView);
    connect(ui->QTreeView_MainTree, &QTreeView::expanded, &treeViewManager.instance(), &TreeViewManager::onTreeViewIndexExpanded);

    LongClickHandler *longClickHandler = new LongClickHandler(ui->QListView_FileViewer, this);
    connect(longClickHandler, &LongClickHandler::longClicked, &listViewManager.instance(), &ListViewManager::onListViewItemLongClicked);
    connect(this, &MainWindow::populateListView, &listViewManager.instance(), &ListViewManager::setModelForListView);
    connect(ui->QListView_FileViewer, &QListView::doubleClicked, &listViewManager.instance(), &ListViewManager::onListViewItemDoubleClicked);
    connect(&listViewManager.instance(), &ListViewManager::updateViewData, this, &MainWindow::updateTreeView);
    connect(&listViewManager.instance(), &ListViewManager::callFileViewerDialog, this, &MainWindow::openFileViewerDialog);
    connect(this, &MainWindow::updateHideFilesFilter, &listViewManager.instance(),&ListViewManager::shouldAcceptDirectories);

    connect(this, &MainWindow::updateLightModeBooleanData, &visuals.instance(), &VisualModeUpdater::updateLightModeBooleanData);

    splitter = splitterLeftAndRightPanels();
    connect(splitter, &QSplitter::splitterMoved, this, &MainWindow::handleSplitterMoved);

    treeViewManager.instance().setModelForTreeView(ui->QTreeView_MainTree);

    QTimer::singleShot(100, this, &MainWindow::initializeMainWindow);
    QTimer::singleShot(100, this, &MainWindow::updateIcons);
    QTimer::singleShot(100, this, &MainWindow::refresh);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Initializes the main window components and UI elements outside constructor.
 */
void MainWindow::initializeMainWindow()
{
    if (!loadLayout())
    {
        updateTreeView(nullptr);
        QStandardItemModel *standardModel = new QStandardItemModel(this);
        ui->QListView_FileViewer->setModel(standardModel);
    }

    ui->QTreeView_MainTree->header()->resizeSection(0,250);
    ui->QTreeView_MainTree->header()->hideSection(1);
}

/**
 * @brief Overrides the resizeEvent function to handle resizing of the main window.
 *
 * @param event A QResizeEvent representing the resize event.
 */
void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (event->spontaneous() && event->type() == QEvent::Resize)
    {
        ui->QListView_FileViewer->resize(ui->Widget_ListViewPanel->size());

        ModifiedFileSystemModel *model = dynamic_cast<ModifiedFileSystemModel*>(ui->QListView_FileViewer->model());
        if (model)
        {
            ui->QListView_FileViewer->setModel(nullptr);
            ui->QListView_FileViewer->setModel(model);
        }
    }

    QMainWindow::resizeEvent(event);
}

/**
 * @brief Handles the moved signal of the splitter, updating the file viewer list view accordingly.
 *
 * @param pos The new position of the splitter.
 * @param index The index of the splitter.
 */
void MainWindow::handleSplitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);

    ModifiedFileSystemModel *model = qobject_cast<ModifiedFileSystemModel*>(ui->QListView_FileViewer->model());
    if (model)
    {
        ui->QListView_FileViewer->setModel(nullptr);
        ui->QListView_FileViewer->setModel(model);
    }
}

/**
 * \brief Event triggered when the main window is about to close.
 *
 * \param event The close event.
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("FileManager", "Splitter");
    settings.setValue("SplitterSizes", splitter->saveState());
    saveLayout();
    QMainWindow::closeEvent(event);
}

/**
 * \brief Splits the QTreeView and QListView panels using a QSplitter.
 * Fixes side button panel position.
 */
QSplitter* MainWindow::splitterLeftAndRightPanels()
{
    QWidget *widgetLeftPanel = ui->Widget_TreeViewPanel;
    QWidget *widgetRightPanel = ui->Widget_ListViewPanel;
    QWidget *widgetButtonsPanel = ui->Widget_ButtonsPanel;

    QWidget *mainContentPanel = new QWidget(this);
    QHBoxLayout *mainContentLayout = new QHBoxLayout(mainContentPanel);

    QSplitter *mySplitter = new QSplitter(Qt::Horizontal, this);

    mySplitter->addWidget(widgetLeftPanel);
    mySplitter->addWidget(widgetRightPanel);

    mainContentLayout->addWidget(mySplitter);
    mainContentLayout->setContentsMargins(5, 0, 0, 0);
    mainContentLayout->setSpacing(5);

    QWidget *existingMainContentPanel = ui->MainContentPanel;
    existingMainContentPanel->deleteLater();

    mainContentPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *centralWidget = ui->centralwidget;
    QHBoxLayout *parentLayout = qobject_cast<QHBoxLayout*>(centralWidget->layout());

    if (parentLayout)
    {
        parentLayout->addWidget(mainContentPanel);
        parentLayout->addWidget(widgetButtonsPanel);
    }
    return mySplitter;
}

/**
 * \brief Loads layout from saved settings.
 */
bool MainWindow::loadLayout()
{
    QSettings settings("FileManager", "AppSettings");

    if (settings.allKeys().isEmpty())
    {
        ui->QPushButton_ShowPanel->hide();
        visuals.updateLightModeBooleanData(isLightMode, isGridLayout, isShowFiles);
        on_QPushButton_LightDarkModePushButton_clicked();
        on_QPushButton_HideFilesPushButton_clicked();
        on_QPushButton_LayoutPushButton_clicked();

        return false;
    }

    resize(settings.value("WindowSize", QSize(800, 600)).toSize());

    QString rootPath = settings.value("RootPath", QDir::homePath()).toString();
    if(!rootPath.isEmpty())
    {
        updateTreeView(rootPath);
        ui->QLineEdit_DirectoryTextDisplay->setText(rootPath);
    }

    isGridLayout = settings.value("isGridLayout").toBool();
    isShowFiles = settings.value("isShowFiles").toBool();
    isLightMode = settings.value("isLightMode").toBool();
    visuals.updateLightModeBooleanData(isLightMode, isGridLayout, isShowFiles);

    on_QPushButton_LightDarkModePushButton_clicked();
    on_QPushButton_HideFilesPushButton_clicked();
    on_QPushButton_LayoutPushButton_clicked();

    ui->Widget_HidePanel->setVisible(settings.value("HideButtons").toBool());

    QSettings splitterSize("FileManager", "Splitter");
    QByteArray splitterState = splitterSize.value("SplitterSizes").toByteArray();

    if (!splitterState.isEmpty())
    {
        splitter->restoreState(splitterState);
    }

    return true;
}

/**
 * \brief Saves layout to settings.
 */
void MainWindow::saveLayout()
{
    QSettings settings("FileManager", "AppSettings");

    settings.setValue("WindowSize", size());

    settings.setValue("isLightMode", !isLightMode);
    settings.setValue("isGridLayout", !isGridLayout);
    settings.setValue("isShowFiles", !isShowFiles);

    settings.setValue("HideButtons", !ui->Widget_HidePanel->isHidden());

    settings.setValue("RootPath", ui->QLineEdit_DirectoryTextDisplay->text());
    settings.sync();
}

/**
 * \brief Refreshes UI.
 */
void MainWindow::refresh()
{
    emit populateListView(treeViewManager.treeViewSelectedItemPath(ui->QTreeView_MainTree->currentIndex()));

    QModelIndex selectedIndex = ui->QTreeView_MainTree->currentIndex();
    QTimer::singleShot(100, this, [this, selectedIndex]() {
        ui->QTreeView_MainTree->expand(selectedIndex);
    });
}

/**
 * \brief Updates the tree view model, as well as connected components.
 *
 * \param path The path to selected index of the model.
 */
void MainWindow::updateTreeView(const QString &path)
{
    if(!path.isEmpty())
    {
        emit populateTreeView(path);
        emit populateListView(path);
        ui->QLineEdit_DirectoryTextDisplay->setText(path);
    }
}

/**
 * \brief Opens file view dialog.
 *
 * \param path The path to selected file.
 * \param isImage Bool which points towards correct behaviour.
 */
void MainWindow::openFileViewerDialog(const QString &path, bool isImage)
{
    FileViewerDialog* fileViewer = new FileViewerDialog(this);

    if(isImage)
    {
        fileViewer->openImage(path);
    }
    else
    {
        fileViewer->loadTextFile(path);
    }

    fileViewer->show();
}

/**
 * \brief Shows or hides side button panel.
 */
void MainWindow::on_QPushButton_ShowPanel_clicked()
{
    ui->Widget_HidePanel->setVisible(!ui->Widget_HidePanel->isVisible());
}

/**
 * \brief Opens a secondary window for adding a new folder to the selected directory.
 */
void MainWindow::on_QPushButton_AddFolder_clicked()
{
    QString selectedItemPath = listViewManager.instance().listViewSelectedItemPath(ui->QListView_FileViewer->currentIndex());

    if(selectedItemPath.isEmpty())
    {
        selectedItemPath = treeViewManager.treeViewSelectedItemPath(ui->QTreeView_MainTree->currentIndex());

        if(selectedItemPath.isEmpty())
        {
            return;
        }
    }

    FileOperationsDialog *secondaryWindow = new FileOperationsDialog(selectedItemPath, 'c', this);
    connect(secondaryWindow, &FileOperationsDialog::refresh, this, &MainWindow::refresh);

    secondaryWindow->show();
}

/**
 * \brief Opens a secondary window for deleting the selected file.
 */
void MainWindow::on_QPushButton_DeleteFile_clicked()
{
    QString selectedItemPath = listViewManager.listViewSelectedItemPath(ui->QListView_FileViewer->currentIndex());
    if(selectedItemPath.isEmpty())
    {
        return;
    }

    FileOperationsDialog *secondaryWindow = new FileOperationsDialog(selectedItemPath, 'd', this);
    connect(secondaryWindow, &FileOperationsDialog::refresh, this, &MainWindow::refresh);

    secondaryWindow->show();
}

/**
 * \brief Opens a secondary window for renaming the selected file.
 */
void MainWindow::on_QPushButton_RenameFile_clicked()
{
    QString selectedItemPath = listViewManager.listViewSelectedItemPath(ui->QListView_FileViewer->currentIndex());
    if(selectedItemPath.isEmpty())
    {
        return;
    }

    FileOperationsDialog *secondaryWindow = new FileOperationsDialog(selectedItemPath, 'r', this);
    connect(secondaryWindow, &FileOperationsDialog::refresh, this, &MainWindow::refresh);

    secondaryWindow->show();
}

/**
 * \brief Copies the text displayed in the directory text field to the clipboard.
 */
void MainWindow::on_QPushButton_CopyButton_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->QLineEdit_DirectoryTextDisplay->displayText());
}

/**
 * \brief Opens a file dialog to select a directory and updates the displayed directory path and model.
 */
void MainWindow::on_QPushButton_DirectoryButton_clicked()
{
    QString selectedDirectory = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                                  QDir::homePath(),
                                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!selectedDirectory.isEmpty())
    {
        ui->QLineEdit_DirectoryTextDisplay->setText(selectedDirectory);
        updateTreeView(selectedDirectory);
    }
}

/**
 * \brief Toggles the application's theme between light and dark modes.
 */
void MainWindow::on_QPushButton_LightDarkModePushButton_clicked()
{
    isLightMode = !isLightMode;

    visuals.instance().loadStyleSheet(*ui->centralwidget, *ui->QListView_FileViewer,*ui->QTreeView_MainTree);

    updateIcons();
}

/**
 * \brief Toggles the file viewer layout between grid and list views.
 */
void MainWindow::on_QPushButton_LayoutPushButton_clicked()
{
    isGridLayout = !isGridLayout;

    ui->QListView_FileViewer->setItemDelegate(nullptr);

    if (isGridLayout)
    {
        ui->QListView_FileViewer->setViewMode(QListView::IconMode);
        ItemNameModifierDelegate* delegate = new ItemNameModifierDelegate(this);
        delegate->setCustomSize(QSize(120, 120));
        ui->QListView_FileViewer->setItemDelegate(delegate);
    }
    else
    {
        ui->QListView_FileViewer->setViewMode(QListView::ListMode);
        ItemNameModifierDelegate* delegate = new ItemNameModifierDelegate(this);
        delegate->setCustomSize(QSize(800, 40));
        ui->QListView_FileViewer->setItemDelegate(delegate);
    }

    updateIcons();
}

/**
 * \brief Toggles the visibility of folders in the file viewer.
 */
void MainWindow::on_QPushButton_HideFilesPushButton_clicked()
{
    isShowFiles = !isShowFiles;

    emit updateHideFilesFilter(isShowFiles);
    updateTreeView(treeViewManager.treeViewSelectedItemPath(ui->QTreeView_MainTree->currentIndex()));

    updateIcons();
}

/**
 * \brief Updates style sheet for each icon.
 */
void MainWindow::updateIcons()
{
    emit updateLightModeBooleanData(isLightMode,isGridLayout, isShowFiles);
    visuals.instance().updateIconsToMode(*ui->QPushButton_CopyButton
                                         ,*ui->QPushButton_DirectoryButton
                                         ,*ui->QPushButton_RenameFile
                                         ,*ui->QPushButton_DeleteFile
                                         ,*ui->QPushButton_AddFolder
                                         ,*ui->QPushButton_LightDarkModePushButton
                                         ,*ui->QPushButton_LayoutPushButton
                                         ,*ui->QPushButton_HideFilesPushButton);
}
