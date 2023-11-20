#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileexplorer.h"
#include <QFileDialog>
#include <QFileSystemModel>
#include <QSplitter>
#include <QVBoxLayout>
#include <QToolTip>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    FileExplorer *fileExplorerInstance = FileExplorer::getInstance();

    QObject::connect(fileExplorerInstance, &FileExplorer::SelectionDone, this, &MainWindow::ShowTree);
    QObject::connect(ui->TreeView, &QTreeView::clicked, fileExplorerInstance, &FileExplorer::OnTreeViewItemClicked);

    fileExplorerInstance->setupFileSystemModel(QDir::rootPath());


    connect(ui->LightDarkModeCheckBox, &QCheckBox::stateChanged, this, &MainWindow::on_LightDarkMode_stateChanged);
    on_LightDarkMode_stateChanged(ui->LightDarkModeCheckBox->checkState());

    connect(ui->LayoutCheckBox, &QCheckBox::stateChanged, this, &MainWindow::on_LayoutCheckBox_stateChanged);
    on_LayoutCheckBox_stateChanged(ui->LayoutCheckBox->checkState());

    connect(ui->HideFilesCheckBox, &QCheckBox::stateChanged, this, &MainWindow::on_HideFilesCheckBox_stateChanged);
    on_HideFilesCheckBox_stateChanged(ui->HideFilesCheckBox->checkState());


    MainWindowDisplayChanges();


    //if(dataToLoad==null)
    QString tooltipText = "C:\\";
    SetToolTipBehavior(ui->DirectoryTextDisplay, tooltipText);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetToolTipBehavior(QObject* object, const QString& tooltipText)
{
    if (tooltipMap.contains(object))
    {
        return;
    }

    tooltipMap[object] = tooltipText;

    connect(object, &QObject::objectNameChanged, this, [=]()
            {
                if (tooltipMap.contains(object))
                {
                    tooltipMap.remove(object);
                }
            });

    connect(object, &QObject::destroyed, this, [=]()
            {
                if (tooltipMap.contains(object))
                {
                    tooltipMap.remove(object);
                }
            });

    object->installEventFilter(this);
}

void MainWindow::OnObjectEntered(QObject* obj)
{
    QString tooltipText = tooltipMap.value(obj);
    if (!tooltipText.isEmpty())
    {
        QToolTip::showText(QCursor::pos(), tooltipText);
    }
}

void MainWindow::OnObjectLeft(QObject* obj)
{
    Q_UNUSED(obj);
    QToolTip::hideText();
}

void MainWindow::MainWindowDisplayChanges()
{
    QWidget *widgetLeftPanel = ui->LeftPanelWidget;
    QWidget *widgetRightPanel = ui->CentralPanelWidget;

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

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

    //functions...
    //load from saved data
    //save data
}

void MainWindow::ShowTree(QFileSystemModel* model)
{
    ui->TreeView->setModel(model);
}

#pragma region CheckBoxManagement{
void MainWindow::on_LightDarkMode_stateChanged(int state)
{
    QString styleSheet;

    if (state == Qt::Checked) {
        styleSheet = "QCheckBox#LightDarkModeCheckBox::indicator:checked { image: url(:/Images/Icons/Mode_Light.png); width: 64px; height: 32px;}";
    } else {
        styleSheet = "QCheckBox#LightDarkModeCheckBox::indicator:unchecked { image: url(:/Images/Icons/Mode_Dark.png); width: 64px; height: 32px;}";
    }

    ui->LightDarkModeCheckBox->setStyleSheet(styleSheet);
    //manage mode
}

void MainWindow::on_LayoutCheckBox_stateChanged(int state)
{
    QString styleSheet;

    if (state == Qt::Checked) {
        styleSheet = "QCheckBox#LayoutCheckBox::indicator:checked { image: url(:/Images/Icons/grid.svg); width: 24px; height: 24px;}";
    } else {
        styleSheet = "QCheckBox#LayoutCheckBox::indicator:unchecked { image: url(:/Images/Icons/list.svg); width: 24px; height: 24px;}";
    }

    ui->LayoutCheckBox->setStyleSheet(styleSheet);
    //do something for layout
}

void MainWindow::on_HideFilesCheckBox_stateChanged(int state)
{
    QString styleSheet;

    if (state == Qt::Checked) {
        styleSheet = "QCheckBox#HideFilesCheckBox::indicator:checked { image: url(:/Images/Icons/eye-off.svg); width: 24px; height: 24px;}";
    } else {
        styleSheet = "QCheckBox#HideFilesCheckBox::indicator:unchecked { image: url(:/Images/Icons/eye.svg); width: 24px; height: 24px;}";
    }

    ui->HideFilesCheckBox->setStyleSheet(styleSheet);
    //hide/show files
}
#pragma endregion}



void MainWindow::on_SearchIcon_clicked()
{
    //check search options
    //go through files inside list of selected folder, if not there search further into nested folders
}

void MainWindow::on_DirectoryButton_clicked()
{
    QString selectedDirectory = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                                  QDir::homePath(),
                                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!selectedDirectory.isEmpty())
    {
        ui->DirectoryTextDisplay->setText(selectedDirectory);
        SetToolTipBehavior(ui->DirectoryTextDisplay, selectedDirectory);
        fileExplorer->setupFileSystemModel(selectedDirectory);
    }
}





