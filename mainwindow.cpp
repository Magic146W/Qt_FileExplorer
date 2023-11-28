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


    QObject::connect(fileExplorerInstance, &FileExplorer::SelectionDone, this, &MainWindow::PopulateFileViewer);
    QObject::connect(fileExplorerInstance, &FileExplorer::SelectionDone, this, &MainWindow::ShowTree);
    QObject::connect(fileExplorerInstance, &FileExplorer::FileSelected, this, &MainWindow::GetSelectedItemPath);

    QObject::connect(ui->TreeView, &QTreeView::clicked, fileExplorerInstance, &FileExplorer::OnTreeViewItemClicked);

    QObject::connect(ui->FileViewer, &QListView::clicked, fileExplorerInstance, &FileExplorer::OnListViewItemClicked);
    QObject::connect(ui->FileViewer, &QListView::doubleClicked, this, &MainWindow::onListViewItemDoubleClicked);


    fileExplorerInstance->setupFileSystemModel(QDir::rootPath());
    localFileSystemModel->setRootPath(QDir::rootPath());
    ui->TreeView->collapseAll();


    ui->TreeView->header()->resizeSection(0, 300);
    splitterLeftAndRightPanels();
    PopulateFileViewer(nullptr);


    on_LightDarkModeCheckBox_stateChanged(ui->LightDarkModeCheckBox->checkState());
    on_LayoutCheckBox_stateChanged(ui->LayoutCheckBox->checkState());
    on_HideFilesCheckBox_stateChanged(ui->HideFilesCheckBox->checkState());


    loadLayout();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui->FileContainerFrame && event->type() == QEvent::Resize) {
        handleFileContainerFrameResize();
    }

    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QSettings settings("Organization", "Application");
    settings.setValue("SplitterSizes", splitter->saveState());
    saveLayout();
    QMainWindow::closeEvent(event);
}

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


void MainWindow::loadStyleSheet()
{
    QString resourcePath;
    QString styleSheet;
    if(ui->LightDarkModeCheckBox->isChecked())
    {
        resourcePath = ":/StyleSheet/StyleSheet_Light.txt";

        styleSheet = "background-color: #1a0b27;";
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

        styleSheet = "background-color: #1a0b27;";
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

QString MainWindow::updateIconColorName(QString filePath)
{
    QString prefix = ui->LightDarkModeCheckBox->isChecked() ? "B" : "W";

    QFileInfo fileInfo(filePath);
    QString baseName = fileInfo.baseName();
    QString extension = fileInfo.completeSuffix();

    QString updatedFilePath = QString(":/Images/Icons/%1%2.%3").arg(baseName).arg(prefix).arg(extension);

    return updatedFilePath;
}

void MainWindow::updateIconsToMode()
{
    QString updatedIcon = updateIconColorName("copy.png");
    QIcon icon(updatedIcon);
    ui->CopyButton->setIcon(icon);

    updatedIcon = updateIconColorName("Drive.png");
    icon.addFile(updatedIcon);
    ui->DirectoryButton->setIcon(icon);

    updatedIcon = updateIconColorName("edit.png");
    icon.addFile(updatedIcon);
    ui->RenameFile->setIcon(icon);

    updatedIcon = updateIconColorName("trash.png");
    icon.addFile(updatedIcon);
    ui->DeleteFile->setIcon(icon);

    updatedIcon = updateIconColorName("Folder.png");
    icon.addFile(updatedIcon);
    ui->AddFolder->setIcon(icon);



    on_HideFilesCheckBox_stateChanged(ui->HideFilesCheckBox->checkState());
    on_LayoutCheckBox_stateChanged(ui->LayoutCheckBox->checkState());
}

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

void MainWindow::GetSelectedItemPath(QString path)
{
    selectedItemPath = path;
}

void MainWindow::clearListViewSelection()
{
    selectedItemPath.clear();
}

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
        QString updatedIcon = updateIconColorName("List.png");
        styleSheet = QString("QCheckBox#LayoutCheckBox::indicator:unchecked { image: url(%1); width: 24px; height: 24px;}").arg(updatedIcon);
        ui->FileViewer->setViewMode(QListView::ListMode);
        ItemNameModifierDelegate* delegate = new ItemNameModifierDelegate(this);
        delegate->setCustomSize(QSize(30, 30));
        ui->FileViewer->setItemDelegate(delegate);
    }

    ui->LayoutCheckBox->setStyleSheet(styleSheet);
    handleFileContainerFrameResize();
}

void MainWindow::on_HideFilesCheckBox_stateChanged(int state)
{
    QString styleSheet;

    if (state == Qt::Checked)
    {
        QString updatedIcon = updateIconColorName("Eye_off.png");
        styleSheet = QString("QCheckBox#HideFilesCheckBox::indicator:checked { image: url(%1); width: 24px; height: 24px;}").arg(updatedIcon);
    } else
    {
        QString updatedIcon = updateIconColorName("eye.png");
        styleSheet = QString("QCheckBox#HideFilesCheckBox::indicator:unchecked { image: url(%1); width: 24px; height: 24px;}").arg(updatedIcon);
    }

    PopulateFileViewer(localFileSystemModel);
    ui->HideFilesCheckBox->setStyleSheet(styleSheet);
}

void MainWindow::on_DirectoryButton_clicked()
{
    QString selectedDirectory = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                                  QDir::homePath(),
                                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    ui->DirectoryTextDisplay->setText(selectedDirectory);
    FileExplorer::getInstance()->setupFileSystemModel(selectedDirectory);
}

void MainWindow::on_CopyButton_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->DirectoryTextDisplay->displayText());
}

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
