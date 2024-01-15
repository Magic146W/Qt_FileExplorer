#include "visualmodeupdater.h"
#include <QIcon>
#include <QListView>
#include <QTreeView>
#include <QPushButton>
#include <QPushButton>
#include <QFileInfo>
#include <QWidget>
#include <QCheckBox>

/**
 * @file visualmodeupdater.h
 * @brief The VisualModeUpdater class manages the visual aspects of the application based on selected modes.
 */

VisualModeUpdater::VisualModeUpdater()
{

}

VisualModeUpdater::~VisualModeUpdater()
{}

VisualModeUpdater& VisualModeUpdater::instance()
{
    static VisualModeUpdater instance;
    return instance;
}

/**
     * @brief Updates boolean data related to light mode, layout grid, and file visibility.
     * @param isLightModeActivated Whether light mode is activated.
     * @param isLayoutGrid Whether the layout is in grid mode.
     * @param isShowFiles Whether files are set to be shown.
     */
void VisualModeUpdater::updateLightModeBooleanData(bool isLightModeActivated, bool isLayoutGrid, bool isShowFiles)
{
    this->isLightModeActivated = isLightModeActivated;
    this->isLayoutGrid = isLayoutGrid;
    this->isShowFiles = isShowFiles;
}

/**
 * \brief Updates the icon color by appending a prefix for light or dark mode.
 *
 * @param filePath The file path of the icon to update.
 * @return The updated file path with the appropriate color prefix added.
 */
QString VisualModeUpdater::updateIconColorName(const QString &filePath)
{
    QString prefix = isLightModeActivated ? "W" : "B";

    QFileInfo fileInfo(filePath);
    QString baseName = fileInfo.baseName();
    QString extension = fileInfo.completeSuffix();

    QString updatedFilePath = QString(":/Images/Icons/%1%2.%3").arg(baseName, prefix, extension);

    if (QFile::exists(updatedFilePath))
    {
        return updatedFilePath;
    }

    return filePath;
}

/**
 * \brief Updates the icons of buttons based on the selected light/dark mode.
 */
void VisualModeUpdater::updateIconsToMode(QPushButton &copyButton, QPushButton &driveButton, QPushButton &editButton, QPushButton &trashButton, QPushButton &folderButton
                                          ,QPushButton &mode, QPushButton &layout, QPushButton &hide)
{
    QString updatedIcon = updateIconColorName("copy.png");
    QIcon icon(updatedIcon);
    copyButton.setIcon(icon);

    updatedIcon = updateIconColorName("drive.png");
    icon.addFile(updatedIcon);
    driveButton.setIcon(icon);

    updatedIcon = updateIconColorName("edit.png");
    icon.addFile(updatedIcon);
    editButton.setIcon(icon);

    updatedIcon = updateIconColorName("trash.png");
    icon.addFile(updatedIcon);
    trashButton.setIcon(icon);

    updatedIcon = updateIconColorName("folder.png");
    icon.addFile(updatedIcon);
    folderButton.setIcon(icon);

    updatedIcon = updateIconColorName("mode.png");
    icon = QIcon();
    icon.addFile(updatedIcon);
    mode.setIcon(icon);

    if(isLayoutGrid)
    {
        updatedIcon = updateIconColorName("grid.png");
    }
    else
    {
        updatedIcon = updateIconColorName("list.png");
    }
    icon.addFile(updatedIcon);
    layout.setIcon(icon);

    if(isShowFiles)
    {
        updatedIcon = updateIconColorName("eye.png");
    }
    else
    {
        updatedIcon = updateIconColorName("eye_off.png");
    }
    icon.addFile(updatedIcon);
    hide.setIcon(icon);
}

/**
 * \brief Updates UI elements, background colors and style sheets based on the selected mode (light or dark).
 */
void VisualModeUpdater::loadStyleSheet(QWidget &centralwidget, QListView &listView, QTreeView &treeView)
{
    QString resourcePath;
    QString styleSheet;
    if (isLightModeActivated)
    {
        resourcePath = ":/StyleSheet/StyleSheet_Light.txt";
        styleSheet = "background-color: #f7ead0;";
    }
    else
    {
        resourcePath = ":/StyleSheet/StyleSheet_Dark.txt";
        styleSheet = "background-color: #2a2a2a;";
    }

    treeView.setStyleSheet(styleSheet);
    listView.setStyleSheet(styleSheet);

    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QString styleSheetContent = file.readAll();
    file.close();

    centralwidget.setStyleSheet(styleSheetContent);
}
