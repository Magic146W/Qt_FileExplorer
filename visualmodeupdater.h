#ifndef VISUALMODEUPDATER_H
#define VISUALMODEUPDATER_H

#include <QObject>
#include <QListView>
#include <QTreeView>
#include <QPushButton>
#include <QPushButton>
#include <QFileInfo>
#include <QWidget>
#include <QCheckBox>

class VisualModeUpdater : public QObject
{
    Q_OBJECT
public:
    static VisualModeUpdater& instance();
    VisualModeUpdater();
    ~VisualModeUpdater();

    void updateLightModeBooleanData(bool isLightModeActivated, bool isLayoutGrid, bool isShowFiles);

    QCheckBox *layout;
    QCheckBox *files;

private:
    bool isLightModeActivated;
    bool isLayoutGrid;
    bool isShowFiles;

public slots:
    void loadStyleSheet(QWidget &centralwidget, QListView &listView, QTreeView &treeView);
    void updateIconsToMode(QPushButton &copyButton, QPushButton &driveButton, QPushButton &editButton, QPushButton &trashButton, QPushButton &folderButton,QPushButton &mode, QPushButton &layout, QPushButton &hide);
    QString updateIconColorName(const QString &filePath);

signals:

};

#endif // VISUALMODEUPDATER_H
