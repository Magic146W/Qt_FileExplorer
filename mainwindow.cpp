#include "mainwindow.h"
#include "filelistmodel.h"
#include "fileviewerwindow.h"
#include "noemptyfoldersmodel.h"
#include "ui_mainwindow.h"
#include "fileexplorer.h"
#include "itemnamemodifierdelegate.h"
#include "secondarywindow.h"
#include <QFileDialog>
#include <QFileSystemModel>
#include <QSplitter>
#include <QVBoxLayout>
#include <QToolTip>
#include <QMouseEvent>
#include <QClipboard>
#include <QMessageBox>
#include <QStringListModel>
#include <QSettings>

/**
 * \brief The MainWindow class represents the main application window.
 *
 * This class manages the primary user interface and orchestrates interactions between various UI elements,
 * including menus, toolbars, central widgets, and status bars. It serves as the entry point for the application
 * and handles the initialization and setup of the graphical user interface.
 *
 * The MainWindow class provides functionalities for:
 * - Creating and displaying the main window layout.
 * - Handling user interactions and events.
 * - Managing and coordinating different components of the application.
 * - Connecting signals and slots for communication between UI elements and backend logic.
 * - Loading settings, managing resources, and initializing essential components on startup.
 *
 * Inherited from QMainWindow, it extends the QMainWindow functionalities with specific features
 * tailored to the application's needs.
 *
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    localFileSystemModel(new QFileSystemModel(this))
{
    ui->setupUi(this);

    FileExplorer *fileExplorerInstance = FileExplorer::getInstance();
    noEmptyfolderModel = new NoEmptyFoldersModel(this);
    fileListModel = new FileListModel(this);

    ui->FileContainerFrame->installEventFilter(this);

    // Establish connections for various signals and slots
    // Connection for file explorer instance signals
    // - Trigger population of file viewer and tree view based on file selection events
    QObject::connect(fileExplorerInstance, &FileExplorer::SelectionDone, this, &MainWindow::PopulateFileViewer);
    QObject::connect(fileExplorerInstance, &FileExplorer::SelectionDone, this, &MainWindow::ShowTree);
    QObject::connect(fileExplorerInstance, &FileExplorer::FileSelected, this, &MainWindow::GetSelectedItemPath);

    // Connection for tree view clicked signal
    // - Handles click events on the tree view in the file explorer
    QObject::connect(ui->TreeView, &QTreeView::clicked, fileExplorerInstance, &FileExplorer::OnTreeViewItemClicked);

    // Connection for file viewer clicked and double-clicked signals
    // - Handles click and double-click events on the file viewer list
    QObject::connect(ui->FileViewer, &QListView::clicked, fileExplorerInstance, &FileExplorer::OnListViewItemClicked);
    QObject::connect(ui->FileViewer, &QListView::doubleClicked, this, &MainWindow::onListViewItemDoubleClicked);


    fileExplorerInstance->setupFileSystemModel(QDir::rootPath());
    localFileSystemModel->setRootPath(QDir::rootPath());
    ui->TreeView->collapseAll();
    PopulateFileViewer(nullptr);

    //On first use make sure all code loaded properly.
    on_LightDarkModeCheckBox_stateChanged(ui->LightDarkModeCheckBox->checkState());
    on_LayoutCheckBox_stateChanged(ui->LayoutCheckBox->checkState());
    on_HideFilesCheckBox_stateChanged(ui->HideFilesCheckBox->checkState());

    ui->TreeView->header()->resizeSection(0, 250);
    splitterLeftAndRightPanels();



    loadLayout();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * \brief Handles specific events by filtering them through the event filter.
 * Monitors the resize event to trigger a layout handling function.
 *
 * \param obj The object to filter events for.
 * \param event The event that occurred.
 * \return True if the event is processed; otherwise, returns the base class implementation.
 */
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui->FileContainerFrame && event->type() == QEvent::Resize) {
        handleFileContainerFrameResize();
    }

    return QMainWindow::eventFilter(obj, event);
}

/**
 * \brief Event triggered when the main window is about to close.
 * Saves the current state of the splitter and layout settings before closing.
 *
 * \param event The close event.
 */
void MainWindow::closeEvent(QCloseEvent *event) {
    QSettings settings("Organization", "Application");
    settings.setValue("SplitterSizes", splitter->saveState());
    saveLayout();
    QMainWindow::closeEvent(event);
}

/**
 * \brief Loads saved layout settings to the application settings.
 * Stores the window size, checkbox states, root path, and splitter sizes.
 */
void MainWindow::loadLayout()
{
    QSettings settings("MyApp", "MyAppSettings");

    resize(settings.value("WindowSize", QSize(800, 600)).toSize());

    ui->LightDarkModeCheckBox->setChecked(settings.value("LightDarkModeCheckBox", false).toBool());
    ui->LayoutCheckBox->setChecked(settings.value("LayoutCheckBox", false).toBool());
    ui->HideFilesCheckBox->setChecked(settings.value("HideFilesCheckBox", false).toBool());

    QString rootPath = settings.value("RootPath", QDir::homePath()).toString();
    if(!rootPath.isEmpty())
    {
        localFileSystemModel->setRootPath(rootPath);
        ui->DirectoryTextDisplay->setText(rootPath);
        ShowTree(localFileSystemModel);
        PopulateFileViewer(localFileSystemModel);
    }

    QSettings splitterSize("Organization", "Application");
    QByteArray splitterState = splitterSize.value("SplitterSizes").toByteArray();

    if (!splitterState.isEmpty()) {
        splitter->restoreState(splitterState);
    }

    loadStyleSheet();
}

/**
 * \brief Saves the current layout settings to the application settings.
 * Stores the window size, checkbox states, root path.
 */
void MainWindow::saveLayout()
{
    QSettings settings("MyApp", "MyAppSettings");

    settings.setValue("WindowSize", size());

    settings.setValue("LightDarkModeCheckBox", ui->LightDarkModeCheckBox->isChecked());
    settings.setValue("LayoutCheckBox", ui->LayoutCheckBox->isChecked());
    settings.setValue("HideFilesCheckBox", ui->HideFilesCheckBox->isChecked());

    settings.setValue("RootPath", ui->DirectoryTextDisplay->text());
    settings.sync();
}

/**
 * \brief Updates UI elements, background colors and style sheets based on the selected mode (light or dark).
 */
void MainWindow::loadStyleSheet()
{
    QString resourcePath;
    QString styleSheet;
    if(ui->LightDarkModeCheckBox->isChecked())
    {
        resourcePath = ":/StyleSheet/StyleSheet_Light.txt";

        styleSheet = "background-color: #2a2a2a;";
        ui->BottomMainLine->setStyleSheet(styleSheet);
        ui->TopMainLine->setStyleSheet(styleSheet);
        ui->FileManagement_Line_1->setStyleSheet(styleSheet);
        ui->FileManagement_Line_2->setStyleSheet(styleSheet);
        ui->FileManagement_Line_3->setStyleSheet(styleSheet);

        styleSheet = "background-color: #f7ead0;";
        ui->TreeView->setStyleSheet(styleSheet);
        ui->FileViewer->setStyleSheet(styleSheet);
    }else
    {
        resourcePath = ":/StyleSheet/StyleSheet_Dark.txt";

        styleSheet = "background-color: #f7ead0;";
        ui->BottomMainLine->setStyleSheet(styleSheet);
        ui->TopMainLine->setStyleSheet(styleSheet);
        ui->FileManagement_Line_1->setStyleSheet(styleSheet);
        ui->FileManagement_Line_2->setStyleSheet(styleSheet);
        ui->FileManagement_Line_3->setStyleSheet(styleSheet);

        styleSheet = "background-color: #2a2a2a;";
        ui->TreeView->setStyleSheet(styleSheet);
        ui->FileViewer->setStyleSheet(styleSheet);
    }


    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QString styleSheetContent = file.readAll();
    file.close();

    ui->centralwidget->setStyleSheet(styleSheetContent);
}

/**
 * \brief Updates the icon color by appending a prefix for light or dark mode.
 *
 * @param filePath The file path of the icon to update.
 * @return The updated file path with the appropriate color prefix added.
 */
QString MainWindow::updateIconColorName(QString filePath)
{
    QString prefix = ui->LightDarkModeCheckBox->isChecked() ? "B" : "W";

    QFileInfo fileInfo(filePath);
    QString baseName = fileInfo.baseName();
    QString extension = fileInfo.completeSuffix();

    QString updatedFilePath = QString(":/Images/Icons/%1%2.%3").arg(baseName).arg(prefix).arg(extension);

    return updatedFilePath;
}

/**
 * \brief Updates the icons of buttons based on the selected light/dark mode.
 */
void MainWindow::updateIconsToMode()
{
    QString updatedIcon = updateIconColorName("copy.png");
    QIcon icon(updatedIcon);
    ui->CopyButton->setIcon(icon);

    updatedIcon = updateIconColorName("drive.png");
    icon.addFile(updatedIcon);
    ui->DirectoryButton->setIcon(icon);

    updatedIcon = updateIconColorName("edit.png");
    icon.addFile(updatedIcon);
    ui->RenameFile->setIcon(icon);

    updatedIcon = updateIconColorName("trash.png");
    icon.addFile(updatedIcon);
    ui->DeleteFile->setIcon(icon);

    updatedIcon = updateIconColorName("folder.png");
    icon.addFile(updatedIcon);
    ui->AddFolder->setIcon(icon);



    on_HideFilesCheckBox_stateChanged(ui->HideFilesCheckBox->checkState());
    on_LayoutCheckBox_stateChanged(ui->LayoutCheckBox->checkState());
}

/**
 * \brief Handles the resizing of the file container frame and refreshes the file viewer accordingly.
 */
void MainWindow::handleFileContainerFrameResize()
{
    ui->FileViewer->resize(ui->FileContainerFrame->size());

    QFileSystemModel *model = qobject_cast<QFileSystemModel*>(ui->FileViewer->model());
    if (model)
    {
        QString directoryPath = model->rootPath();
        model->setRootPath("");
        model->setRootPath(directoryPath);
    }
    else
    {
        FileListModel *currentModel = qobject_cast<FileListModel*>(ui->FileViewer->model());
        if (currentModel)
        {
            ui->FileViewer->setModel(nullptr);
            ui->FileViewer->setModel(fileListModel);
        }
    }
}

/**
 * \brief Updates the tree view with the provided file system model.
 * Sets the root path of the models, displays the tree view, and scrolls to the specified index.
 *
 * \param model The file system model to be displayed in the tree view.
 */
void MainWindow::ShowTree(QFileSystemModel* model)
{
    clearListViewSelection();
    QString path = model->rootPath();
    localFileSystemModel->setRootPath(path);
    noEmptyfolderModel->setRootPath(path);
    noEmptyfolderModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    ui->TreeView->collapseAll();
    ui->TreeView->setModel(noEmptyfolderModel);

    ui->DirectoryTextDisplay->setText(path);

    QModelIndex index = noEmptyfolderModel->index(path);
    if (index.isValid())
    {
        ui->TreeView->expand(index);
        ui->TreeView->setCurrentIndex(index);
        ui->TreeView->scrollTo(index, QAbstractItemView::PositionAtCenter);
    }
}

/**
 * \brief Populates the file viewer based on the provided file system model.
 * Updates the file viewer with the contents of the specified directory or model.
 *
 * \param model The file system model used to populate the file viewer.
 */
void MainWindow::PopulateFileViewer(QFileSystemModel* model)
{
    clearListViewSelection();
    if(!model)
    {
        localFileSystemModel->removeRows(0, localFileSystemModel->rowCount());
        localFileSystemModel->setRootPath("");
        ui->FileViewer->setModel(localFileSystemModel);
        ui->FileViewer->setRootIndex(localFileSystemModel->index(""));

    }
    else
    {
        localFileSystemModel = model;
        QString directoryPath = localFileSystemModel->rootPath();
        localFileSystemModel->setRootPath(directoryPath);

        if (!ui->HideFilesCheckBox->isChecked())
        {
            ui->FileViewer->setModel(localFileSystemModel);
            ui->FileViewer->setRootIndex(localFileSystemModel->index(directoryPath));
        } else
        {
            if(!directoryPath.isEmpty())
            {
                fileListModel->setFileData(directoryPath);
                ui->FileViewer->setModel(fileListModel);
            }
        }

        ui->FileViewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }
}

/**
 * \brief Splits the left and right panels using a QSplitter.
 */
void MainWindow::splitterLeftAndRightPanels()
{
    QWidget *widgetLeftPanel = ui->LeftPanelWidget;
    QWidget *widgetRightPanel = ui->CentralPanelWidget;

    splitter = new QSplitter(Qt::Horizontal, this);

    splitter->addWidget(widgetLeftPanel);
    splitter->addWidget(widgetRightPanel);

    QWidget *centralWidget = ui->centralwidget;
    QVBoxLayout *parentLayout = qobject_cast<QVBoxLayout*>(centralWidget->layout());

    if (parentLayout)
    {
        splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        parentLayout->addWidget(splitter);
    }

    QWidget *widget = ui->MainContentPanel;
    delete widget;
}

/**
 * \brief Handles the action when an item in the list view is double-clicked.
 * If the selected item is a directory, updates the file system model to the selected directory,
 * If file, opens an image viewer or a text viewer based on the selected item's file type.
 *
 * \param index The index of the selected item.
 */
void MainWindow::onListViewItemDoubleClicked(const QModelIndex &index)
{
    if (index.isValid())
    {
        QString filePath = localFileSystemModel->filePath(index);
        QFileInfo fileInfo(filePath);
        if (fileInfo.isDir())
        {
            localFileSystemModel->setRootPath(filePath);
            PopulateFileViewer(localFileSystemModel);
            ShowTree(localFileSystemModel);
            ui->DirectoryTextDisplay->setText(filePath);
        } else
        {
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                return;
            }

            QFileInfo fileInfo(filePath);

            if (fileInfo.suffix().compare("png", Qt::CaseInsensitive) == 0 ||
                fileInfo.suffix().compare("jpg", Qt::CaseInsensitive) == 0 ||
                fileInfo.suffix().compare("jpeg", Qt::CaseInsensitive) == 0 ||
                fileInfo.suffix().compare("bmp", Qt::CaseInsensitive) == 0 ||
                fileInfo.suffix().compare("gif", Qt::CaseInsensitive) == 0 ||
                fileInfo.suffix().compare("tiff", Qt::CaseInsensitive) == 0)
            {
                FileViewerWindow* imageViewer = new FileViewerWindow(this);
                imageViewer->openImage(filePath);
                imageViewer->show();
            }
            else if (fileInfo.suffix().compare("txt", Qt::CaseInsensitive) == 0 ||
                     fileInfo.suffix().compare("text", Qt::CaseInsensitive) == 0)
            {
                FileViewerWindow* textViewer = new FileViewerWindow(this);
                textViewer->loadTextFile(filePath);
                textViewer->show();
            }
        }
    }
}

/**
 * \brief Sets the list view selected item's path in the MainWindow.
 *
 * \param path The path of the selected item.
 */
void MainWindow::GetSelectedItemPath(QString path)
{
    selectedItemPath = path;
}

/**
 * \brief Clears the selection in the list view by resetting the selected item's path.
 */
void MainWindow::clearListViewSelection()
{
    selectedItemPath.clear();
}


/**
 * \brief Retrieves the path of the selected item in the tree view.
 * Clears the previous selection and returns the path of the currently selected item.
 *
 * \return The path of the selected item in the tree view.
 */
QString MainWindow::treeViewSelectedItemPath()
{
    clearListViewSelection();
    QModelIndex currentIndex = ui->TreeView->selectionModel()->currentIndex();

    if (currentIndex.isValid())
    {
        QVariant data = currentIndex.data(QFileSystemModel::FilePathRole);
        if (data.isValid())
        {
            QString path = data.toString();
            return path;
        }
    }
    return "";
}


/**
 * \brief Toggles the application's theme between light and dark modes.
 */
void MainWindow::on_LightDarkModeCheckBox_stateChanged(int state)
{
    QString styleSheet;

    if (state == Qt::Checked)
    {
        styleSheet = "QCheckBox#LightDarkModeCheckBox::indicator:checked { image: url(:/Images/Icons/Mode_Light.png); width: 64px; height: 32px;}";
    } else
    {
        styleSheet = "QCheckBox#LightDarkModeCheckBox::indicator:unchecked { image: url(:/Images/Icons/Mode_Dark.png); width: 64px; height: 32px;}";
    }

    ui->LightDarkModeCheckBox->setStyleSheet(styleSheet);


    updateIconsToMode();
    loadStyleSheet();
}

/**
 * \brief Toggles the file viewer layout between grid and list views.
 * Changes the icon size and layout mode in the file viewer accordingly.
 */
void MainWindow::on_LayoutCheckBox_stateChanged(int state)
{
    QString styleSheet;
    ui->FileViewer->setItemDelegate(nullptr);

    if (state == Qt::Checked)
    {
        QString updatedIcon = updateIconColorName("grid.png");
        styleSheet = QString("QCheckBox#LayoutCheckBox::indicator:checked { image: url(%1); width: 24px; height: 24px;}").arg(updatedIcon);
        ui->FileViewer->setViewMode(QListView::IconMode);
        ItemNameModifierDelegate* delegate = new ItemNameModifierDelegate(this);
        delegate->setCustomSize(QSize(120, 120));
        ui->FileViewer->setItemDelegate(delegate);
    } else
    {
        QString updatedIcon = updateIconColorName("list.png");
        styleSheet = QString("QCheckBox#LayoutCheckBox::indicator:unchecked { image: url(%1); width: 24px; height: 24px;}").arg(updatedIcon);
        ui->FileViewer->setViewMode(QListView::ListMode);
        ItemNameModifierDelegate* delegate = new ItemNameModifierDelegate(this);
        delegate->setCustomSize(QSize(30, 30));
        ui->FileViewer->setItemDelegate(delegate);
    }

    ui->LayoutCheckBox->setStyleSheet(styleSheet);
    handleFileContainerFrameResize();
}

/**
 * \brief Toggles the visibility of folders in the file viewer.
 */
void MainWindow::on_HideFilesCheckBox_stateChanged(int state)
{
    QString styleSheet;

    if (state == Qt::Checked)
    {
        QString updatedIcon = updateIconColorName("eye_off.png");
        styleSheet = QString("QCheckBox#HideFilesCheckBox::indicator:checked { image: url(%1); width: 24px; height: 24px;}").arg(updatedIcon);
    } else
    {
        QString updatedIcon = updateIconColorName("eye.png");
        styleSheet = QString("QCheckBox#HideFilesCheckBox::indicator:unchecked { image: url(%1); width: 24px; height: 24px;}").arg(updatedIcon);
    }

    PopulateFileViewer(localFileSystemModel);
    ui->HideFilesCheckBox->setStyleSheet(styleSheet);
}


/**
 * \brief Opens a file dialog to select a directory and updates the displayed directory path.
 * Also updates the file system model with the selected directory.
 */
void MainWindow::on_DirectoryButton_clicked()
{
    QString selectedDirectory = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                                  QDir::homePath(),
                                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    ui->DirectoryTextDisplay->setText(selectedDirectory);
    FileExplorer::getInstance()->setupFileSystemModel(selectedDirectory);
}

/**
 * \brief Copies the text displayed in the directory text field to the clipboard.
 */
void MainWindow::on_CopyButton_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->DirectoryTextDisplay->displayText());
}

/**
 * \brief Opens a secondary window for adding a new folder to the selected directory.
 *        If no directory is selected, uses the current tree view selection.
 */
void MainWindow::on_AddFolder_clicked()
{
    if(selectedItemPath.isEmpty())
    {
        selectedItemPath = treeViewSelectedItemPath();
    }

    SecondaryWindow *secondaryWindow = new SecondaryWindow(selectedItemPath, 'c');
    connect(secondaryWindow, &SecondaryWindow::selectionClear, this, &MainWindow::clearListViewSelection);

    secondaryWindow->show();
}

/**
 * \brief Opens a secondary window for renaming the selected file.
 *        Does nothing if no file is selected.
 */
void MainWindow::on_RenameFile_clicked()
{
    if(selectedItemPath.isEmpty())
    {
        return;
    }

    SecondaryWindow *secondaryWindow = new SecondaryWindow(selectedItemPath, 'r');
    connect(secondaryWindow, &SecondaryWindow::selectionClear, this, &MainWindow::clearListViewSelection);

    secondaryWindow->show();
}

/**
 * \brief Opens a secondary window for deleting the selected file.
 *        Does nothing if no file is selected.
 */
void MainWindow::on_DeleteFile_clicked()
{
    if(selectedItemPath.isEmpty())
    {
        return;
    }

    SecondaryWindow *secondaryWindow = new SecondaryWindow(selectedItemPath, 'd');
    connect(secondaryWindow, &SecondaryWindow::selectionClear, this, &MainWindow::clearListViewSelection);

    secondaryWindow->show();
}
