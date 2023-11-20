#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QObject>
#include <QWidget>
#include <QTreeView>
#include <QListView>
#include <QToolBar>
#include <QVBoxLayout>
#include <QFileSystemModel>

class FileExplorer : public QObject
{
    Q_OBJECT

public:
    static FileExplorer* getInstance();
    void setupFileSystemModel(const QString& selectedDirectory = QString());

private:
    explicit FileExplorer(QObject *parent = nullptr);
    static FileExplorer* instance;
    QFileSystemModel *fileSystemModel;
    QString selectedDirectoryPath;


signals:
    void SelectionDone(QFileSystemModel *model);


public slots:
    void OnTreeViewItemClicked(const QModelIndex &index);
    void GetDirectoryContent();

};

#endif // FILEEXPLORER_H
